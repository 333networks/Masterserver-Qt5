#include "databaseinterface.h"

void closeDatabase()
{
    QSqlDatabase dbi;
    dbi.close();
}
