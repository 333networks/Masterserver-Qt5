#include "syncupdater.h"

SyncUpdater::SyncUpdater(const QSharedPointer<CoreObject> &coreObject)
{
    // create local access
    this->_coreObject = coreObject;
}
