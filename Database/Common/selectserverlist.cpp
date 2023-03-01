#include "commonactions.h"

QSqlQuery selectServerList(const QString &gamename,
                           const int     &serverAge_s,
                           const bool    &withSyncData)
{
    // retrieve servers from database
    QSqlQuery q;
    QString selectString = "SELECT ip, queryport FROM serverlist "
                           "WHERE gamename = :gamename ";

    if ( withSyncData )
    {
        // relies on sync data to be accurate (if checker is not enabled)
        selectString += "AND (dt_updated > :timestamp OR dt_sync > :timestamp)";
    }
    else
    {
        // sync data may not be (sufficiently) accurate, only use data that we verified directly
        selectString += "AND dt_updated > :timestamp";
    }

    // bind values and execute
    q.prepare(selectString);
    q.bindValue(":gamename", gamename);
    q.bindValue(":timestamp", QDateTime::currentDateTime().addSecs(-serverAge_s ).toSecsSinceEpoch());
    if ( ! q.exec() )
        reportQuery(q);

    return q;
}
