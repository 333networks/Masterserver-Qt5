#include "securevalidate.h"

AuthResult validateGamename(const bool    &isBeacon,
                            const QString &gamename,
                            const QString &validate,
                            const QString &cipherIn,
                            const QString &secureIn,
                            const int     &enctypeIn)
{
    // output result
    AuthResult authResult;

    // override certain cases for UDP beacon
    if ( isBeacon and _overrideValidateBeacon.contains(gamename) )
    {
        authResult.auth = true;
        authResult.validate = "override";
        return authResult;
    }

    // override certain cases for TCP client
    if ( ! isBeacon and _overrideValidateClient.contains(gamename) )
    {
        authResult.auth = true;
        authResult.validate = "override";
        return authResult;
    }

    // inputs + checks
    QByteArray secure = secureIn.toLatin1();
    QByteArray cipher = cipherIn.toLatin1();

    // get validate value
    if ( 6 <= cipher.length() and cipher.length() < 16 and
         6 <= secure.length() and secure.length() < 16 )
    {
        // safe to call validation. proceed.
        authResult.validate = generateValidateString(cipher, secure, enctypeIn);
    }
    else
    {
        // incorrect input, not safe to calculate validation
        authResult.auth = false;
        authResult.validate = "invalid!";
    }

    // correct validation provided?
    authResult.auth = (authResult.validate.compare(validate) == 0);

    // return result as boolean and string
    return authResult;
}

QString returnValidate(const QByteArray &cipher,
                       const QByteArray &secure,
                       const int        &enctype)
{
    // get validate value
    if ( 6 <= cipher.length() and cipher.length() < 16 and
         6 <= secure.length() and secure.length() < 16 )
    {
        return generateValidateString(cipher, secure, enctype);
    }
    else
    {
        return "invalid!";
    }
}

QString genChallengeString(const int len, const bool moreChars)
{
    QString randomString;
    if (moreChars)
    {
        // use A-Za-z0-9
        for(unsigned char i = 0; i < len; ++i)
        {
            randomString += moreCharacters[qrand() % moreCharacters.length()];
        }
    }
    else
    {
        // use A-Z only
        for(unsigned char i = 0; i < len; ++i)
        {
            randomString += possibleCharacters[qrand() % possibleCharacters.length()];
        }
    }
    return randomString;
}
