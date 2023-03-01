#ifndef STRUCTGAMEINFO_H
#define STRUCTGAMEINFO_H

#include <QString>

struct GameInfo
{
    // gamename is the unique identifier
    QString gamename;

    // 6-byte GameSpy identifier
    QString cipher;

    // game label
    QString label;

    // default port
    unsigned short port = 0;

    // known protocol
    QString protocol;
};

#endif // STRUCTGAMEINFO_H
