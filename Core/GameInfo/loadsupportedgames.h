#ifndef LOADSUPPORTEDGAMES_H
#define LOADSUPPORTEDGAMES_H

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "Database/Common/commonactions.h"
#include "gameinfostructure.h"

// load supported games from json file

const QString _supportedPath = "../data/SupportedGames.json";
QHash<QString, GameInfo> loadSupportedGames (const QString &applicationPath);

#endif // LOADSUPPORTEDGAMES_H
