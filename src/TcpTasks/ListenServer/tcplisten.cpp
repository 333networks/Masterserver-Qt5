#include "listenserver.h"

bool ListenServer::listen()
{
    // connect socket
    connect(&_tcpServer, &QTcpServer::newConnection, this, &ListenServer::onListenConnection);

    // start listening for new TCP connections
    bool error = _tcpServer.listen(QHostAddress::Any, _coreObject->Settings.ListenServerSettings.listenPort);

    // error notification:
    if ( ! error )
    {
        // complete startup
        _coreObject->Log.logEvent("fatal", QStringLiteral("error starting tcp Listen server: %1")
                                  .arg(_tcpServer.errorString()));
        return false;
    }

    // complete startup
    _coreObject->Log.logEvent("info", QStringLiteral("start listening for TCP beacons on port %1")
                            .arg(_coreObject->Settings.ListenServerSettings.listenPort));

    return true;
}
