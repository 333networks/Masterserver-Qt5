#include "syncclient.h"

void SyncClient::onSyncDisconnect()
{
    // remote host closed the connection (typically occurs after receiving the list from remote host)
    if (_tcpSocket.error() == QAbstractSocket::RemoteHostClosedError )
    {
        _coreObject->Log.logEvent("tcp", QStringLiteral("disconnected from %1").arg(_clientLabel) );
    }

    // timer already stopped or a timeout occurred
    if ( ! _timeOut.isActive() or _tcpSocket.error() == QAbstractSocket::SocketTimeoutError)
    {
        _coreObject->Log.logEvent("warning", QStringLiteral("timeout while attempting to sync with %1 (1)").arg(_clientLabel));
    }

    // an error occured and is reported, excluding...
    if ( _tcpSocket.error() != QAbstractSocket::RemoteHostClosedError and   // ...regular disconnect caught above
         _tcpSocket.error() != QAbstractSocket::SocketTimeoutError and      // ...timeout caught above
         _tcpSocket.error() != QAbstractSocket::UnknownSocketError )        // ...QTimer timeout does not leave an errorcode (defaults to unknown error)
    {
        _coreObject->Log.logEvent("warning", QStringLiteral("error while syncing with %1: %2").arg(_clientLabel, _tcpSocket.errorString()));
    }

    // stop timer if necessary and delete this client
    _timeOut.stop();
    this->deleteLater();
}

void SyncClient::onSyncTimeOut()
{
    // if no error was specified while timer expired, there was a connection timeout
    if ( _tcpSocket.error() == QAbstractSocket::UnknownSocketError )
    {
        _coreObject->Log.logEvent("warning", QStringLiteral("timeout while attempting to sync with %1 (2)").arg(_clientLabel));
    }

    // other errors, like establishing connection/refused
    if ( _tcpSocket.error() != QAbstractSocket::UnknownSocketError )
    {
        _coreObject->Log.logEvent("warning", QStringLiteral("error while syncing with %1: %2").arg(_clientLabel, _tcpSocket.errorString()));
    }

    // stop timer and close socket
    _timeOut.stop();
    _coreObject->Log.logEvent("tcp", QStringLiteral("%1 scheduled for deletion").arg(_clientLabel) );
    _tcpSocket.disconnectFromHost();
    this->deleteLater();
}
