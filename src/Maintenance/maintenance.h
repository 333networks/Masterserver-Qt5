#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <QTimer>

#include "Core/CoreObject/coreobject.h"
#include "Database/databaseinterface.h"

class Maintenance: public QObject
{
    Q_OBJECT

public:
    Maintenance(const QSharedPointer<CoreObject> &coreObject);
    bool scheduleMaintenance();

private:
    QSharedPointer<CoreObject> _coreObject;

    // schedule timer
    QTimer _maintenanceTimer;

private:
    int updateStats();
    int pruneBeacons();
    int pruneServerInfo();
    int prunePlayers();

private slots:
    void onMaintenanceTimerAction();
};

#endif // MAINTENANCE_H
