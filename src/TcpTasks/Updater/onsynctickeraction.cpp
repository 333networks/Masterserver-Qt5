#include "syncupdater.h"

void SyncUpdater::onSyncTickerAction()
{
    // exists?
    if ( _syncIndex < _coreObject->Settings.SyncerSettings.syncServers.length() )
    {
        // retrieve sync settings from config
        SyncServer syncServ = _coreObject->Settings.SyncerSettings.syncServers.at(_syncIndex);
        _syncIndex++;

        // execute sync with remote masterserver
        new SyncClient(_coreObject, syncServ.remoteAddress, syncServ.listenPort);
    }
    else
    {
        // last entry. turn off timer
        _syncTicker.stop();
    }
}
