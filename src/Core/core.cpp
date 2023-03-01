#include "core.h"

Core::Core(QString applicationPath)
{
    _applicationPath = applicationPath;
}

// TODO
void Core::shutdown()
{
    logPrimitive() << "[stop] quitting masterserver application" << endl;

    // end application
    exit(0);
}
