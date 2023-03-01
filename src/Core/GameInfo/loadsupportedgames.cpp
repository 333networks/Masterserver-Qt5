#include "loadsupportedgames.h"

QHash<QString, GameInfo> loadSupportedGames (const QString &applicationPath)
{
    // return hash
    QHash<QString, GameInfo> supportedGames;

    // supported games file path (following README structure)
    const QString supportPath = applicationPath + "/" + _supportedPath;

    // determine if file exists
    if ( ! QFile(supportPath).exists() )
    {
        // no game info file exists
        logPrimitive() << "No games file found at " << supportPath << endl
                       << "Please provide the correct file . " << endl;
        return supportedGames;
    }

    // load config file
    QFile supportedFile(supportPath);
    if ( ! supportedFile.open(QIODevice::ReadOnly) )
    {
        // error occurred. report and quit.
        logPrimitive() << "Unable to open the game file. Is the file open or in use?" << endl;
        return supportedGames;
    }

    // stats
    int total = 0;

    // load as json object
    QJsonDocument supportedJsonData(QJsonDocument::fromJson(supportedFile.readAll()));

    /* Optimisation for web interface: add games to the database.
     *
     * If database/json data do not match, replace database table
     * with json data. This should only occur during database creation
     * or when manual changes were made to the json file (which is rare).
     */
    bool dbInsert = supportedJsonData.array().count() != getNumGames(0).value("numTotal", 0);
    QSqlQuery q;
    if (dbInsert)
    {
        // no game info file exists
        logPrimitive() << "Game info mismatch in database. Reloading json data. ("
                       << supportedJsonData.array().count() << "/"
                       << getNumGames(0).value("numTotal", 0) << ")" << endl;

        // void existing data
        q.prepare("DELETE FROM gameinfo");
        if ( ! q.exec() )
            reportQuery(q);
    }

    for (int i = 0; i < supportedJsonData.array().count(); i++)
    {
        // get the game object
        QJsonObject thisGame = supportedJsonData.array().at(i).toObject();

        GameInfo gameInfo;
        gameInfo.gamename = thisGame.value("gamename").toString("");
        gameInfo.cipher   = thisGame.value("cipher").toString("");
        gameInfo.port     = static_cast<unsigned short>(thisGame.value("port").toDouble(0));
        gameInfo.label    = thisGame.value("label").toString("");

        // insert in db if needed
        if (dbInsert)
        {
            QString insertString = "INSERT INTO gameinfo (gamename, label) "
                                   "VALUES (:gamename, :label)";

            q.prepare(insertString);
            q.bindValue(":gamename", gameInfo.gamename );
            q.bindValue(":label",    gameInfo.label);

            if ( ! q.exec() )
                reportQuery(q);
        }

        // add to list
        supportedGames.insert(gameInfo.gamename, gameInfo);
        total++;
    }

    // no games found? report this!
    if (total <= 0)
    {
        // no game info exists
        logPrimitive() << "No game info found in file at " << supportPath << endl
                       << "Please provide the correct file . " << endl;
        return supportedGames;
    }

    // all games parsed. done.
    return supportedGames;
}
