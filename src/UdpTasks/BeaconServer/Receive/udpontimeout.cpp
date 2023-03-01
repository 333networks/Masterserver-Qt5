#include "../beaconserver.h"

// soft timeout. every <timeout>, remove all servers older than <timeout> from the list.
// that means a server has a maximum wait/response time of 2*<timeout>
void BeaconServer::onUdpTimedOut()
{
    // iterate through the server list
    QHashIterator<QString, UdpData> list(_beaconList);
    while (list.hasNext())
    {
        // select
        list.next();

        // check passed time: add date < remove date?
        qint64 currentTime = QDateTime::currentSecsSinceEpoch();
        if ( list.value().time < currentTime - (_timeOutTime_ms / 1000) )
        {
            // if timeout has passed, remove the server from the list
            _coreObject->Log.logEvent("udp", QStringLiteral("%1 timed out").arg(list.key()));
            _beaconList.remove(list.key());
        }
    }

    // periodically emit readyread signal to avoid issues similar to StatusChecker
    // readyread function will handle any inconsistencies
    emit _udpSocket.readyRead();
}
