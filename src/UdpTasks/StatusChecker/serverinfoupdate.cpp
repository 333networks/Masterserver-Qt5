#include "statuschecker.h"

bool StatusChecker::updateServerInfo(const QString                 &serverAddress,
                                     const unsigned short          &serverPort,
                                     const QHash<QString, QString> &serverInfo)
{
    // start query string and query
    QString updateString = "UPDATE serverinfo SET ";

    { // add parameter placeholders
        QStringListIterator updateField(_updateFields);
        while ( updateField.hasNext() )
        {
            QString field = updateField.next();
            if ( ! serverInfo.value(field).isEmpty() )
                updateString += QStringLiteral("%1 = :%1, ").arg( field );
        }
    }

    // complete update string
    updateString += "mutators = :mutators, "
                    "dt_serverinfo = :dt_serverinfo "
                    "WHERE sid IN ("
                    "SELECT id FROM serverlist "
                    "WHERE ip = :ip AND queryport = :queryport"
                    ")";

    // bind parameters
    QSqlQuery q;
    q.prepare(updateString);

    { // add parameter bindings
        QStringListIterator updateField(_updateFields);
        while ( updateField.hasNext() )
        {
            QString field = updateField.next();
            if ( ! serverInfo.value(field).isEmpty() )
                q.bindValue( QStringLiteral(":%1").arg(field),
                             serverInfo.value(field));
        }
    }

    // bind remaining values and execute
    q.bindValue(":mutators",      serverInfo.value("mutators", "None"));
    q.bindValue(":dt_serverinfo", QDateTime::currentSecsSinceEpoch() );
    q.bindValue(":ip",        serverAddress);
    q.bindValue(":queryport", serverPort);

    if ( ! q.exec() )
        return reportQuery(q);

    // was a row updated?
    return (q.numRowsAffected() > 0);
}
