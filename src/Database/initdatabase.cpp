#include "databaseinterface.h"

bool initDatabase(const QString applicationPath)
{
    // open SQLite database
    QSqlDatabase dbi = QSqlDatabase::addDatabase( "QSQLITE" );
    dbi.setDatabaseName(applicationPath + "/" + _sqlitePath);

    // open database
    if ( ! dbi.open() )
    {
        QSqlError sqlError = dbi.lastError();
        logPrimitive() << "Error opening database: " << sqlError.text() << endl;
        return false;
    }

    // SQLite tweaks with async, wal and timeout
    dbi.exec("PRAGMA synchronous = OFF");
    dbi.exec("PRAGMA journal_mode = WAL"); // previous: MEMORY
    dbi.exec("PRAGMA busy_timeout = 500"); // 500ms

    { // check if the database was generated with this version of the software

        // serverlist exists (and thus the other tables exist) but not version
        if ( dbi.tables().contains("serverlist") and ! dbi.tables().contains("versioninfo"))
        {
            // outdated because the "version" table does not even exist yet (pre-0.14)
            logPrimitive() << "The database tables are outdated (pre-0.14). Please remove the current database." << endl;
            return false;
        }

        // does the version table exist, and does it contain the correct version?
        if ( dbi.tables().contains("versioninfo") )
        {
            float dbVersion = 0.0;
            QSqlQuery versionQuery;
            versionQuery.prepare("SELECT version FROM versioninfo WHERE type = \"database\"");

            // failed query? (unlikely)
            if ( ! versionQuery.exec() )
                return reportQuery(versionQuery);

            if (versionQuery.next())
                dbVersion = versionQuery.value("version").toFloat();

            // version check is intentionally hardcoded since the last change in database structure (currently v0.15)
            if ( dbVersion < DATABASE_VERSION )
            {
                logPrimitive() << "The database tables are outdated (" << QString::number(dbVersion) << "). Please remove the current database." << endl;
                return false;
            }

            // tables up to date
            return true;
        }
    }

    // relevant tables do not exist. ask to generate.
    logPrimitive() << "The database table do not exist. Do you want to generate these? [y/N]" << endl;

    // if no, do not proceed.
    if ( ! QTextStream(stdin).readLine().startsWith("y") )
    {
        // do not generate. failed to load database.
        return false;
    }

    // create tables and proceed
    return createTables();
}
