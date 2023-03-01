#include "maintenance.h"

Maintenance::Maintenance(const QSharedPointer<CoreObject> &coreObject)
{
    // create local access
    this->_coreObject = coreObject;
}
