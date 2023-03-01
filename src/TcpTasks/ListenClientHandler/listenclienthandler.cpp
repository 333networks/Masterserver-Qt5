#include "listenclienthandler.h"

ListenClientHandler::ListenClientHandler(const QSharedPointer<CoreObject> &coreObject,
                                         QTcpSocket *tcpSocket)
    : _tcpSocket(tcpSocket)
{
    // create local access
    this->_coreObject = coreObject;

    // connect read, timeout and disconnect events
    connect(tcpSocket, &QTcpSocket::readyRead,    this, &ListenClientHandler::onListenClientRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ListenClientHandler::onListenClientDisconnect);
    connect(&_timeOut, &QTimer::timeout,          this, &ListenClientHandler::onListenClientTimeOut);

    // timeout
    _timeOut.setInterval( _timeOutTime_ms );
    _timeOut.start();

    // challenge
    _clientLabel = QStringLiteral("%1:%2").arg(tcpSocket->peerAddress().toString(), QString::number(tcpSocket->peerPort()));
    _secure      = genChallengeString(6, false);

    _tcpSocket->write( QStringLiteral("\\basic\\\\secure\\%1").arg(_secure).toUtf8() );
    _tcpSocket->flush();

    // log new connections
    _coreObject->Log.logEvent("tcp", QStringLiteral("%1 connected").arg(_clientLabel) );
}
