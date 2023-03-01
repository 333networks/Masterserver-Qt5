#include "statuschecker.h"

QSqlQuery StatusChecker::getNextServer(const int &currentServerId)
{
    // select server with >id
    QSqlQuery q;
    QString selectString = "SELECT id, ip, queryport, gamename, f_auth FROM serverlist "
                           "WHERE id > :currentServerID "
                           "AND (dt_updated > :timestamp OR dt_sync > :timestamp)"
                           "LIMIT 1";

    // bind values and execute
    q.prepare(selectString);
    q.bindValue(":currentServerID", currentServerId);
    q.bindValue(":timestamp", QDateTime::currentDateTime()
                .addSecs(-_coreObject->Settings.ListenServerSettings.serverttl_s)
                .toSecsSinceEpoch());
    if ( ! q.exec() )
        reportQuery(q);

    return q;
}
