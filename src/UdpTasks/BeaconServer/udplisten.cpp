#include "beaconserver.h"

bool BeaconServer::listen()
{
    // connect socket, timer with events
    connect(&_udpSocket, &QUdpSocket::readyRead, this, &BeaconServer::onUdpRead);
    connect(&_sweepTimer, &QTimer::timeout, this, &BeaconServer::onUdpTimedOut);

    // bind socket and timeout
    _udpSocket.bind(QHostAddress::Any, _coreObject->Settings.BeaconServerSettings.beaconPort);
    _sweepTimer.start( _timeOutTime_ms );

    // complete startup
    _coreObject->Log.logEvent("info", QStringLiteral("start listening for UDP beacons on port %1")
                            .arg(_coreObject->Settings.BeaconServerSettings.beaconPort));

    return true;
}
