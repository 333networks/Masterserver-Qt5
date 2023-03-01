#include "syncclient.h"

bool SyncClient::updateSyncedServer(const QString        &serverAddress,
                                    const unsigned short &serverPort)
{
    // update existing entry, but do not insert.
    QSqlQuery q;
    QString updateString;

    // update with available values
    updateString = "UPDATE serverlist SET "
                   "dt_sync = :timestamp "
                   "WHERE ip = :ip "
                   "AND queryport = :queryport";

    // bind values and execute
    q.prepare(updateString);
    q.bindValue(":ip", serverAddress);
    q.bindValue(":queryport", serverPort);
    q.bindValue(":timestamp", QDateTime::currentSecsSinceEpoch() );

    if ( ! q.exec() )
        return reportQuery(q);

    // was a row updated?
    return (q.numRowsAffected() > 0);
}
