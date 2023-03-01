#include "logger.h"

bool Logger::openLogFile()
{
    // verify that there is no open logfile already
    if ( _logFile.openMode() == QIODevice::NotOpen)
    {
        // set new log file
        _logFile.setFileName(_logPath + "/" + _logFileName);
        return _logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
    // else:
    // file already open, can not get a lock
    return false;
}

void Logger::closeLogFile()
{
    // close file if open. if already closed, ignore.
    _logFile.close();
}
