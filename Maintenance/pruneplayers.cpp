#include "maintenance.h"

int Maintenance::prunePlayers()
{
    QString deleteString = "DELETE FROM playerinfo "
                           "WHERE playerinfo.sid NOT IN ( "
                           "SELECT serverinfo.sid FROM serverinfo) "
                           "OR dt_player < :timestamp";
    QSqlQuery deleteQuery;
    deleteQuery.prepare(deleteString);
    deleteQuery.bindValue(":timestamp", QDateTime::currentDateTime()
                        .addSecs(-7200).toSecsSinceEpoch()); // 2 hours

    if ( ! deleteQuery.exec() )
        return reportQuery(deleteQuery);

    return deleteQuery.numRowsAffected();
}
