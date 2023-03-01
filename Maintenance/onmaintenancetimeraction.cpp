#include "maintenance.h"

// perform maintenance actions
void Maintenance::onMaintenanceTimerAction()
{
    // announce
    _coreObject->Log.logEvent("main", QStringLiteral("performing maintenance"));

    // update statistics
    int numUpdated = updateStats();
    if (numUpdated > 0)
        _coreObject->Log.logEvent("stat", QStringLiteral("updated %1 game stats").arg(QString::number(numUpdated)));

    // prune direct beacons
    int numPrunedBeacons = pruneBeacons();
    if (numPrunedBeacons > 0)
        _coreObject->Log.logEvent("prune", QStringLiteral("pruned %1 direct beacons").arg(QString::number(numPrunedBeacons)));

    // prune serverinfo, serverinfo from which the serverlist entry is gone already
    int numPrunedInfo = pruneServerInfo();
    if (numPrunedInfo > 0)
        _coreObject->Log.logEvent("prune", QStringLiteral("pruned %1 server info entries").arg(QString::number(numPrunedInfo)));

    // prune orphaned players from which the serverinfo entry is gone already OR updateinfo is outdated
    int numPrunedPlayers = prunePlayers();
    if (numPrunedPlayers > 0)
        _coreObject->Log.logEvent("prune", QStringLiteral("pruned %1 players").arg(QString::number(numPrunedPlayers)));
}
