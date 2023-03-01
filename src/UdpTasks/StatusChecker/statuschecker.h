#ifndef STATUSCHECKER_H
#define STATUSCHECKER_H

#include <QTimer>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include "Core/CoreObject/coreobject.h"
#include "Database/Common/commonactions.h"
#include "Protocols/GameSpy0/gamespy0.h"
#include "Protocols/GameSpy0/securevalidate.h"
#include "UdpTasks/udpdatastructure.h"

class StatusChecker: public QObject
{
    Q_OBJECT
public:
    StatusChecker(const QSharedPointer<CoreObject> &coreObject);

    // activate server checker/ticker
    bool startTicker();

private:
    QSharedPointer<CoreObject> _coreObject;

    // udp socket
    QUdpSocket _udpSocket;
    QString    _udpRequest = "\\info\\";

    // ticker and reset timers
    QTimer _tickTicker;
    QTimer _resetTimer;

    // ticker helpers
    int _dbIndex = -1;

    // data buffer for received data.
    QHash<QString, QString> _dataBuffer;

    // secure/validate challenge buffer
    QHash<QString, UdpData> _secureBuffer;

    // temporary fix for readyRead signal not being emitted
    int _missedReadCalls = 0;

    // (defined) database fields to be updated in updateServerInfo(...)
    const QStringList _updateFields
    {
        "hostname", "gamever", "minnetver", "location", "listenserver",
        "adminname", "adminemail", "password", "gametype", "gamestyle",
        "changelevels", "maptitle", "mapname", "numplayers", "maxplayers",
        "minplayers", "botskill", "balanceteams", "playersbalanceteams",
        "friendlyfire", "maxteams", "timelimit", "goalteamscore", "fraglimit",
        "misc", "hostport"
    };

private slots:
    // udp- and timer events
    void onUdpResponseRead();
    void onTicker();            // udp query to remote gameserver

private:
    // get next server from database
    QSqlQuery getNextServer (const int &currentServerId);

    // server info to database
    bool insertServerInfo(const QString                 &serverAddress,
                          const unsigned short          &serverPort);

    bool updateServerInfo(const QString                 &serverAddress,
                          const unsigned short          &serverPort,
                          const QHash<QString, QString> &serverInfo);

    bool insertPlayerInfo(const QString                 &serverAddress,
                          const unsigned short          &serverPort,
                          const QHash<QString, QString> &serverInfo);
};

#endif // STATUSCHECKER_H
