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

        // determine beacon and server counts
        QString selectStats = "SELECT COUNT(CASE WHEN f_direct THEN 1 END) AS num_direct, "
                                                                 "count(*) AS num_total "
                              "FROM serverlist "
                              "WHERE gamename = :gamename AND dt_updated > :timestamp";
        QSqlQuery statQuery;
        statQuery.prepare(selectStats);
        statQuery.bindValue(":gamename", gamename);
        statQuery.bindValue(":timestamp", QDateTime::currentDateTime()
                            .addSecs(-_coreObject->Settings.ListenServerSettings.serverttl_s).toSecsSinceEpoch());
        if ( ! statQuery.exec() )
            return reportQuery(statQuery);

        // get values
        int num_direct = -1;
        int num_total  = -1;
        if ( statQuery.next() )
        {
            num_direct = statQuery.value("num_direct").toInt();
            num_total  = statQuery.value("num_total").toInt();
        }

        // write to db
        QString updateStatQuery = "UPDATE gameinfo "
                                  "SET num_direct = :num_direct, num_total = :num_total "
                                  "WHERE gamename = :gamename ";
        statQuery.prepare(updateStatQuery);
        statQuery.bindValue(":num_direct", num_direct);
        statQuery.bindValue(":num_total", num_total);
        statQuery.bindValue(":gamename", gamename);
        if ( ! statQuery.exec() )
            return reportQuery(statQuery);

        // update counter
        numOfUpdatedStats += statQuery.numRowsAffected();
    }

    return numOfUpdatedStats;
}
