#ifndef COREOBJECT_H
#define COREOBJECT_H

#include "Core/CoreObject/serverinfostructure.h"
#include "Core/GameInfo/gameinfostructure.h"
#include "Logger/logger.h"
#include "Settings/settingstructure.h"

class CoreObject
{
public:
    CoreObject();

    // struct with internal and external settings
    SettingStructure Settings;

    // list of game details: gamename, cipher
    QHash<QString, GameInfo> SupportedGames;

    // logging functions
    Logger Log;

    // server address list acquired through third party masterservers
    QList<ServerInfo> PendingServers;

    // generate our session/identification string, to prevent self-syncing
    QString masterserverIdentity; // msid value
};

#endif // COREOBJECT_H
