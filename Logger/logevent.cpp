#include "logger.h"

void Logger::logEvent(const QString &messageType,
                      const QString &message)
{
    // printing to display suppressed?
    if ( ! _suppressDisplay.contains(messageType) and ! _suppressDisplay.contains("all") )
    {
        QString dateTimeStr(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz"));
        logPrimitive() << QStringLiteral("[%1][%2]\t%3").arg(dateTimeStr, messageType, message.toLatin1()) << endl;
    }

    // printing to logfile suppressed?
    if ( ! _suppressLog.contains(messageType) and ! _suppressLog.contains("all") )
    {
        // write message to log
        QString dateTimeStr(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz"));
        writeLogFile( QStringLiteral("[%1][%2]\t%3").arg(dateTimeStr, messageType, message.toLatin1()) );
    }
}
