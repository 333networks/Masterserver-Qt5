#ifndef LISTENSERVER_H
#define LISTENSERVER_H

#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>

#include "Core/CoreObject/coreobject.h"
#include "TcpTasks/ListenClientHandler/listenclienthandler.h"

class ListenServer: public QObject
{
    Q_OBJECT
public:
    ListenServer(const QSharedPointer<CoreObject> &coreObject);
    bool listen();

private:
    QSharedPointer<CoreObject> _coreObject;

    // tcp server socket
    QTcpServer  _tcpServer;

signals:

private slots:
    void onListenConnection();

};

#endif // LISTENSERVER_H
