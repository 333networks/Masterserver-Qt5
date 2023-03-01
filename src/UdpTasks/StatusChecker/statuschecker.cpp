#include "statuschecker.h"

StatusChecker::StatusChecker(const QSharedPointer<CoreObject> &coreObject)
{
    // create local access
    this->_coreObject = coreObject;
}
