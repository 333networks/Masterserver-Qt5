#include "statuschecker.h"

void StatusChecker::onUdpResponseRead()
{
    // read now called, reset workaround counter
    _missedReadCalls = 0;

    while ( _udpSocket.hasPendingDatagrams() )
    {
        // get sender and payload
        QNetworkDatagram    datagram        = _udpSocket.receiveDatagram();
        QString             senderAddress   = QHostAddress( datagram.senderAddress().toIPv4Address() ).toString();
        unsigned short      senderPort      = datagram.senderPort();
        QString             receiveBuffer   = datagram.data();
        receiveBuffer = receiveBuffer.toLatin1();

        // shorthand label
        QString senderAddressLabel = QStringLiteral("%1:%2").arg(senderAddress, QString::number(senderPort));
        _coreObject->Log.logEvent("udp", QStringLiteral("%1 sent '%2'").arg(senderAddressLabel, receiveBuffer ) );

        // ignore empty data packets (query port forwarded to a game port)
        if (receiveBuffer.length() <= 0)
            continue;

        // determine protocol and response based on the first character (backslash, byte value, ... )
        unsigned short protocol_chooser = receiveBuffer.at(0).unicode();
        if (protocol_chooser != 92)
            continue;

        // buffer complete? else wait for data to be complete
        _dataBuffer[senderAddressLabel] += receiveBuffer;

        // status response or validate response? (either status or secure, not both)
        if ( receiveBuffer.contains("\\validate\\") )
        {
            // parse key/value pairs and QHash label
            QMultiHash<QString, QString> receiveData = parseGameSpy0Buffer(_dataBuffer[senderAddressLabel]);

            // load existing information
            QString secure   = _secureBuffer.value(senderAddressLabel).secure;
            QString gamename = _secureBuffer.value(senderAddressLabel).gamename;

            // if entry, secure, gamename and/or cipher do not exist, AuthResult will be false+invalid
            AuthResult authResult = validateGamename(false, // this is not a beacon
                                                     gamename,
                                                     receiveData.value("validate",""),
                                                     _coreObject->SupportedGames.value(gamename).cipher,
                                                     secure,
                                                     receiveData.value("enctype", "0").toInt() );

            // compare with received response
            if ( authResult.auth )
            {
                // server authenticated - log and add to database
                _coreObject->Log.logEvent("secure", QStringLiteral("successful validate from %1 for %2")
                               .arg(senderAddressLabel, gamename));

                // update the existing entry
                updateServer(senderAddress, senderPort, gamename, false, true);

                // remove from secure buffer
                _secureBuffer.remove(senderAddressLabel);
            }
            else // log failed validate
            {
                // set validate false (but update last response time)
                updateServer(senderAddress, senderPort, gamename, false, false);
                _coreObject->Log.logEvent("secure", QStringLiteral("failed validate from %1 for %2")
                               .arg(senderAddressLabel, gamename));
                _coreObject->Log.logEvent("secure", QStringLiteral("secure: '%1', gamename: '%2', validate: '%3', expected: '%4'")
                                            .arg(secure, gamename, receiveData.value("validate", "null"), authResult.validate ));
            }

            // clear receive buffer
            _dataBuffer.remove(senderAddressLabel);

            // there should be no further data (ignore)
            continue;
        }

        // all status query data received?
        if (receiveBuffer.contains("\\final\\"))
        {

            // parse key/value pairs and QHash label
            QMultiHash<QString, QString> receiveData = parseGameSpy0Buffer(_dataBuffer[senderAddressLabel]);

            // update or insert primary details
            if ( ! updateServer(senderAddress, senderPort, receiveData.value("gamename", "unknown"), false, false) )
            {
                // add to database
                insertServer(senderAddress, senderPort, receiveData.value("gamename", "unknown"), false);
            }

            // then update detailed information
            if ( ! updateServerInfo(senderAddress, senderPort, receiveData) )
            {
                // insert and update new entry (this does NOT insert to the serverlist, only info)
                // this assumes that an entry with this ip/port exists in the serverlist (fails silently)
                insertServerInfo(senderAddress, senderPort);
                updateServerInfo(senderAddress, senderPort, receiveData);
            }

            // update player info (removes old playerdata entries)
            insertPlayerInfo(senderAddress, senderPort, receiveData);

            // clear receive buffer
            _dataBuffer.remove(senderAddressLabel);

        } // if final
    }
}
