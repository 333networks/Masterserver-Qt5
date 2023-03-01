#include "overrides.h"

// some games use different key/value pairs. override with known key.
QString overrideKey(const QString &rawKey)
{
    // convert to lowercase
    QString key = rawKey.toLower();

    // some keys are indexed, like "player_0".
    if ( _index_match.indexIn(key) >= 0 )
    {
        // find keyword without index and match key
        if ( _validKeys.contains( _index_match.cap(1) ) ) // group starts at 1, not 0
        {
            // concat the index back and return found override
            return _validKeys.value( _index_match.cap(1), key ) + _index_match.cap(2);
        }
    }
    else
    {
        // non-indexed key
        if ( _validKeys.contains( key ) )
        {
            // return found override
            return _validKeys.value(key, key);
        }
    }

    // no override found. return original.
    return key;
}

// some games deviate from the gamename protocol. override with correct gamename
QString overrideGamename(const QString &gamenameIn)
{
    // convert to lowercase
    QString gamename = gamenameIn.toLower();

    if ( _validGamenames.contains(gamename ) )
    {
        return _validGamenames.value(gamename, gamename);
    }
    return gamename;
}
