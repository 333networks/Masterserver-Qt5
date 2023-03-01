#include "maintenance.h"

bool Maintenance::scheduleMaintenance()
{
    // set update timer
    connect(&_maintenanceTimer, &QTimer::timeout, this, &Maintenance::onMaintenanceTimerAction);

    _maintenanceTimer.setInterval( _coreObject->Settings.MaintenanceSettings.timeMaintenanceInterval_s * 1000);
    _maintenanceTimer.start();

    // complete startup
    _coreObject->Log.logEvent("info", QStringLiteral("performing maintenance every %1 seconds")
                            .arg(_coreObject->Settings.MaintenanceSettings.timeMaintenanceInterval_s));

    return true;
}
