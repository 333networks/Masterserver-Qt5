#include "statuschecker.h"

void StatusChecker::onTicker()
{

    // get next recent item from database (id > index)
    QSqlQuery q = getNextServer(_dbIndex);

    if ( q.next() )
    {
        // get serverinfo from query
        _dbIndex = q.value("id").toInt();
        QString        remoteAddress = q.value("ip").toString();
        unsigned short remotePort    = static_cast<unsigned short>(q.value("queryport").toInt());
        QString        gamename      = q.value("gamename").toString();
        bool           authenticated = static_cast<unsigned short>(q.value("f_auth").toBool());

        // if this particular server was added through SYNC, it may not yet have been authenticated.
        // send secure/validate challenge in that case
        if ( ! authenticated )
        {
            // shorthand label
            QString remoteAddressLabel = QStringLiteral("%1:%2").arg(remoteAddress, QString::number(remotePort));
            QString secure = genChallengeString(6, false);

            // use HeartBeat struct to cache gamename+secure
            // TODO optimise the HeartBeat struct in BeaconServer to cater both purposes
            UdpData secureInfo;
            secureInfo.secure   = secure;
            secureInfo.gamename = gamename;
            _secureBuffer.insert(remoteAddressLabel, secureInfo);

            // send secure challenge
            QString udpSecure = QStringLiteral("\\secure\\%1").arg(secure);
            QNetworkDatagram udpSecureDatagram(udpSecure.toUtf8(), QHostAddress(remoteAddress), remotePort);
            _udpSocket.writeDatagram(udpSecureDatagram);
        }

        // create datagram with info- or status request
        QNetworkDatagram udpDatagram(_udpRequest.toUtf8(), QHostAddress(remoteAddress), remotePort);

        // and send
        _udpSocket.writeDatagram(udpDatagram);

        // readyRead miss workaround
        if ( _udpSocket.hasPendingDatagrams() )
        {
            /* WORKAROUND INFO
             * _udpSocket.bytesAvailable() > 0 and _udpSocket.hasPendingDatagrams(),
             * but apparently no readyRead signal was emitted. As a result, incoming data
             * is no longer processed. Temporary fix until the cause has been found:
             * emit readyRead manually when datagrams are available. It is still possible
             * that the ticker is faster than the readyRead-bound function and that readyRead
             * was issued correctly, so we count two ticks of missed read calls. If the
             * readyRead signal was correctly issued, the onUdpResponseRead function will
             * reset the missed readcall counter. Even when botched, it is better to emit the
             * same signal twice, rather than not at all (onUdpResponseRead has a failsaife
             * for duplicate/redundant calls).
             *
             * Other info:
             * is this related to https://www.qtcentre.org/threads/64370-QUdpSocket-readyRead-failure ?
             *
             * Displaying readyread state and available data:
            qDebug() << "bytes available: "
                     << _udpSocket.bytesAvailable()
                     << "pending datagrams: "
                     << _udpSocket.hasPendingDatagrams()
                     ;
             */
            _missedReadCalls++;

            // two missed calls in 2 ticks, emit signal manually
            if ( _missedReadCalls >= 2)
            {
                _coreObject->Log.logEvent("warning", "checker udp data not signalled: emitting readyread manually");
                emit _udpSocket.readyRead();
            }
        }  // end workaround

    } // end next
}
