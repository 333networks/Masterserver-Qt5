#ifndef SERVERINFOSTRUCTURE_H
#define SERVERINFOSTRUCTURE_H

#include <QDateTime>
#include <QHostAddress>

struct ServerInfo
{
    // server address
    QHostAddress ip;

    // server port
    unsigned short port = 0;

    // gamename
    QString gamename = "";

    // date that the serverinfo was added or last updated
    qint64 time = QDateTime::currentSecsSinceEpoch();
};

// compare operator
inline bool operator== (const ServerInfo serverInfo1, const ServerInfo serverInfo2)
{
    // compare address, port and gamename. ignore time.
    return ( serverInfo1.ip.isEqual(serverInfo2.ip) and
             serverInfo1.port == serverInfo2.port and
             serverInfo1.gamename == serverInfo2.gamename );
}

#endif // SERVERINFOSTRUCTURE_H
