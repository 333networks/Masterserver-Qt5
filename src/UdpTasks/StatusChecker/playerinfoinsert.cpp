#include "statuschecker.h"

bool StatusChecker::insertPlayerInfo(const QString                 &serverAddress,
                                     const unsigned short          &serverPort,
                                     const QHash<QString, QString> &serverInfo)
{
    QSqlQuery q;
    int serverID = -1;

    // TODO: combine from 3 to 2 queries?
    // (query optimisation)

    // get server ID first
    QString selectString = "SELECT id FROM serverlist "
                           "WHERE ip = :ip AND queryport = :queryport ";

    q.prepare(selectString);
    q.bindValue(":ip",        serverAddress);
    q.bindValue(":queryport", serverPort);

    if ( ! q.exec() )
        return reportQuery(q);

    if ( q.next() ) // else serverID remains -1
        serverID = q.value(0).toUInt();

    // remove old player entries
    QString deleteString = "DELETE FROM playerinfo WHERE sid = :sid";
    q.prepare(deleteString);
    q.bindValue(":sid", serverID);
    if ( ! q.exec() )
        return reportQuery(q);

    // iterate through serverInfo player data
    int playerIndex = 0;
    while ( serverInfo.contains( QStringLiteral("player_%1").arg(playerIndex) ) )
    {
        QString insertString = "INSERT INTO playerinfo "
                               "(sid, name, team, frags, mesh, skin, face, ping, misc, dt_player) "
                               "VALUES (:sid, :name, :team, :frags, :mesh, :skin, :face, :ping, :misc, :dt_player)";
        q.prepare(insertString);
        q.bindValue(":sid",   serverID);
        q.bindValue(":name",  serverInfo.value(QStringLiteral("player_%1").arg(playerIndex), "Player"));
        q.bindValue(":team",  serverInfo.value(QStringLiteral(  "team_%1").arg(playerIndex), "0"));
        q.bindValue(":frags", serverInfo.value(QStringLiteral( "frags_%1").arg(playerIndex), "0"));
        q.bindValue(":mesh",  serverInfo.value(QStringLiteral(  "mesh_%1").arg(playerIndex), "default"));
        q.bindValue(":skin",  serverInfo.value(QStringLiteral(  "skin_%1").arg(playerIndex), "default"));
        q.bindValue(":face",  serverInfo.value(QStringLiteral(  "face_%1").arg(playerIndex), "default"));
        q.bindValue(":ping",  serverInfo.value(QStringLiteral(  "ping_%1").arg(playerIndex), "0"));
        q.bindValue(":misc",  ""); // reserved for additional query info
        q.bindValue(":dt_player", QDateTime::currentSecsSinceEpoch() );
        if ( ! q.exec() )
            return reportQuery(q);

        // successfull insert, increase player index
        playerIndex++;
    }

    return true;
}
