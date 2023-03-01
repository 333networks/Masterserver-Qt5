#include "statuschecker.h"

bool StatusChecker::startTicker()
{

    /* UDP Query Checking
     *
     * Send a \info\ or \status\ query to the remote gameserver. The
     * server will respond with its serverdata, which is processed
     * and stored in the database. Required for display on the website.
     */

    // check type (extended or simple)
    _udpRequest = _coreObject->Settings.CheckerSettings.getExtendedInfo ? "\\status\\" : "\\info\\";

    // set udp datagram handler
    connect(&_udpSocket, &QUdpSocket::readyRead, this, &StatusChecker::onUdpResponseRead);

    // set UDP+1 port for outgoing status checking (avoid querying from random port)
    _udpSocket.bind(QHostAddress::Any, _coreObject->Settings.BeaconServerSettings.beaconPort+1);

    // attach timer function to tick through servers (from db)
    connect(&_tickTicker, &QTimer::timeout, this, &StatusChecker::onTicker);

    // set reset timer and event ("cycle time")
    connect(&_resetTimer, &QTimer::timeout, [this]
    {
        // reset indices and clear udp cache
        _dbIndex = -1;
        _dataBuffer.clear();
        _secureBuffer.clear();
    });
    _resetTimer.setInterval( _coreObject->Settings.CheckerSettings.timeCheckerReset_s * 1000);
    _resetTimer.start();

    // ticker enable (keeps running, also when there are no new servers to be queried-- keeps firing every 1/serversPerSecond)
    _tickTicker.setInterval( _coreObject->Settings.CheckerSettings.timeServerInterval_ms );
    _tickTicker.start();

    // announce servers per second (calculate from interval)
    double serversPerSecond = (1000.0 / _coreObject->Settings.CheckerSettings.timeServerInterval_ms);
    _coreObject->Log.logEvent("info", QStringLiteral("server status every %1 seconds (%2 servers/s)")
                            .arg( QString::number(_coreObject->Settings.CheckerSettings.timeCheckerReset_s),
                                  QString::number(serversPerSecond, 'f', 1) ) );

    return true;
}
