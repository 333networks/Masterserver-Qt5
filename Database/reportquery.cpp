#include "databaseinterface.h"

bool reportQuery(const QSqlQuery &q)
{
    logPrimitive() << "Database query error." << endl << "" << endl
                   << "Technical info: " << q.lastQuery() << endl << "" << endl
                   << "Reported error: " <<  q.lastError().text() << endl;

    // always return false, so we can report error and return false in one line.
    return false;
}
