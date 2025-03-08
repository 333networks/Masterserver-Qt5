#ifndef LISTENCLIENTHANDLER_H
#define LISTENCLIENTHANDLER_H

#include <QTimer>
#include <QTcpSocket>
#include <QHostAddress>

#include "Core/CoreObject/coreobject.h"
#include "Database/Common/commonactions.h"

#include "Protocols/GameSpy0/gamespy0.h"
#include "protocols/enctype2.h"
#include "Protocols/GameSpy0/securevalidate.h"

class ListenClientHandler : public QObject
{
    Q_OBJECT
public:
    ListenClientHandler(const QSharedPointer<CoreObject> &coreObject,
                        QTcpSocket *tcpSocket);

private:
    const int _timeOutTime_ms = 7500;

    QSharedPointer<CoreObject> _coreObject;
    QScopedPointer<QTcpSocket>  _tcpSocket;
    QTimer                      _timeOut;
    QString                     _rxBuffer = "";
    bool                        _hasValidated = false;
    bool                        _requestEnctype2 = false;

    QString _clientLabel;
    QString _secure;
private:
    void disconnect();

    // database functions
    QByteArray compileServerlist (const QString &gamename,
                                  const int     &serverAge_s,
                                  const bool    &cmp);
    QByteArray compileSyncList(const QStringList &gamenameList,
                               const int         &serverAge_s);

private slots:
    void onListenClientRead();
    void onListenClientTimeOut();
    void onListenClientDisconnect();

};

#endif // LISTENCLIENTHANDLER_H
