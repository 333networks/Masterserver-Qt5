#include "gamespy0.h"

QMultiHash<QString, QString> parseGameSpy0Buffer(const QString &bufferString)
{
    // initialise output hash
    QMultiHash<QString, QString> queryStringHash;

    // split on backslash
    QStringList bufferStringList = bufferString.split('\\', QString::KeepEmptyParts);

    // iterate through all items
    QListIterator<QString> property (bufferStringList);

    // the first element is always empty -- skip it
    if ( property.hasNext() )
    {
        property.next();
    }

    // store as key -> value
    while ( property.hasNext() )
    {
        // unify valid keys
        QString key = overrideKey( property.next().trimmed() );

        // see if a value for this key exists
        if ( ! property.hasNext() )
            break;

        // get value
        QString value = property.next().trimmed();

        // insert to return hash
        queryStringHash.insert(key, value);
    }

    // override gamename
    if ( queryStringHash.contains("gamename") )
    {
        QList<QString> gn = queryStringHash.values("gamename");
        queryStringHash.remove("gamename");

        // read backwards to preserve element order
        for (int i = gn.size()-1; i >= 0; i--)
        {
            queryStringHash.insert("gamename", overrideGamename( gn.value(i) ) );
        }
    }

    return queryStringHash;
}
