#include "syncclient.h"

QStringList SyncClient::replyQuery(const QMultiHash<QString,  QString> &query)
{
    // initialise output
    QStringList queryResponse;

    // gamespy uses incrementing query ids in the messages
    _queryId = ( (_queryId > 99) ? 1 : _queryId + 1 );
    int querySubId = 1;

    // secure response
    if ( query.contains("secure") and _coreObject->SupportedGames.contains(TYPE_GAMENAME))
    {
        // sanity checks
        QByteArray secure = query.value("secure", "").toLatin1();
        QByteArray cipher =      _coreObject->SupportedGames.value(TYPE_GAMENAME).cipher.toLatin1();
        int enctype = query.value("enctype", "0").toInt();
        QString validate = returnValidate(cipher, secure, enctype);

        queryResponse.append(
                    QStringLiteral("\\validate\\%1\\queryid\\%2.%3")
                        .arg(validate, QString::number(_queryId), QString::number(querySubId++))
                );
    }

    // basic
    if ( query.contains("basic") )
    {
        queryResponse.append(
                    QStringLiteral("\\gamename\\%1"
                                   "\\gamever\\%2"
                                   "\\location\\0"
                                   "\\queryid\\%3.%4")
                        .arg(TYPE_GAMENAME,
                             SHORT_VER,
                             QString::number(_queryId),
                             QString::number(querySubId++)
                             )
                );
    }



    // end query with final
    queryResponse.append("\\final\\");

    return queryResponse;
}
