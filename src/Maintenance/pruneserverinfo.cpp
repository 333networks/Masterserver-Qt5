#include "maintenance.h"

int Maintenance::pruneServerInfo()
{
    QString deleteString = "DELETE FROM serverinfo "
                           "WHERE sid NOT IN ( "
                           "SELECT id FROM serverlist)";
    QSqlQuery deleteQuery;
    deleteQuery.prepare(deleteString);

    if ( ! deleteQuery.exec() )
        return reportQuery(deleteQuery);

    return deleteQuery.numRowsAffected();
}
