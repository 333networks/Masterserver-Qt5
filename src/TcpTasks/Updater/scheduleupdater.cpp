#include "syncupdater.h"

bool SyncUpdater::scheduleUpdater()
{
    // schedule sync X seconds apart (prevent network/cpu spikes)
    connect(&_syncTicker, &QTimer::timeout, this, &SyncUpdater::onSyncTickerAction);
    _syncTicker.setInterval( _graceTime_ms );

    // set update timer
    connect(&_updaterTimer, &QTimer::timeout, [this]
    {
        // reset and start ticker
        _syncIndex = 0;
        _syncTicker.start();
    });

    _updaterTimer.setInterval( _coreObject->Settings.SyncerSettings.syncInterval_s * 1000);
    _updaterTimer.start();

    // complete startup
    _coreObject->Log.logEvent("info", QStringLiteral("sync with other masterservers every %1 seconds")
                            .arg(_coreObject->Settings.SyncerSettings.syncInterval_s));

    // run immediately at startup
    _syncIndex = 0;
    _syncTicker.start();

    return true;
}
