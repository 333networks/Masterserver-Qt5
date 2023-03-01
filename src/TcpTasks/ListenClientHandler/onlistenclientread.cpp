#include "listenclienthandler.h"

void ListenClientHandler::onListenClientRead()
{
    // clear buffer from previous intereactions
    if ( _rxBuffer.contains("\\final\\") )
    {
        // if the keyword "\\final\\" is already found, a previous interaction was completed
        // it is now safe to clear the buffer
        _rxBuffer = "";
    }

    // read incoming data
    QByteArray receiveBuffer = _tcpSocket->readAll();
    _coreObject->Log.logEvent("tcp", QStringLiteral("%1 sent '%2'")
                              .arg(_clientLabel, receiveBuffer.data()) );

    // reset timeout when data is received
    _timeOut.start();

    // add data to buffer and wait for the interaction to be complete
    _rxBuffer += receiveBuffer;
    if ( ! receiveBuffer.contains("\\final\\") )
    {
        // wait for data to be complete
        return;
    }

    // process data according to \key\value formatting
    QMultiHash<QString, QString> receiveData = parseGameSpy0Buffer(_rxBuffer.toLatin1());

    // part 1: validation challenge.
    if ( receiveData.contains("validate") )
    {
        // combined queries provide two gamenames: one for authentication and one for list request.
        QString gamename = ( receiveData.values("gamename").size() >= 2
                             ? receiveData.values("gamename").takeLast() // removes duplicate gamename
                             : receiveData.value("gamename","") );

        // sanity check
        if ( _coreObject->SupportedGames.contains(gamename) )
        {
            // get response
            AuthResult authResult = validateGamename(false, // tcp client, not a beacon
                                                     gamename,
                                                     receiveData.value("validate",""),
                                                     _coreObject->SupportedGames.value(gamename).cipher,
                                                     _secure,
                                                     receiveData.value("enctype", "0").toInt() );

            // authenticate
            if ( authResult.auth )
            {
                _hasValidated = true;
                _coreObject->Log.logEvent("secure", QStringLiteral("%1 passed validation for %2").arg(_clientLabel, gamename));
            }
            else
            {
                _coreObject->Log.logEvent("secure", QStringLiteral("%1 failed validation for %2").arg(_clientLabel, gamename));

                // log detailed information on failure
                _coreObject->Log.logEvent("secure", QStringLiteral("secure: '%1', gamename: '%2', validate: '%3', expected: '%4'")
                                            .arg(_secure, gamename, receiveData.value("validate", "null"), authResult.validate ));

                // kick client after unsuccessful validation
                this->disconnect();
            }
        }
        else
        {
            _coreObject->Log.logEvent("secure", QStringLiteral("%1 tried to validate for unknown game %2").arg(_clientLabel, gamename));

            // log detailed information on failure
            _coreObject->Log.logEvent("secure", QStringLiteral("secure: '%1', gamename: '%2', validate: '%3'")
                                        .arg(_secure, gamename, receiveData.value("validate", "NULL") ));
            // kick client after unsuccessful validation
            this->disconnect();
        }

    } // hasValidated ?

    // part 2: after validation, send serverlist
    if ( receiveData.contains("list") )
    {
        // list request is only granted after validation
        if ( ! _hasValidated )
        {
            // kick client after unsuccessful validation
            _coreObject->Log.logEvent("secure", QStringLiteral("%1 requested a list for %2 without validating").arg(_clientLabel, receiveData.value("gamename", "")) );
            this->disconnect();
            return;
        }

        // get list from db and send it
        QByteArray writeBuffer = compileServerlist(
                                        receiveData.value("gamename", ""),
                                        _coreObject->Settings.ListenServerSettings.serverttl_s,
                                        (receiveData.value("list","").compare("cmp", Qt::CaseInsensitive) == 0 ));
        _tcpSocket->write(writeBuffer);
        _coreObject->Log.logEvent("list", QStringLiteral("%1 received the list for %2").arg(_clientLabel, receiveData.value("gamename", "")));

        // all done
        this->disconnect();

    } // list

    // part 2b: 333networks synchronisation
    if ( receiveData.contains("sync") )
    {
        // list request is only granted after validation
        if ( ! _hasValidated )
        {
            // kick client after unsuccessful validation
            _coreObject->Log.logEvent("secure", QStringLiteral("%1 requested to sync for %2 without validating").arg(_clientLabel, receiveData.value("gamename", "")) );
            this->disconnect();
            return;
        }

        // do not sync with ourself
        QString remoteIdentity = receiveData.value("msid", "");
        if ( _coreObject->masterserverIdentity == remoteIdentity )
        {
            // msid match -- skip syncing
            _tcpSocket->write("\\final\\");
            _coreObject->Log.logEvent("list", QStringLiteral("skipping sync for self at %1").arg(_clientLabel) );
            this->disconnect();
            return;
        }

        // sync request has options "all" or "gamename_1 gamename_2 gamename_n"
        QStringList gamenameList;
        if ( receiveData.value("sync","").toLower().compare("all") == 0 )
        {
            // get list of gamenames from database
            gamenameList = getGamenames(_coreObject->Settings.ListenServerSettings.serverttl_s);
        }
        else
        {
            // only load gamenames that are in our list of available games
            QStringListIterator rawListNames( receiveData.value("sync","").toLower().split(" ") );
            while ( rawListNames.hasNext() )
            {
                QString rawGamename = overrideGamename( rawListNames.next() );
                if ( _coreObject->SupportedGames.contains(rawGamename) )
                {
                    gamenameList.append(rawGamename);
                }
            }
        }

        // get compiled sync list for all requested gamenames
        QByteArray writeBuffer = compileSyncList(
                                    gamenameList,
                                    _coreObject->Settings.ListenServerSettings.serverttl_s);
        _tcpSocket->write(writeBuffer);

        // log (displays all the gamenames that were synced or empty when no gamenames/servers were available)
        _coreObject->Log.logEvent("list", QStringLiteral("%1 received the sync list for %2").arg(_clientLabel, gamenameList.join(",")) );

        // all done
        this->disconnect();

    } // sync

    // optional: echo
    if ( receiveData.contains("echo") )
    {
        _coreObject->Log.logEvent("echo", QStringLiteral("%1 echoed %2").arg(_clientLabel, receiveData.value("echo", "")) );
        _tcpSocket->write("\\echo_reply\\" + receiveData.value("echo", "").toLatin1() );
    }

    // else

    // unknown, log as error
    if ( ! receiveData.contains("validate") and
         ! receiveData.contains("list") and
         ! receiveData.contains("sync") and
         ! receiveData.contains("echo") )
    {
        _coreObject->Log.logEvent("unknown", QStringLiteral("%1 with unknown request %2").arg(_clientLabel, _rxBuffer ) );
        this->disconnect();
    }


}
