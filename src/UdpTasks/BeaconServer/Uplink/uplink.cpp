#include "../beaconserver.h"

bool BeaconServer::uplink()
{
    // set uplink message (port and gamename)
    _uplinkData = QStringLiteral("\\heartbeat\\%1\\gamename\\%2")
            .arg( QString::number( _coreObject->Settings.BeaconServerSettings.beaconPort),
                  TYPE_GAMENAME );

    // connect timer with events
    connect(&_uplinkTimer, &QTimer::timeout, this, &BeaconServer::onUplinkTimer);

    // start timer
    _uplinkTimer.start( _broadcastInterval_s * 1000 );

    // complete startup
    _coreObject->Log.logEvent("info", QStringLiteral("broadcasting UDP uplinks every %1 seconds")
                            .arg( _broadcastInterval_s ));

    return true;
}
