#include "listenserver.h"

void ListenServer::onListenConnection()
{
    // handle client in its own class
    new ListenClientHandler(_coreObject, _tcpServer.nextPendingConnection());
}
