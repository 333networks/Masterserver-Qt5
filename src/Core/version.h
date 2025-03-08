#ifndef VERSION_H
#define VERSION_H

#include <QString>

/*
 * Version and author information
 *
 * Only when you make (significant) modifications to the master server source
 * code, you should edit these variables to reflect your changes.
 *
 * For example,
 * if you limit functionality to only one game, you should change the variable
 * BUILD_TYPE and SHORT_VER to something that reflects the change in function.
 *
 * In addition, if you have the actual interest to go through all this source
 * code to end up here, consider sending Darkelarious (the original author) a
 * postcard or (e)mail with your compliments. Or buy us a coffee. We like the
 * appreciation.
 */

// gamename for the 333networks-type MasterServer
#define TYPE_GAMENAME QString("333networks")

// build type: type of software
#define BUILD_TYPE QString("MasterServer Qt5")

// software version (of this particular type)
#define BUILD_VERSION QString("0.27")

// short version (in query) -- Qt v0.n
#define SHORT_VER QString("Qt-" + BUILD_VERSION + "hf4")

// build time/date
#define BUILD_TIME QStringLiteral("%1 %2").arg(__DATE__).arg(__TIME__)

// software author, contact
#define BUILD_AUTHOR QString("Darkelarious <darkelarious@333networks.com>")

// minimum required database version
#define DATABASE_VERSION 0.27

#endif // VERSION_H
