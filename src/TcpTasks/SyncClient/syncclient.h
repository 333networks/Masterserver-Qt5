#ifndef SYNCCLIENT_H
#define SYNCCLIENT_H

#include <QTimer>
#include <QTcpSocket>
#include <QHostAddress>

#include "Core/CoreObject/coreobject.h"
#include "Database/Common/commonactions.h"
#include "Protocols/GameSpy0/gamespy0.h"
#include "Protocols/GameSpy0/securevalidate.h"

class SyncClient: public QObject
{
    Q_OBJECT
public:
    SyncClient(const QSharedPointer<CoreObject> &coreObject,
               const QString                    &remoteHost,
               const unsigned short int         &remotePort);

private:
    QSharedPointer<CoreObject> _coreObject;
    const int _timeOutTime_ms = 7500;

    // tcp client handles
    QTcpSocket _tcpSocket;
    QTimer     _timeOut;
    QString    _rxBuffer = "";
    QString    _clientLabel;

    // helpers
    int _queryId = 0;

    // functions
    QStringList replyQuery(const QMultiHash<QString, QString> &query);

private: // update sync time in database
    bool updateSyncedServer(const QString        &serverAddress,
                            const unsigned short &serverPort);

private slots:
    void onSyncConnect();
    void onSyncRead();
    void onSyncDisconnect();
    void onSyncTimeOut();

signals:
};

#endif // SYNCCLIENT_H
