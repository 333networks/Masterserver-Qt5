#ifndef LOADSETTINGS_H
#define LOADSETTINGS_H

#include <QFile>
#include <QSettings>

#include "Logger/logprimitive.h"
#include "settingstructure.h"

// settings path (following README structure)
const QString _settingsPath = "../data/MasterServer-Settings.ini";

// load all settings from the config file
SettingStructure loadSettings(const QString &applicationPath);

// write all settings to the config file
void writeSettings(const QString &settingsFilePath);

#endif // LOADSETTINGS_H
