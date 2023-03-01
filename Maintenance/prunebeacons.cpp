#include "maintenance.h"

int Maintenance::pruneBeacons()
{
    QString updateString = "UPDATE serverlist "
                           "SET f_direct = 0 "
                           "WHERE f_direct = 1 AND dt_beacon < :timestamp";
    QSqlQuery updateQuery;
    updateQuery.prepare(updateString);
    updateQuery.bindValue(":timestamp", QDateTime::currentDateTime().addSecs(-600).toSecsSinceEpoch());

    if ( ! updateQuery.exec() )
        return reportQuery(updateQuery);

    return updateQuery.numRowsAffected();
}
