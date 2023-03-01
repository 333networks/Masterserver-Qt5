#include "loadsettings.h"

void writeSettings (const QString &settingsFilePath)
{
    // user already specified that config is to be written
    QSettings settingsFile(settingsFilePath, QSettings::IniFormat);

    // initialise default settings to write to config
    SettingStructure settings;

    // logging settings
    settingsFile.setValue("Logging/CycleLogs",       settings.LoggingSettings.cycle);
    settingsFile.setValue("Logging/SuppressLog",     settings.LoggingSettings.suppressLog);
    settingsFile.setValue("Logging/SuppressDisplay", settings.LoggingSettings.suppressDisplay);

    // beacon server settings (udp server)
    settingsFile.setValue("BeaconServer/BeaconPort", settings.BeaconServerSettings.beaconPort);
    settingsFile.setValue("BeaconServer/DoUplink",   settings.BeaconServerSettings.doUplink);

    // listen server settings (tcp server)
    settingsFile.setValue("ListenServer/ListenPort",        settings.ListenServerSettings.listenPort);
    settingsFile.setValue("ListenServer/ServerLifeTime_s",  settings.ListenServerSettings.serverttl_s);

    // syncer settings (tcp client)
    settingsFile.setValue("Syncer/DoSync",      settings.SyncerSettings.doSync);
    settingsFile.setValue("Syncer/SyncGames",   settings.SyncerSettings.syncGames);
    settingsFile.setValue("Syncer/SyncInterval_s", settings.SyncerSettings.syncInterval_s);

    // if sync is set, but no servers are listed, generate default line with 333networks
    if ( settings.SyncerSettings.doSync and settings.SyncerSettings.syncServers.size() <= 0 )
    {
        SyncServer defaultServer;
        defaultServer.remoteAddress = "master.333networks.com"; // default settings are correct
        settings.SyncerSettings.syncServers.append(defaultServer);
    }

    // write list of servers
    QListIterator<SyncServer> syncRecordIterator(settings.SyncerSettings.syncServers);
    int i = 0;

    settingsFile.beginWriteArray("Syncer");
    while ( syncRecordIterator.hasNext() )
    {
        // next list item
        SyncServer syncServer = syncRecordIterator.next();
        if ( syncServer.remoteAddress.size() > 0 )
        {
            // write to settings file
            settingsFile.setArrayIndex(i++);

            // output format 1\SyncServer=master.333networks.com, 27900, 28900
            QString strServer = QStringLiteral("%1, %2, %3")
                    .arg(syncServer.remoteAddress,
                         QString::number(syncServer.beaconPort),
                         QString::number(syncServer.listenPort) );
            settingsFile.setValue("SyncServer", strServer);
        }

    }
    settingsFile.endArray();

    // server checker (udp client ticker)
    settingsFile.setValue("Checker/DoCheck",                settings.CheckerSettings.doCheck);
    settingsFile.setValue("Checker/GetExtendedInformation", settings.CheckerSettings.getExtendedInfo);
    settingsFile.setValue("Checker/ServerCheckInterval_ms", settings.CheckerSettings.timeServerInterval_ms);
    settingsFile.setValue("Checker/CycleInterval_s",        settings.CheckerSettings.timeCheckerReset_s);

    // maintenance settings
    settingsFile.setValue("Maintenance/DoMaintenance",  settings.MaintenanceSettings.doMaintenance);
    settingsFile.setValue("Maintenance/MaintainRate_s", settings.MaintenanceSettings.timeMaintenanceInterval_s);

    // public details
    settingsFile.setValue("PublicDetails/Hostname",  settings.PublicInformationSettings.hostname);
    settingsFile.setValue("PublicDetails/AdminName", settings.PublicInformationSettings.adminName);
    settingsFile.setValue("PublicDetails/Contact",   settings.PublicInformationSettings.contact);
}
