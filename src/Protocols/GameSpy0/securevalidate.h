#ifndef SECUREVALIDATE_H
#define SECUREVALIDATE_H

#include <QString>
#include "Protocols/overrides.h"

// return both status and result string
struct AuthResult {
    bool    auth     = false;
    QString validate = "";
};

// authenticate the beacon/client and return the validate string
AuthResult validateGamename(const bool    &isBeacon,
                            const QString &gamename,
                            const QString &validate,
                            const QString &cipherIn,
                            const QString &secureIn,
                            const int     &enctypeIn);

// return validate string (acccepts unsanitised inputs)
QString returnValidate(const QByteArray &cipher,
                       const QByteArray &secure,
                       const int        &enctype);

// generate random challenge strings
QString genChallengeString(const int len, const bool moreChars);


// algorithm for the secure/validate challenge
unsigned char charshift ( const unsigned char &reg );
QString generateValidateString(const QByteArray &cipher,
                               const QByteArray &secure,
                               const int        &enctype);

// pre-built characters for generating validate string
const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const QString moreCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

/* Pre-built algorithm data for enctype 1 from Luigi Auriemma's
 * gsmsalg 0.3.3, https://aluigi.altervista.org/papers/gsmsalg.h
 * under GNU General Public License v2.
 */
const unsigned char enctype1_data[] = (
"\x01\xba\xfa\xb2\x51\x00\x54\x80\x75\x16\x8e\x8e\x02\x08\x36\xa5"
"\x2d\x05\x0d\x16\x52\x07\xb4\x22\x8c\xe9\x09\xd6\xb9\x26\x00\x04"
"\x06\x05\x00\x13\x18\xc4\x1e\x5b\x1d\x76\x74\xfc\x50\x51\x06\x16"
"\x00\x51\x28\x00\x04\x0a\x29\x78\x51\x00\x01\x11\x52\x16\x06\x4a"
"\x20\x84\x01\xa2\x1e\x16\x47\x16\x32\x51\x9a\xc4\x03\x2a\x73\xe1"
"\x2d\x4f\x18\x4b\x93\x4c\x0f\x39\x0a\x00\x04\xc0\x12\x0c\x9a\x5e"
"\x02\xb3\x18\xb8\x07\x0c\xcd\x21\x05\xc0\xa9\x41\x43\x04\x3c\x52"
"\x75\xec\x98\x80\x1d\x08\x02\x1d\x58\x84\x01\x4e\x3b\x6a\x53\x7a"
"\x55\x56\x57\x1e\x7f\xec\xb8\xad\x00\x70\x1f\x82\xd8\xfc\x97\x8b"
"\xf0\x83\xfe\x0e\x76\x03\xbe\x39\x29\x77\x30\xe0\x2b\xff\xb7\x9e"
"\x01\x04\xf8\x01\x0e\xe8\x53\xff\x94\x0c\xb2\x45\x9e\x0a\xc7\x06"
"\x18\x01\x64\xb0\x03\x98\x01\xeb\x02\xb0\x01\xb4\x12\x49\x07\x1f"
"\x5f\x5e\x5d\xa0\x4f\x5b\xa0\x5a\x59\x58\xcf\x52\x54\xd0\xb8\x34"
"\x02\xfc\x0e\x42\x29\xb8\xda\x00\xba\xb1\xf0\x12\xfd\x23\xae\xb6"
"\x45\xa9\xbb\x06\xb8\x88\x14\x24\xa9\x00\x14\xcb\x24\x12\xae\xcc"
"\x57\x56\xee\xfd\x08\x30\xd9\xfd\x8b\x3e\x0a\x84\x46\xfa\x77\xb8");

#endif // SECUREVALIDATE_H
