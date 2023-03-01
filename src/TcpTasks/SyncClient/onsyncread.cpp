#include "syncclient.h"

void SyncClient::onSyncRead()
{
    // reset timeout after receiving (any) data
    _timeOut.start();

    // read from tcp connection and append to buffer
    QByteArray receiveBuffer = _tcpSocket.readAll();
    _rxBuffer.append( receiveBuffer );

    // prevent large heaps of text -- log only relevant message, not masterserver data
    if ( receiveBuffer.length() > 30 )
    {
        // log size of data
        _coreObject->Log.logEvent("tcp", QStringLiteral("%1 sent %2 characters")
                                  .arg(_clientLabel, QString::number(receiveBuffer.length()) ) );
    }
    else
    {
        // log message
        _coreObject->Log.logEvent("tcp", QStringLiteral("%1 sent '%2'")
                                  .arg(_clientLabel, receiveBuffer.data()) );
    }

    // remote masterserver opens with secure challenge
    if ( _rxBuffer.contains("secure") )
    {
        // parse to hash
        QMultiHash<QString, QString> receiveData = parseGameSpy0Buffer(_rxBuffer.toLatin1());

        // generate response
        QStringList response = replyQuery(receiveData);

        // return response
        _tcpSocket.write(response.join("").toLatin1());

        // sync request
        QString request = QStringLiteral("\\sync\\%1\\msid\\%2")
                .arg(_coreObject->Settings.SyncerSettings.syncGames,
                     _coreObject->masterserverIdentity);
        _tcpSocket.write(request.toLatin1());

        // all relevant information received. clear buffer for next interaction
        _rxBuffer = "";
        return;
    }

    if ( _rxBuffer.contains("final") )
    {
        // parse to hash: receivedData format is {gamename} => {string of addresses}
        QMultiHash<QString, QString> receiveData = parseGameSpy0Buffer(_rxBuffer.toLatin1());
        receiveData.remove("final"); // prevent "final" from registering as gamename

        // count number of addresses for logging
        int totalServerCount = 0;

        // use transaction for SQLite
        QSqlDatabase::database().transaction();

        // parse to list of list of <ServerInfo>
        QHashIterator<QString,QString> receiveDataIterator(receiveData);
        while ( receiveDataIterator.hasNext() )
        {
            // {gamename} => {string of addresses}
            receiveDataIterator.next();
            QString gamename          = receiveDataIterator.key();
            QString addressBufferList = receiveDataIterator.value();

            // split address list in single addresses
            QStringListIterator listIterator( addressBufferList.split(" ", QString::SkipEmptyParts) );
            while ( listIterator.hasNext() )
            {
                // address (ip:port)
                QString        addr       = listIterator.next();

                // older Qt5 masterservers sync in ::ffff:127.0.0.1 format, trim the '::ffff:'
                addr.remove("::ffff:");

                // (address cont.)
                QStringList    address    = addr.split(':');
                unsigned short remotePort = address.takeLast().toUShort();
                QString        remoteAddr = address.join(":"); // IPv4 has only 1 element, IPv6 has 4 that need joining with ':'

                // valid address?
                if ( ! QHostAddress(remoteAddr).isNull() and remotePort > 0 )
                {
                    // if it does not exist in the db, insert
                    if ( ! updateSyncedServer(remoteAddr, remotePort) )
                    {
                        // add
                        insertServer(remoteAddr, remotePort, gamename, false);
                    }
                }
                totalServerCount++;
            } // has next address
        } // has next game

        // commit SQLite
        QSqlDatabase::database().commit();

        // report in log
        _coreObject->Log.logEvent("sync", QStringLiteral("received %1 servers in %2 games from %3")
                                  .arg( QString::number(totalServerCount),
                                        QString::number(receiveData.count()),
                                        _clientLabel)
                                  );
    } // if final

    // else keep appending data until \\final\\ is received
}
