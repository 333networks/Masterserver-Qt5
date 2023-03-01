#include "beaconserver.h"

BeaconServer::BeaconServer(const QSharedPointer<CoreObject> &coreObject)
{
    // create local access
    this->_coreObject = coreObject;
}
