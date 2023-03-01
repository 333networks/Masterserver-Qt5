#include "databaseinterface.h"

bool createTables()
{
    { // create serverlist
        QSqlQuery q;
        QString createServerlist = QStringLiteral(
            "CREATE TABLE serverlist("
            "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
            "gamename      TEXT    NOT NULL DEFAULT '',"
            "ip            TEXT    NOT NULL ,"
            "queryport     INTEGER NOT NULL DEFAULT 0,"
            "t_protocol    INTEGER NOT NULL DEFAULT 0,"
            "f_blacklist   INTEGER NOT NULL DEFAULT 0,"
            "f_auth        INTEGER NOT NULL DEFAULT 0,"
            "f_direct      INTEGER NOT NULL DEFAULT 0,"
            "dt_added      BIGINT  DEFAULT (CAST(strftime('%s','now') AS BIGINT)),"
            "dt_beacon     BIGINT  DEFAULT (CAST(strftime('%s','now') AS BIGINT)),"
            "dt_sync       BIGINT  DEFAULT (CAST(strftime('%s','now') AS BIGINT)),"
            "dt_updated    BIGINT  DEFAULT (CAST(strftime('%s','now') AS BIGINT))"
            ")");

        // bind values and execute (not all db-interfaces support prepare(create table), exec directly)
        if ( ! q.exec(createServerlist) )
            return reportQuery(q);
    }

    { // create serverinfo
        QSqlQuery q;
        QString createServerinfo = QStringLiteral(
            "CREATE TABLE serverinfo("
                "sid                    INTEGER NOT NULL DEFAULT 0,"
                "hostport               INTEGER NOT NULL DEFAULT 0,"
                "hostname               TEXT,"
                "gamever                TEXT,"
                "minnetver              TEXT,"
                "country                TEXT,"
                "location               TEXT,"
                "listenserver           TEXT,"
                "adminname              TEXT,"
                "adminemail             TEXT,"
                "password               TEXT,"
                "gametype               TEXT,"
                "gamestyle              TEXT,"
                "changelevels           TEXT,"
                "maptitle               TEXT,"
                "mapname                TEXT,"
                "numplayers             INTEGER DEFAULT 0,"
                "maxplayers             INTEGER DEFAULT 0,"
                "minplayers             INTEGER DEFAULT 0,"
                "botskill               TEXT,"
                "balanceteams           TEXT,"
                "playersbalanceteams    TEXT,"
                "friendlyfire           TEXT,"
                "maxteams               TEXT,"
                "timelimit              TEXT,"
                "goalteamscore          TEXT,"
                "fraglimit              TEXT,"
                "mutators               TEXT DEFAULT 'None',"
                "misc                   TEXT,"
                "dt_serverinfo          BIGINT DEFAULT (CAST(strftime('%s','now') AS BIGINT))"
            ")");

        // bind values and execute
        if ( ! q.exec(createServerinfo) )
            return reportQuery(q);
    }

    { // create playerinfo
        QSqlQuery q;
        QString createPlayerlist = QStringLiteral(
            "CREATE TABLE playerinfo("
                "sid        INTEGER NOT NULL DEFAULT 0,"
                "name       TEXT DEFAULT 'Player',"
                "team       TEXT,"
                "frags      INTEGER DEFAULT 0,"
                "mesh       TEXT,"
                "skin       TEXT,"
                "face       TEXT,"
                "ping       INTEGER DEFAULT 0,"
                "misc       TEXT,"
                "dt_player  BIGINT DEFAULT (CAST(strftime('%s','now') AS BIGINT))"
            ")");

        // bind values and execute
        if ( ! q.exec(createPlayerlist) )
            return reportQuery(q);
    }

    { // create gameinfo
        QSqlQuery q;
        QString createGamelist = QStringLiteral(
            "CREATE TABLE gameinfo("
                "gamename   TEXT    NOT NULL DEFAULT '',"
                "label      TEXT    NOT NULL DEFAULT '',"
                "num_total  INTEGER NOT NULL DEFAULT 0,"
                "num_direct INTEGER NOT NULL DEFAULT 0"
            ")");

        // bind values and execute
        if ( ! q.exec(createGamelist) )
            return reportQuery(q);
    }

    { // create version info
        QSqlQuery q;
        QString createVersion = QStringLiteral(
            "CREATE TABLE versioninfo("
                "type    TEXT    NOT NULL DEFAULT '',"
                "version TEXT    NOT NULL DEFAULT ''"
            ")");

        // bind values and execute
        if ( ! q.exec(createVersion) )
            return reportQuery(q);

        // insert current version directly into the db during creation
        q.prepare("INSERT INTO versioninfo (type, version) VALUES (\"database\", :currentver)");
        q.bindValue(":currentver", BUILD_VERSION);

        if ( ! q.exec() )
            reportQuery(q);
    }

    // TODO: optional: write an sql file to /data/tables.sql for easy access on generated tables.

    // no errors
    return true;
}
