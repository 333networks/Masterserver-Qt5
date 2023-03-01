#ifndef SETTINGSTRUCTURE_H
#define SETTINGSTRUCTURE_H

#include <QString>
#include <QList>

// masterservers sync options
struct SyncServer
{
    // domain name string, not QHostAddress
    QString remoteAddress;

    // udp port
    unsigned short int beaconPort = 27900;

    // tcp port
    unsigned short int listenPort = 28900;
};

// cascaded struct with setting structure
struct SettingStructure
{
    // initialisation check
    bool init = false;

    // log settings
    struct LoggingSettings
    {
        // never, yearly, monthly, weekly, daily
        QString cycle = "weekly";

        // suppress type: [timestamp][type] <message>
        QString suppressLog     = "debug udp tcp";
        QString suppressDisplay = "debug udp tcp";
    }
    LoggingSettings;

    // udp beacon server settings
    struct BeaconServerSettings
    {
        // default port 27900
        unsigned short int beaconPort = 27900;

        // uplink settings enabled by default
        bool doUplink = true;
    }
    BeaconServerSettings;

    // tcp listen server settings
    struct ListenServerSettings
    {
        // default port 28900
        unsigned short int listenPort = 28900;

        // server time to live for client list
        int serverttl_s = 1800;
    }
    ListenServerSettings;

    // synchronisation settings (works only with 333networks-compatible masterservers)
    struct SyncerSettings
    {
        // syncer settings enabled by default
        bool doSync = true;

        // sync games (which games to sync)
        QString syncGames = "all";

        // list of servers to sync
        QList<SyncServer> syncServers;

        // sync event interval
        int syncInterval_s = 1800;
    }
    SyncerSettings;

    // checker settings (query all individual servers to determine their state)
    struct CheckerSettings
    {
        // check individual remote servers?
        bool doCheck = true;

        // get information for the website too?
        bool getExtendedInfo = true;

        // time between servers (ticker)
        int timeServerInterval_ms = 250;

        // cycle time before a reset takes place
        int timeCheckerReset_s = 900; // every 15 minutes
    } CheckerSettings;

    // maintenance settings
    struct MaintenanceSettings
    {
        // do maintenance?
        bool doMaintenance = true;

        // interval
        int timeMaintenanceInterval_s = 300; // every 5 minutes
    }
    MaintenanceSettings;

    // contact information
    struct PublicInformationSettings
    {
        // your website, domain name, brand name or identity
        QString hostname = "";

        // your (nick)name
        QString adminName = "";

        // your e-mailaddress (format not checked, TODO)
        QString contact = "";
    }
    PublicInformationSettings;
};

#endif // SETTINGSTRUCTURE_H
