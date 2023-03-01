#include "core.h"

void Core::run()
{
    // randomize
    qsrand(static_cast<unsigned int>(QDateTime::currentMSecsSinceEpoch()));

    // announce startup
    logPrimitive() << "*** Starting 333networks Master Server v" << BUILD_VERSION << " ***" << endl;

    // can not set file paths
    if (_applicationPath.length() <= 0)
        this->shutdown();

    // debug info (hardcoded disable for releases)
    if ( false )
        logPrimitive() << "Using Qt " << qVersion() << endl;

    // set our own 12-byte identifier
    _coreObject->masterserverIdentity = genChallengeString(12, true);

    // load config settings from file
    _coreObject->Settings = loadSettings(_applicationPath);
    if ( ! _coreObject->Settings.init )
        this->shutdown();

    // initialise database
    if ( ! initDatabase(_applicationPath) )
        this->shutdown();

    // load game info from file and into database
    _coreObject->SupportedGames = loadSupportedGames(_applicationPath);
    if ( _coreObject->SupportedGames.count() <= 0 )
        this->shutdown();

    // logger init
    if ( ! _coreObject->Log.init(_applicationPath, _coreObject->Settings) )
        this->shutdown();

    /*
     * enter runmode
     */

    // udp beacon server
    if ( ! _udpBeaconServer->listen() )
        this->shutdown();

    // tcp listen server
    if ( ! _tcpListenServer->listen() )
        this->shutdown();

    /*
     * advanced functionality
     */

    // maintenance and statistics
    if ( _coreObject->Settings.MaintenanceSettings.doMaintenance )
        _maintenance->scheduleMaintenance();

    // udp uplink broadcast every X minutes
    if ( _coreObject->Settings.BeaconServerSettings.doUplink )
        _udpBeaconServer->uplink();

    // syncing with other masterservers
    if ( _coreObject->Settings.SyncerSettings.doSync )
        _syncUpdater->scheduleUpdater();

    // server checker
    if ( _coreObject->Settings.CheckerSettings.doCheck )
        _statusChecker->startTicker();

    /*
     * all services running
     */
}
