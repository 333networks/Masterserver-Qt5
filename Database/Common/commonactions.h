#ifndef COMMONACTIONS_H
#define COMMONACTIONS_H

#include <QDateTime>
#include "Database/databaseinterface.h"
#include "Logger/logprimitive.h"

// insert, update or delete a server from the list
bool insertServer(const QString         &serverAddress,
                  const unsigned short  &serverPort,
                  const QString         &gamename,
                  const bool            &directBeacon);

bool updateServer(const QString    &serverAddress,
                  const unsigned short  &serverPort,
                  const QString         &gamename,
                  const bool            &directBeacon,
                  const bool            &authenticated);

bool existServer(const QString         &serverAddress,
                 const unsigned short  &serverPort);

QSqlQuery selectServerList(const QString &gamename,
                           const int     &serverAge_s,
                           const bool    &withSyncData);

QStringList getGamenames(const int &serverAge_s);

QHash<QString, int> getNumGames(const int &serverAge_s);

#endif // COMMONACTIONS_H
