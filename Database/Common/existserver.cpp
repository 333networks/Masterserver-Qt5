#include "commonactions.h"

bool existServer(const QString         &serverAddress,
                 const unsigned short  &serverPort)
{
    // find existing entry
    QSqlQuery q;
    QString selectString = "SELECT id FROM serverlist "
                           "WHERE ip = :ip AND queryport = :queryport ";

    // bind values and execute
    q.prepare(selectString);
    q.bindValue(":ip", serverAddress);
    q.bindValue(":queryport", serverPort);

    if ( ! q.exec() )
        return reportQuery(q);

    // was a row/server found?
    return q.next();
}
