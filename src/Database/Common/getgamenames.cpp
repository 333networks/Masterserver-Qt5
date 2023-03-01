#include "commonactions.h"

QStringList getGamenames(const int &serverAge_s)
{
    // init output
    QStringList gamenameList;

    // retrieve active gamenames from database
    QSqlQuery q;
    QString selectString = "SELECT DISTINCT gamename FROM serverlist "
                           "WHERE dt_updated > :timestamp "
                           "ORDER BY gamename ASC";

    // bind and execute
    q.prepare(selectString);
    q.bindValue(":timestamp", QDateTime::currentDateTime().addSecs(-serverAge_s ).toSecsSinceEpoch());

    if ( ! q.exec() )
        reportQuery(q);

    // parse to stringlist
    while ( q.next() )
    {
         gamenameList.append( q.value(0).toString() );
    }

    return gamenameList;
}
