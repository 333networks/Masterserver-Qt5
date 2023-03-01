#include "../beaconserver.h"

QStringList BeaconServer::replyQuery(const QMultiHash<QString, QString> &query)
{
    // initialise output
    QStringList queryResponse;

    // gamespy uses incrementing query ids in the messages
    _queryId = ( (_queryId > 99) ? 1 : _queryId + 1 );
    int querySubId = 1;

    // secure response
    if ( query.contains("secure") and _coreObject->SupportedGames.contains( TYPE_GAMENAME))
    {
        // sanity checks and cast to byte array
        QByteArray secure = query.value("secure", "").toLatin1();
        QByteArray cipher = _coreObject->SupportedGames.value(TYPE_GAMENAME).cipher.toLatin1();
        int enctype = query.value("enctype", "0").toInt();
        QString validate = returnValidate(cipher, secure, enctype);

        queryResponse.append(
                    QStringLiteral("\\validate\\%1\\queryid\\%2.%3")
                        .arg(validate, QString::number(_queryId), QString::number(querySubId++))
                );
    }

    // basic
    if ( query.contains("basic") or query.contains("status") )
    {
        queryResponse.append(
                    QStringLiteral("\\gamename\\%1"
                                   "\\gamever\\%2"
                                   "\\location\\0"
                                   "\\queryid\\%3.%4")
                        .arg(TYPE_GAMENAME, SHORT_VER, QString::number(_queryId), QString::number(querySubId++))
                );
    }

    // info
    if ( query.contains("info") or query.contains("status") )
    {
        // cast server statistics as player info
        QString selectStats = "SELECT SUM(num_direct) AS serv_direct, "
                                     "SUM(num_total)  AS serv_total "
                              "FROM gameinfo ";
        QSqlQuery statQuery;
        statQuery.prepare(selectStats);
        if ( ! statQuery.exec() )
            reportQuery(statQuery); // do NOT return/die!

        // get values
        int serv_direct = -1;
        int serv_total  = -1;
        if ( statQuery.next() )
        {
            serv_direct = statQuery.value("serv_direct").toInt();
            serv_total  = statQuery.value("serv_total").toInt();
        }

        queryResponse.append(
                    QStringLiteral("\\hostname\\%1"
                                   "\\hostport\\%2"
                                   "\\gametype\\%3"
                                   "\\mapname\\333networks"
                                   "\\numplayers\\%4"
                                   "\\maxplayers\\%5"
                                   "\\gamemode\\openplaying"
                                   "\\queryid\\%6.%7")
                        .arg( _coreObject->Settings.PublicInformationSettings.hostname,
                              QString::number(_coreObject->Settings.ListenServerSettings.listenPort),
                              "Masterserver",   // replace with "bare masterserver" or "integrated beacon/website checker", based on the settings
                              QString::number(serv_direct),
                              QString::number(serv_total),
                              QString::number(_queryId), QString::number(querySubId++))
                );
    }

    // rules
    if ( query.contains("rules") or query.contains("status") )
    {
        // compile information about the specific software
        QString mutators;
        mutators.append( QStringLiteral("buildtype: %1, buildtime: %2, Qt version: %3, author: %4")
                         .arg(BUILD_TYPE,
                              BUILD_TIME,
                              qVersion(),
                              BUILD_AUTHOR) );

        queryResponse.append(
                    QStringLiteral("\\mutators\\%1"
                                   "\\AdminName\\%2"
                                   "\\AdminEMail\\%3"
                                   "\\queryid\\%4.%5")
                        .arg( mutators,
                              _coreObject->Settings.PublicInformationSettings.adminName,
                              _coreObject->Settings.PublicInformationSettings.contact,
                              QString::number(_queryId), QString::number(querySubId++))
                );
    }

    // echo: reply with echo_reply
    if (query.contains("echo"))
    {
        queryResponse.append(QStringLiteral("\\echo_reply\\%1\\queryid\\%2.%3")
                             .arg(query.value("echo"), QString::number(_queryId), QString::number(querySubId++))
                );
    }

    // end query with final
    queryResponse.append("\\final\\");

    return queryResponse;
}
