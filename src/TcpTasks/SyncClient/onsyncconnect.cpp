#include "syncclient.h"

void SyncClient::onSyncConnect()
{
    // reset timeout
    _timeOut.start();

    // log
    _coreObject->Log.logEvent("tcp", QStringLiteral("connected to %1").arg(_clientLabel) );
}
