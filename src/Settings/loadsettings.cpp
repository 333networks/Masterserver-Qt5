#include "loadsettings.h"

SettingStructure loadSettings (const QString &applicationPath)
{
    // return object
    SettingStructure settings;

    // determine if file exists
    QString settingsFilePath = applicationPath + "/" + _settingsPath;
    if ( ! QFile(settingsFilePath).exists() )
    {
        // no settings file exists. ask to generate clean settings file.
        logPrimitive() << "No valid settings file was found at " << settingsFilePath << endl
                       << "Do you want to generate a clean settings file? [y/N]" << endl;

        // if opted for new config, create a new config file at the provided location
        if ( QTextStream(stdin).readLine().startsWith("y") )
        {
            // generate new settings file
            writeSettings(settingsFilePath);

            // inform that a file was written
            logPrimitive() << "A new settings file was generated. Please update your "
                           << "settings in " << settingsFilePath << endl
                           << "and restart the application." << endl;
        }
        // do NOT init = true, the application is now intended to shut down due to init==false
        return settings;
    }

    // open settings
    QSettings settingsFile(settingsFilePath, QSettings::IniFormat);
    if ( settingsFile.status() != QSettings::NoError )
    {
        // error occurred. report and quit.
        logPrimitive() << "An error occurred while loading the configuration (" << QString::number( settingsFile.status() ) << ").";

        // do NOT init = true, the application is now intended to shut down due to init==false
        return settings;
    }

    // logging settings
    settings.LoggingSettings.cycle            = settingsFile.value("Logging/CycleLogs",       settings.LoggingSettings.cycle).toString();
    settings.LoggingSettings.suppressLog      = settingsFile.value("Logging/SuppressLog",     settings.LoggingSettings.suppressLog).toString();
    settings.LoggingSettings.suppressDisplay  = settingsFile.value("Logging/SuppressDisplay", settings.LoggingSettings.suppressDisplay).toString();

    // beacon server settings (udp server)
    settings.BeaconServerSettings.beaconPort  = static_cast<unsigned short>( settingsFile.value("BeaconServer/BeaconPort", settings.BeaconServerSettings.beaconPort).toInt() );
    settings.BeaconServerSettings.doUplink    = settingsFile.value("BeaconServer/DoUplink",                                settings.BeaconServerSettings.doUplink).toBool();

    // listen server settings (tcp server)
    settings.ListenServerSettings.listenPort  = static_cast<unsigned short>( settingsFile.value("ListenServer/ListenPort", settings.ListenServerSettings.listenPort).toInt() );
    settings.ListenServerSettings.serverttl_s = settingsFile.value("ListenServer/ServerLifeTime_s",                        settings.ListenServerSettings.serverttl_s).toInt();

    // syncer settings (tcp client)
    settings.SyncerSettings.doSync            = settingsFile.value("Syncer/DoSync",         settings.SyncerSettings.doSync).toBool();
    settings.SyncerSettings.syncGames         = settingsFile.value("Syncer/SyncGames",      settings.SyncerSettings.syncGames).toString().toLower();
    settings.SyncerSettings.syncInterval_s    = settingsFile.value("Syncer/SyncInterval_s", settings.SyncerSettings.syncInterval_s).toInt();

    // error and number of items
    int parseError = -1;
    int len = settingsFile.beginReadArray("Syncer");

    // read list of syncer items
    for (int i = 0; i < len; i++)
    {
        settingsFile.setArrayIndex(i);
        SyncServer syncServer;
        QStringList strServer = settingsFile.value("SyncServer").toString().split(",");

        if ( strServer.length() >= 3 )
        {
            // parse
            syncServer.remoteAddress = strServer.value(0).trimmed();
            syncServer.beaconPort    = strServer.value(1).toUShort();
            syncServer.listenPort    = strServer.value(2).toUShort();

            // sanity checks
            if (! syncServer.remoteAddress.isEmpty() and
                  syncServer.beaconPort > 0 and
                  syncServer.listenPort > 0)
            {
                // add
                settings.SyncerSettings.syncServers.append(syncServer);
                continue;
            }
        }

        // else input error, do not continue parsing
        parseError = i;
        break;
    }
    settingsFile.endArray();

    // server checker (udp client ticker)
    settings.CheckerSettings.doCheck                = settingsFile.value("Checker/DoCheck",                settings.CheckerSettings.doCheck).toBool();
    settings.CheckerSettings.getExtendedInfo        = settingsFile.value("Checker/GetExtendedInformation", settings.CheckerSettings.getExtendedInfo).toBool();
    settings.CheckerSettings.timeServerInterval_ms  = settingsFile.value("Checker/ServerCheckInterval_ms", settings.CheckerSettings.timeServerInterval_ms).toInt();
    settings.CheckerSettings.timeCheckerReset_s     = settingsFile.value("Checker/CycleInterval_s",        settings.CheckerSettings.timeCheckerReset_s).toInt();

    // maintenance settings
    settings.MaintenanceSettings.doMaintenance              = settingsFile.value("Maintenance/DoMaintenance",  settings.MaintenanceSettings.doMaintenance).toBool();
    settings.MaintenanceSettings.timeMaintenanceInterval_s  = settingsFile.value("Maintenance/MaintainRate_s", settings.MaintenanceSettings.timeMaintenanceInterval_s).toInt();

    // public details
    settings.PublicInformationSettings.hostname  = settingsFile.value("PublicDetails/Hostname",  "").toString();
    settings.PublicInformationSettings.adminName = settingsFile.value("PublicDetails/AdminName", "").toString();
    settings.PublicInformationSettings.contact   = settingsFile.value("PublicDetails/Contact",   "").toString();

    // sanity checks
    try {
        // beacon / udp server
        if ( settings.BeaconServerSettings.beaconPort <= 0 ) throw QString("BeaconServer/BeaconPort");

        // listen / tcp server
        if ( settings.ListenServerSettings.listenPort <= 0 ) throw QString("ListenServer/ListenPort");
        if ( settings.ListenServerSettings.serverttl_s < 0)  throw QString("ListenServer/ServerLifeTimeSeconds");

        // syncer / tcp client
        if ( settings.SyncerSettings.syncGames.isEmpty() )   throw QString("Syncer/SyncGames");
        if ( settings.SyncerSettings.syncInterval_s <= 0 )   throw QString();
        if (parseError >= 0)                                 throw QStringLiteral("Syncer/SyncServer[%1]").arg(QString::number(parseError+1));

        // server checker / udp client
        if ( settings.CheckerSettings.timeServerInterval_ms < 10 )  throw QString("Checker/ServerCheckInterval_ms");
        if ( settings.CheckerSettings.timeCheckerReset_s < 1 )      throw QString("Checker/CycleInterval_s");

        // make sure that details are filled in
        if (settings.PublicInformationSettings.hostname.isEmpty())  throw QString("PublicDetails/Hostname");
        if (settings.PublicInformationSettings.adminName.isEmpty()) throw QString("PublicDetails/AdminName");
        if (settings.PublicInformationSettings.contact.isEmpty())   throw QString("PublicDetails/Contact");

    }
    catch (QString error)
    {
        logPrimitive() << "One or more settings are incorrect: setting \"" << error << "\" has an incorrect value! "
                       << "Please correct the value and restart the application." << endl;
        return settings;
    }

    // loading settings complete
    settings.init = true;
    return settings;
}
