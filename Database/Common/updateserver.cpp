#include "commonactions.h"

bool updateServer(const QString         &serverAddress,
                  const unsigned short  &serverPort,
                  const QString         &gamename,
                  const bool            &directBeacon,
                  const bool            &authenticated
                  )
{
    // update existing entry, but do not insert.
    QSqlQuery q;
    QString updateString;

    // update with available values
    updateString = "UPDATE serverlist SET ";

    /*
     * Note that direct/auth set to 'false' will NOT override a previous value in the database
     */

    // is this a direct beacon?
    if (directBeacon)
    {
        updateString += "f_direct = 1, ";
        updateString += "dt_beacon  = :timestamp, ";
    }

    // did the server authenticate?
    if (authenticated) updateString += "f_auth   = 1, ";

    updateString +=  "gamename = :gamename, "
                     "dt_updated = :timestamp "
                     "WHERE ip = :ip AND queryport = :queryport";

    // bind values and execute
    q.prepare(updateString);
    q.bindValue(":ip", serverAddress);
    q.bindValue(":queryport", serverPort);
    q.bindValue(":gamename", gamename);
    q.bindValue(":timestamp", QDateTime::currentSecsSinceEpoch() );

    if ( ! q.exec() )
        return reportQuery(q);

    // was a row updated?
    return (q.numRowsAffected() > 0);
}
