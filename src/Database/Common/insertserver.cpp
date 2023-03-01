#include "commonactions.h"

// insert a server into the list
bool insertServer(const QString         &serverAddress,
                  const unsigned short  &serverPort,
                  const QString         &gamename,
                  const bool            &directBeacon)
{
    // insert query string
    QSqlQuery q;
    QString insertString = "INSERT INTO serverlist (ip, queryport, gamename, f_direct) "
                           "VALUES (:ip, :queryport, :gamename, :directbeacon)";

    // bind values and execute
    q.prepare(insertString);
    q.bindValue(":ip", serverAddress);
    q.bindValue(":queryport", serverPort);
    q.bindValue(":gamename", gamename);
    q.bindValue(":directbeacon", ( directBeacon ? 1 : 0 ) ); // bool to int

    if ( ! q.exec() )
        return reportQuery(q);

    // was a row inserted?
    return (q.numRowsAffected() > 0);
}
