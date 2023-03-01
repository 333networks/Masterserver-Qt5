#include "listenclienthandler.h"

QByteArray ListenClientHandler::compileSyncList(const QStringList &gamenameList,
                                                const int         &serverAge_s)
{
    // output list in \\gamename1\\ip:port ip:port\\gamename2\\ip:port ip:port\\final\\ format
    QByteArray compiledList;

    // go through list of gamenames
    QStringListIterator gamenameListNames(gamenameList);
    while ( gamenameListNames.hasNext() )
    {
        // retrieve servers from database (only verified, not from other syncs)
        QString gamename = gamenameListNames.next();
        QSqlQuery q = selectServerList( gamename, serverAge_s, false);

        // identifier
        compiledList += QStringLiteral("\\%1\\").arg(gamename);

        // iterate through resulting queries and add to compile list
        // ip-addresses are stored as text and do not need to be converted back and forth
        while (q.next())
        {
            // add to list -> ip:port
            compiledList += QStringLiteral("%1:%2 ").arg(q.value(0).toString(), q.value(1).toString());

        } // while next
    }

    // terminator after list
    compiledList.append("\\final\\");

    // list compiled
    return compiledList;
}
