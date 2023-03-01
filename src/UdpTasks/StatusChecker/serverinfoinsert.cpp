#include "statuschecker.h"

bool StatusChecker::insertServerInfo(const QString        &serverAddress,
                                     const unsigned short &serverPort)
{
    // insert query string
    QSqlQuery q;
    QString insertString = "INSERT INTO serverinfo (sid) "
                           "SELECT id FROM serverlist "
                           "WHERE ip = :ip AND queryport = :queryport";

    // bind values and execute
    q.prepare(insertString);
    q.bindValue(":ip",        serverAddress);
    q.bindValue(":queryport", serverPort);

    if ( ! q.exec() )
        return reportQuery(q);

    // was a row updated?
    return (q.numRowsAffected() > 0);
}
