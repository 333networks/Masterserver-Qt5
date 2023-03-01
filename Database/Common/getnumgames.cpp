#include "commonactions.h"

QHash<QString, int> getNumGames(const int &serverAge_s)
{
    QSqlQuery q;
    QHash<QString, int> numGames;

    // number of games
    q.prepare("SELECT count(gamename) FROM gameinfo");
    if ( ! q.exec() )
        reportQuery(q);
    if (q.next())
        numGames["numTotal"] = q.value(0).toInt();

    // number of active games
    QString selectString = "SELECT count(gamename) FROM gameinfo "
                           "WHERE num_direct > 0 "
                           "OR    num_total  > 0 ";
    q.prepare(selectString);
    q.bindValue(":timestamp", QDateTime::currentDateTime().addSecs(-serverAge_s ).toSecsSinceEpoch());
    if ( ! q.exec() )
        reportQuery(q);
    if (q.next())
        numGames["numActive"] = q.value(0).toInt();


    // TODO: get more relevant stats

    return numGames;
}
