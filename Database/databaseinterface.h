#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDataStream>
#include "Core/version.h"
#include "Logger/logprimitive.h"

// database file path (following README structure)
const QString _sqlitePath = "../data/masterserver.db";

// init and close
bool initDatabase(const QString applicationPath);
void closeDatabase();

// report execution errors (terminal/display only)
bool reportQuery(const QSqlQuery &q); // always returns false!

// generate tables in first run
bool createTables();

#endif // DATABASEINTERFACE_H
