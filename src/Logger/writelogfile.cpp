#include "logger.h"

bool Logger::writeLogFile(const QString message)
{
    // first see if we need to cycle the log file already
    cycleLogFile();

    // write to file
    if ( _logFile.isOpen() )
    {
        QTextStream logStream(&_logFile);
        logStream << message << endl;
        return true;
    }

    logPrimitive() << "[log error]" << message;
    return false;
}
