#include "logger.h"

bool Logger::init(const QString          &applicationPath,
                  const SettingStructure &settings)
{
    // test if log directory exists
    _logPath = applicationPath + "/" + _logDirectory;
    QDir logDir(_logPath);
    if ( ! logDir.mkpath(".") )
    {
        QTextStream(stdout) << "Failed to access log directory at " + _logPath << endl
                            << "Please create the director and/or apply relevant permissions." << endl;
        return false;
    }

    // log cycling -- determine log cycling setting
    const QHash<QString, int> logCycling = {
        {"never",   0},
        {"yearly",  1},
        {"monthly", 2},
        {"weekly",  3},
        {"daily",   4}
    };

    // load log-specific settings
    _cyclePeriod     = logCycling.value( settings.LoggingSettings.cycle );
    _suppressLog     = settings.LoggingSettings.suppressLog;
    _suppressDisplay = settings.LoggingSettings.suppressDisplay;

    // cycling initialisation ("never" is also a cycle)
    if ( ! cycleLogFile() )
    {
        QTextStream(stdout) << "Failed to access log file " + _logFileName + " at " + _logPath << endl;
        return false;
    }
    return true;
}
