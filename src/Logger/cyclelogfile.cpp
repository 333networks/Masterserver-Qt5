#include "logger.h"

// if "now" matches the previous logfile name, no cycling needed. otherwise, determine new logfile name.
bool Logger::cycleLogFile()
{
    // calculate the expected log filename
    QString newFilename = _logLabel;

    // get date to see if we need to cycle
    QDate date(QDate::currentDate());

    // when is it necessary to cycle?
    switch ( _cyclePeriod )
    {
        case 1: // yearly
            newFilename += date.toString("-yyyy");
            break;
        case 2: // monthly
            newFilename += date.toString("-yyyy-MM");
            break;
        case 3: // weekly
            newFilename += date.toString("-yyyy") + "-week" + QString("%1").arg(date.weekNumber(), 2, 10, QChar('0'));
            break;
        case 4: // daily
            newFilename += date.toString("-yyyy-MM-dd");
            break;
        default:; // 0 = never
    }

    // add log file extension
    newFilename += ".log";

    // if current and new filename are not the same, cycle log file
    if (_logFileName != newFilename)
    {
        // cycle log file
        _logFileName = newFilename;

        // close the existing/opened log file
        closeLogFile();

        // open the new log file (pass possible error to caller)
        return openLogFile();
    }

    // else: no cycling necessary
    return true;
}
