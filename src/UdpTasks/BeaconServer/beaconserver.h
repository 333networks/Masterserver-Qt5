#ifndef BEACONSERVER_H
#define BEACONSERVER_H

#include <QTimer>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QHostInfo>
#include "Core/CoreObject/coreobject.h"
#include "Database/Common/commonactions.h"
#include "Protocols/GameSpy0/gamespy0.h"
#include "Protocols/GameSpy0/securevalidate.h"
#include "UdpTasks/udpdatastructure.h"

class BeaconServer : public QObject
{
    Q_OBJECT
public:
    BeaconServer(const QSharedPointer<CoreObject> &coreObject);

    // activate listener and broadcast
    bool listen();
    bool uplink();

private: // general udp task handles
    QSharedPointer<CoreObject> _coreObject;
    const int _timeOutTime_ms      = 15000; // 15 second soft timeout
    const int _broadcastInterval_s = 60;   //   1 min between beacons

    // udp socket
    QUdpSocket _udpSocket;

    // determine reply to incoming requests
    QStringList replyQuery(const QMultiHash<QString, QString> &query);

private: // udp beacon server

    // heartbeat processing for different protocol types
    void processHeartbeatGamespy0(const QNetworkDatagram &datagram,
                                  const QString          &senderAddress,
                                  const unsigned short   &senderPort,
                                  const QString          &receiveBuffer);

    // timer to sweep up abandoned beacons (timeouts)
    QTimer _sweepTimer;

    // store information about unverified beacons
    QHash<QString, UdpData> _beaconList;

    // helper for replyQuery()
    int _queryId;

private slots: // udp beacon server event slots
    void onUdpRead();
    void onUdpTimedOut();

private: // broadcast heartbeat

    // outbound heartbeat timer and content
    QTimer  _uplinkTimer;
    QString _uplinkData;

private slots: // broadcast heartbeat events
    void onUplinkTimer();
};

#endif // BEACONSERVER_H
