#include "listenclienthandler.h"

void ListenClientHandler::disconnect()
{
    _timeOut.stop();
    _tcpSocket->disconnectFromHost();
}

void ListenClientHandler::onListenClientDisconnect()
{
    _timeOut.stop();
    _coreObject->Log.logEvent("tcp", QStringLiteral("%1 disconnected").arg(_clientLabel) );
    this->deleteLater();
}

void ListenClientHandler::onListenClientTimeOut()
{
    _timeOut.stop();
    _coreObject->Log.logEvent("tcp", QStringLiteral("%1 timed out").arg(_clientLabel) );
    this->disconnect();
}
