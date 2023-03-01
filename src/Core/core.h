#ifndef CORE_H
#define CORE_H

#include "Core/CoreObject/coreobject.h"
#include "Core/GameInfo/loadsupportedgames.h"
#include "Core/version.h"
#include "Database/databaseinterface.h"
#include "Settings/loadsettings.h"
#include "Maintenance/maintenance.h"
#include "Protocols/GameSpy0/securevalidate.h"
#include "UdpTasks/BeaconServer/beaconserver.h"
#include "UdpTasks/StatusChecker/statuschecker.h"
#include "TcpTasks/ListenServer/listenserver.h"
#include "TcpTasks/Updater/syncupdater.h"

class Core : public QObject
{
    Q_OBJECT
public:
    Core(QString applicationPath);
    void run();
    void shutdown();

private:
    // root path
    QString _applicationPath = "";

    // internal dataobject
    QSharedPointer<CoreObject> _coreObject = QSharedPointer<CoreObject>(new CoreObject);

    // Networking services
    QSharedPointer<BeaconServer> _udpBeaconServer = QSharedPointer<BeaconServer>(new BeaconServer(_coreObject));
    QSharedPointer<ListenServer> _tcpListenServer = QSharedPointer<ListenServer>(new ListenServer(_coreObject));

    // Updaters
    QSharedPointer<SyncUpdater>   _syncUpdater   = QSharedPointer<SyncUpdater>  (new SyncUpdater  (_coreObject));
    QSharedPointer<StatusChecker> _statusChecker = QSharedPointer<StatusChecker>(new StatusChecker(_coreObject));

    // Maintenance
    QSharedPointer<Maintenance> _maintenance = QSharedPointer<Maintenance>(new Maintenance(_coreObject));
};

#endif // CORE_H
