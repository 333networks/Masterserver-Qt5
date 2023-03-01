#include "listenserver.h"

ListenServer::ListenServer(const QSharedPointer<CoreObject> &coreObject)
{
    // create local access
    this->_coreObject = coreObject;
}
