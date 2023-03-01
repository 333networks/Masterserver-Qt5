#ifndef LOGGER_H
#define LOGGER_H

#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

#include "Settings/settingstructure.h"
#include "Core/version.h"
#include "logprimitive.h"

class Logger
{
public:
    Logger();
    bool init(const QString          &applicationPath,
              const SettingStructure &settings);
    void stop();
    void logEvent(const QString &messageType,
                  const QString &message);

private:
    // path variables
    const QString _logDirectory = "../log";
    const QString _logLabel = "MasterServer-" + SHORT_VER;
    QString _logPath = "";

    // local variables
    int _cyclePeriod;
    QString _logFileName;
    QString _suppressLog;
    QString _suppressDisplay;

    // file i/o
    QFile _logFile;

private:
    bool cycleLogFile();
    bool openLogFile();
    void closeLogFile();
    bool writeLogFile(const QString message);

};

#endif // LOGGER_H
