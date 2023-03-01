#ifndef UDPDATASTRUCTURE_H
#define UDPDATASTRUCTURE_H

#include <QHostAddress>
#include <QDateTime>

// heartbeat struct for incoming udp heartbeats
struct UdpData
{
    // update time or creation time
    qint64 time = QDateTime::currentSecsSinceEpoch();

    // address information
    QString         ip   = "";
    unsigned short  port = 0;

    // gamename (beacons)
    QString gamename = "";

    // secure / validate challenge data
    QString secure = "";

    // raw data from socket
    QString rawData = "";

    // parsed data as key->value
    QMultiHash<QString, QString> serverData;
};

#endif // UDPDATASTRUCTURE_H
