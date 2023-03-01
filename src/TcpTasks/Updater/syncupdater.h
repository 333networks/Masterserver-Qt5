#ifndef SYNCUPDATER_H
#define SYNCUPDATER_H

#include <QTimer>
#include <QHostInfo>

#include "Core/CoreObject/coreobject.h"
#include "Database/Common/commonactions.h"
#include "TcpTasks/SyncClient/syncclient.h"

class SyncUpdater: public QObject
{
    Q_OBJECT
public:
    SyncUpdater(const QSharedPointer<CoreObject> &coreObject);
    bool scheduleUpdater();

private:
    QSharedPointer<CoreObject> _coreObject;
    const int _graceTime_ms = 5000;

    // update/ticker timer
    QTimer _updaterTimer;
    QTimer _syncTicker;

    // index
    int _syncIndex = 0;

private slots:
    void onSyncTickerAction();
};

#endif // SYNCUPDATER_H
