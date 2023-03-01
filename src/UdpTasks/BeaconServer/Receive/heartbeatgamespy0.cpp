#include "../beaconserver.h"

// heartbeat processing for different protocol types
void BeaconServer::processHeartbeatGamespy0(const QNetworkDatagram &datagram,
                                            const QString          &senderAddress,
                                            const unsigned short   &senderPort,
                                            const QString          &receiveBuffer)
{
    // parse key/value pairs and create a readable server label
    QMultiHash<QString, QString> receiveData = parseGameSpy0Buffer(receiveBuffer);
    QString senderAddressLabel = QStringLiteral("%1:%2").arg(senderAddress, QString::number(senderPort));

    /*
     * Receive a heartbeat.
     * Update or insert the server in the database. If not authenticated, send a secure/validate challenge.
     */
    if ( receiveData.contains("heartbeat") )
    {
        // store heartbeat and request authentication
        UdpData newBeacon;
        newBeacon.ip        = senderAddress;
        newBeacon.port      = receiveData.value("heartbeat", "0").toUShort();
        newBeacon.gamename  = receiveData.value("gamename", "unknown");

        // sanity check: known game(name)
        if ( _coreObject->SupportedGames.contains( newBeacon.gamename ) )
        {
            // valid port and/or default port available?
            if (newBeacon.port == 0)
            {
                // override with default port if possible
                if ( _coreObject->SupportedGames.value(newBeacon.gamename).port > 0 )
                {
                    newBeacon.port = _coreObject->SupportedGames.value( newBeacon.gamename ).port;
                }
                else // no valid port available. log and abort.
                {
                    _coreObject->Log.logEvent("heartbeat", QStringLiteral("%1 invalid port for %2")
                                   .arg(newBeacon.ip, newBeacon.gamename) );
                    return;
                }
            }
        }
        else // unknown game. log and abort.
        {
            _coreObject->Log.logEvent("unsupported", QStringLiteral("%1:%2 for unsupported game %3")
                           .arg(newBeacon.ip, QString::number(newBeacon.port), newBeacon.gamename) );
            return;
        }


        // if this server already exists, update it
        if ( updateServer(newBeacon.ip, newBeacon.port, newBeacon.gamename, true, false) )
        {
            // log update
            _coreObject->Log.logEvent("heartbeat", QStringLiteral("%1:%2 for %3")
                    .arg(newBeacon.ip, QString::number(newBeacon.port), newBeacon.gamename) );

            // no further tasks for this datagram
            return;
        }

        // else:

        // add to database
        insertServer(newBeacon.ip, newBeacon.port, newBeacon.gamename, true);

        // log type "uplink" for first heartbeat. from now on, this server is logged with type "heartbeat"
        _coreObject->Log.logEvent("new", QStringLiteral("%1:%2 for %3")
                       .arg(newBeacon.ip, QString::number(newBeacon.port),newBeacon.gamename) );

        /*
         * Set up secure/validate challenge
         */

        // some games are incompatible with secure/validate (within this protocol)
        if ( _overrideValidateBeacon.contains(newBeacon.gamename) )
            return;

        // generate new challenge
        newBeacon.secure = genChallengeString(6, false);

        // store heartbeat in temporary list
        _beaconList.remove(senderAddressLabel); // remove potential old challenges first
        _beaconList.insert(senderAddressLabel, newBeacon);

        // request authentication of remote server
        _udpSocket.writeDatagram( datagram.makeReply( QStringLiteral("\\secure\\%1").arg(newBeacon.secure).toLatin1() ) );

        // no further tasks for this datagram
        return;
    }

    /*
     * received response to authentication request
     */
    if ( receiveData.contains("validate") )
    {
        // load existing information
        UdpData valBeacon = _beaconList.value(senderAddressLabel);

        // empty heartbeat UdpData? then received validate timed out and previous UdpData was removed
        if (QHostAddress(valBeacon.ip).isNull() || valBeacon.gamename.length() <= 0)
        {
            _coreObject->Log.logEvent("secure", QStringLiteral("unexpected validate from %1").arg(senderAddress));
            return;
        }

        // get response
        AuthResult authResult = validateGamename(true, // this is a beacon
                                                 valBeacon.gamename,
                                                 receiveData.value("validate",""),
                                                 _coreObject->SupportedGames.value(valBeacon.gamename).cipher,
                                                 valBeacon.secure,
                                                 receiveData.value("enctype", "0").toInt() );

        // compare with received response
        if ( authResult.auth )
        {
            // server authenticated - log and add to database
            _coreObject->Log.logEvent("secure", QStringLiteral("successful validate from %1:%2 for %3")
                           .arg(valBeacon.ip, QString::number(valBeacon.port),valBeacon.gamename) );

            // update the existing entry that was already added in the initial heartbeat
            updateServer(valBeacon.ip, valBeacon.port, valBeacon.gamename, false, true);

            // remove from temporary/pending list
            _beaconList.remove(senderAddressLabel);
        }
        else // log failed validate
        {
            // set validate false (but update last response time)
            updateServer(valBeacon.ip, valBeacon.port, valBeacon.gamename, false, false);
            _coreObject->Log.logEvent("secure", QStringLiteral("failed validate from %1:%2 for %3")
                           .arg(valBeacon.ip, QString::number(valBeacon.port),valBeacon.gamename) );
            _coreObject->Log.logEvent("secure", QStringLiteral("secure: '%1', gamename: '%2', validate: '%3', expected: '%4'")
                                        .arg(valBeacon.secure, valBeacon.gamename, receiveData.value("validate", "null"), authResult.validate ));
        }

        return;
    }

    /*
     * status queries directed at masterserver
     */
    if (receiveData.contains("secure") or
        receiveData.contains("basic") or
        receiveData.contains("info") or
        receiveData.contains("rules") or
        receiveData.contains("status") or
        receiveData.contains("echo") )
    {
        // parse response query
        QStringList response = replyQuery(receiveData);

        // return response
        _udpSocket.writeDatagram( datagram.makeReply( response.join("").toLatin1() ) );

        // log incoming query
        _coreObject->Log.logEvent("query", QStringLiteral("%1 queried us with %2")
                       .arg(senderAddress, receiveData.keys().join(", ") ) );

        // log echo separately
        if ( receiveData.contains("echo") )
            _coreObject->Log.logEvent("echo", QStringLiteral("%1: '%2'")
                           .arg(senderAddress, receiveData.value("echo") ) );

        return;
    }

    // ignore trailing queryid+final
    if (receiveData.size() > 0)
    {
        // receive queryid and final from the query
        receiveData.remove("queryid");
        receiveData.remove("final");

        // nothing remains? then ignore. otherwise, proceed to "unknown query"
        if ( receiveData.size() <= 0)
            return;
    }

    // received another type of query?
    _coreObject->Log.logEvent("unknown", QStringLiteral("received unknown udp uplink %1 from %2")
                   .arg(receiveBuffer, senderAddress) );
}
