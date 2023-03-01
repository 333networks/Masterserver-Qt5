#include "syncclient.h"

SyncClient::SyncClient(const QSharedPointer<CoreObject> &coreObject,
                       const QString                    &remoteHost,
                       const unsigned short int         &remotePort)
{
    // create local access
    this->_coreObject = coreObject;

    // connect events and functions
    connect(&_tcpSocket, &QTcpSocket::connected,    this, &SyncClient::onSyncConnect);
    connect(&_tcpSocket, &QTcpSocket::readyRead,    this, &SyncClient::onSyncRead);
    connect(&_tcpSocket, &QTcpSocket::disconnected, this, &SyncClient::onSyncDisconnect);
    connect(&_timeOut,   &QTimer::timeout,          this, &SyncClient::onSyncTimeOut);

    // convenience label to found
    _clientLabel = QStringLiteral("%1:%2")
            .arg(remoteHost, QString::number(remotePort));

    // set timeout
    _timeOut.setInterval( _timeOutTime_ms );
    _timeOut.start(); // connection timout time === read timeout time

    // connect to remote masterserver
    _tcpSocket.connectToHost(remoteHost, remotePort);
    _coreObject->Log.logEvent("tcp", QStringLiteral("connecting to %1").arg(_clientLabel));
}
