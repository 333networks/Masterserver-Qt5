#include "listenclienthandler.h"

QByteArray ListenClientHandler::compileServerlist (const QString &gamename,
                                                   const int     &serverAge_s,
                                                   const bool    &cmp)
{
    // retrieve servers from database (both direct and sync)
    QSqlQuery q = selectServerList( gamename, serverAge_s, true);

    // output array
    QByteArray compiledList;
    QDataStream dsList(&compiledList,QIODevice::WriteOnly);

    // iterate through resulting queries and add to compile list
    while (q.next())
    {
        QString        ip   = q.value(0).toString();
        unsigned short port = q.value(1).value<unsigned short>();

        // add server to list (compressed/cmp or plaintext)
        if (cmp)
        {
            // QHostAddress.toIPv4Address() provides the correct ABCD format, append EF port bytes
            dsList << QHostAddress(ip).toIPv4Address() << port;
        }
        else // plaintext output
        {
            // ip in database is plaintext already
            compiledList += QStringLiteral("\\ip\\%1:%2").arg(ip, QString::number(port));
        }

    } // while next

    // terminator after list
    compiledList.append("\\final\\");

    // list compiled
    return compiledList;
}
