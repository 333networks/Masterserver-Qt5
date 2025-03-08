#include "maintenance.h"

int Maintenance::updateStats()
{
    // result
    int numOfUpdatedStats = 0;

    // get list of gamenames in database
    QString   selectGamenames = "SELECT DISTINCT gamename FROM serverlist";
    QSqlQuery gamenameQuery;
    gamenameQuery.prepare(selectGamenames);
    if ( ! gamenameQuery.exec() )
        return reportQuery(gamenameQuery);

    // update stats for every gamename
    while ( gamenameQuery.next() )
    {
        // get next gamename
        QString gamename = gamenameQuery.value(0).toString();

        // hotfix hf4
        // update number of servers directly
        QString numStatsAll = "UPDATE gameinfo "
                              "SET num_total = ( "
                                "SELECT COUNT(gamename) FROM serverlist "
                                "WHERE gamename = :gamename AND dt_updated > :timestamp "
                              "), "
                              "num_direct = ( "
                                "SELECT COUNT(gamename) FROM serverlist "
                                "WHERE gamename = :gamename AND dt_updated > :timestamp AND f_direct"
                              ") WHERE gamename = :gamename";

        QSqlQuery statQuery;
        statQuery.prepare(numStatsAll);
        statQuery.bindValue(":gamename", gamename);
        statQuery.bindValue(":timestamp", QDateTime::currentDateTime()
                                              .addSecs(-_coreObject->Settings.ListenServerSettings.serverttl_s).toSecsSinceEpoch());
        if ( ! statQuery.exec() )
            return reportQuery(statQuery);

        // update counter
        numOfUpdatedStats += statQuery.numRowsAffected();
    }

    return numOfUpdatedStats;
}
