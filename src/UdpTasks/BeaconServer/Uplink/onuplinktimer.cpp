#include "../beaconserver.h"

void BeaconServer::onUplinkTimer()
{
    // get uplinks from settings
    QListIterator<SyncServer> syncServers(_coreObject->Settings.SyncerSettings.syncServers);
    while ( syncServers.hasNext() )
    {
        // get next item
        SyncServer     thisUplink     = syncServers.next();
        QString        remoteHostname = thisUplink.remoteAddress;
        unsigned short remotePort     = thisUplink.beaconPort;

        // resolve (async) uplink address and let callback perform the uplink
        QHostInfo::lookupHost(remoteHostname, this, [this, remoteHostname, remotePort] (const QHostInfo &host)
        {
            // errors during lookup?
            if ( host.error() == QHostInfo::NoError and ! host.addresses().empty() )
            {
                // create and send heartbeat
                QNetworkDatagram udpDatagram( _uplinkData.toLatin1(), host.addresses().first(), remotePort );
                _udpSocket.writeDatagram( udpDatagram );

                // add to log
                _coreObject->Log.logEvent("uplink", QStringLiteral("sending uplink to %1 (%2:%3)")
                                          .arg(remoteHostname,
                                               host.addresses().first().toString(),
                                               QString::number(remotePort)));

                // function this->onUdpRead() will handle any responses (secure/basic/status)
            }
            else
            {
                // log failure to resolve
                _coreObject->Log.logEvent("uplink", QStringLiteral("cannot resolve %1: %2")
                                          .arg( remoteHostname, host.errorString()));
            }
        }); // end QHostInfo

    } // end while
}
