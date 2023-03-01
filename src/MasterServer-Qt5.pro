QT += core network sql
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Core/CoreObject/coreobject.cpp \
        Database/Common/existserver.cpp \
        Database/Common/getgamenames.cpp \
        Database/Common/getnumgames.cpp \
        Database/Common/insertserver.cpp \
        Database/Common/selectserverlist.cpp \
        Database/Common/updateserver.cpp \
        Database/closedatabase.cpp \
        Database/createtables.cpp \
        Database/initdatabase.cpp \
        Database/reportquery.cpp \
        Core/GameInfo/loadsupportedgames.cpp \
        Logger/cyclelogfile.cpp \
        Logger/initlog.cpp \
        Logger/logevent.cpp \
        Logger/logger.cpp \
        Logger/openlogfile.cpp \
        Logger/writelogfile.cpp \
        Settings/loadsettings.cpp \
        Settings/writesettings.cpp \
        Core/core.cpp \
        Core/corerun.cpp \
        Maintenance/maintenance.cpp \
        Maintenance/onmaintenancetimeraction.cpp \
        Maintenance/prunebeacons.cpp \
        Maintenance/pruneplayers.cpp \
        Maintenance/pruneserverinfo.cpp \
        Maintenance/schedulemaintenance.cpp \
        Maintenance/updatestats.cpp \
        Protocols/GameSpy0/algorithm.cpp \
        Protocols/GameSpy0/gamespy0.cpp \
        Protocols/GameSpy0/securevalidate.cpp \
        Protocols/overrides.cpp \
        TcpTasks/ListenClientHandler/compileserverlist.cpp \
        TcpTasks/ListenClientHandler/compilesynclist.cpp \
        TcpTasks/ListenClientHandler/listenclientdisconnect.cpp \
        TcpTasks/ListenClientHandler/listenclienthandler.cpp \
        TcpTasks/ListenClientHandler/onlistenclientread.cpp \
        TcpTasks/ListenServer/listenserver.cpp \
        TcpTasks/ListenServer/onlistenconnection.cpp \
        TcpTasks/ListenServer/tcplisten.cpp \
        TcpTasks/SyncClient/onsyncconnect.cpp \
        TcpTasks/SyncClient/onsyncdisconnect.cpp \
        TcpTasks/SyncClient/onsyncread.cpp \
        TcpTasks/SyncClient/syncclient.cpp \
        TcpTasks/SyncClient/syncreplyquery.cpp \
        TcpTasks/SyncClient/updatesyncedserver.cpp \
        UdpTasks/BeaconServer/Receive/heartbeatgamespy0.cpp \
        UdpTasks/BeaconServer/Receive/replyquery.cpp \
        UdpTasks/BeaconServer/Receive/udponread.cpp \
        UdpTasks/BeaconServer/Receive/udpontimeout.cpp \
        UdpTasks/BeaconServer/Uplink/onuplinktimer.cpp \
        UdpTasks/BeaconServer/Uplink/uplink.cpp \
        UdpTasks/BeaconServer/beaconserver.cpp \
        UdpTasks/BeaconServer/udplisten.cpp \
        UdpTasks/StatusChecker/getnextserver.cpp \
        UdpTasks/StatusChecker/oncheckerresponseread.cpp \
        UdpTasks/StatusChecker/onticker.cpp \
        UdpTasks/StatusChecker/playerinfoinsert.cpp \
        UdpTasks/StatusChecker/serverinfoinsert.cpp \
        UdpTasks/StatusChecker/serverinfoupdate.cpp \
        UdpTasks/StatusChecker/statuschecker.cpp \
        UdpTasks/StatusChecker/statusticker.cpp \
        TcpTasks/Updater/onsynctickeraction.cpp \
        TcpTasks/Updater/scheduleupdater.cpp \
        TcpTasks/Updater/syncupdater.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Core/CoreObject/coreobject.h \
    Core/CoreObject/serverinfostructure.h \
    Core/GameInfo/gameinfostructure.h \
    Database/Common/commonactions.h \
    Database/databaseinterface.h \
    Core/GameInfo/loadsupportedgames.h \
    Logger/logger.h \
    Logger/logprimitive.h \
    Settings/loadsettings.h \
    Settings/settingstructure.h \
    Core/core.h \
    Core/version.h \
    Maintenance/maintenance.h \
    Protocols/GameSpy0/gamespy0.h \
    Protocols/GameSpy0/securevalidate.h \
    Protocols/overrides.h \
    TcpTasks/ListenClientHandler/listenclienthandler.h \
    TcpTasks/ListenServer/listenserver.h \
    TcpTasks/SyncClient/syncclient.h \
    UdpTasks/BeaconServer/beaconserver.h \
    UdpTasks/StatusChecker/statuschecker.h \
    UdpTasks/udpdatastructure.h \
    TcpTasks/Updater/syncupdater.h
