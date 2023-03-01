#include "securevalidate.h"

/* This algorithm is based on Luigi Auriemma's gsmsalg 0.3.3,
 * https://aluigi.altervista.org/papers/gsmsalg.h
 * under GNU General Public License v2.
 */
QString generateValidateString(const QByteArray &cipher,
                               const QByteArray &secure,
                               const int        &enctype)
{
    unsigned char cipherLength = static_cast<unsigned char>( cipher.length() );
    unsigned char secureLength = static_cast<unsigned char>( secure.length() );

    // fill array with ascii characters
    unsigned char enc[256];
    for (unsigned short j = 0; j < 256; j++)
    {
        enc[j] = static_cast<unsigned char>(j);
    }

    // cipher shuffle
    unsigned char a = 0, x;
    for (unsigned short j = 0; j < 256; j++)
    {
        a += enc[j] + cipher[j % cipherLength];
        x  = enc[a];
        enc[a] = enc[j];
        enc[j] = x;
    }

    // secure shuffle
    unsigned char tmp[66];
    unsigned char i = 0,
                  y = 0,
                  b = 0;
                  a = 0; // reset a
    for (i = 0; i < cipherLength; i++)
    {
        a += secure.at(i) + 1;
        x = enc[a];
        b += x;
        y = enc[b];
        enc[b] = x;
        enc[a] = y;
        tmp[i] = static_cast<unsigned char>( secure.at(i) ^ enc[ (x+y) & 0xff ] );
    }

    // part of the enctype 1-2 process (uses i from previous loop)
    for (secureLength = i; secureLength % 3; secureLength++)
    {
        tmp[secureLength] = 0;
    }

    // enctype 1 shuffle
    if (enctype == 1)
    {
        for (i = 0; i < secureLength; i++)
        {
            tmp[i] = enctype1_data[ tmp[i] ];
        }
    }
    else if (enctype == 2 )
    {
        for (i = 0; i < secureLength; i++)
        {
            tmp[i] = static_cast<unsigned char>(tmp[i] ^ cipher[i % cipherLength] );
        }
    }

    // final shuffle and stitch validate response together
    unsigned char z = 0;
    QString validate;
    for (i = 0; i < secureLength; i += 3)
    {
        x = (tmp[i]);
        y = (tmp[i+1]);
        z = (tmp[i+2]);

        validate.append( charshift (x >> 2) );
        validate.append( charshift (static_cast<unsigned char>(((x & 3)  << 4) | (y >> 4)) ));
        validate.append( charshift (static_cast<unsigned char>(((y & 15) << 2) | (z >> 6)) ));
        validate.append( charshift (z & 63));
    }
    return validate;
}

// part of gsmsalg 0.3.3 and license
unsigned char charshift ( const unsigned char &reg )
{
    if (reg <  26) return (reg + 'A');
    if (reg <  52) return (reg + 'G');
    if (reg <  62) return (reg - 4);
    if (reg == 62) return ('+');
    if (reg == 63) return ('/');
    return (0);
}
