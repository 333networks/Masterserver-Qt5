#include "enctype2.h"
/* see enctype.h for proper creditations and remarks */

static unsigned int rotl32(unsigned int value, unsigned int count)
{
    return (value << count) | (value >> (-count & 31));
}

static unsigned int crypt_seek(GCryptInfo *info, unsigned int n1, unsigned int n2)
{
    int part;
    int i;
    int keyIndex;
    int xorKey;
    int current;

    current  = 0x8000;
    xorKey   = n1;
    keyIndex = 0;
    i        = 1;

    info->offset = 0;

    while (current > 0) {
        xorKey += i;
        keyIndex += xorKey;
        xorKey += keyIndex;

        if (n2 & current) {
            part   = rotl32(~xorKey, 24);
            xorKey = rotl32(info->key[part & 0xff] ^ part, 24);
            part   = rotl32(info->key[keyIndex & 0xff] ^ keyIndex, 8);
            xorKey ^= info->key[xorKey & 0xff];
            keyIndex = rotl32(info->key[part & 0xff] ^ part, 8);

            i += 1 + i;
        } else {
            info->msg[info->offset]      = xorKey;
            info->msg[info->offset + 16] = keyIndex;
            info->msg[info->offset + 32] = i;
            info->offset++;

            part     = rotl32(keyIndex, 24);
            keyIndex = rotl32(info->key[part & 0xff] ^ part, 24);
            part     = rotl32(info->key[xorKey & 0xff] ^ xorKey, 8);
            keyIndex ^= info->key[keyIndex & 0xff];
            xorKey = rotl32(info->key[part & 0xff] ^ part, 8);

            i *= 2;
        }

        current >>= 1;
    }

    info->xorKey   = xorKey;
    info->keyIndex = keyIndex;
    info->i        = i;
    info->start    = n1;

    return keyIndex ^ xorKey;
}

static void crypt_encrypt(GCryptInfo *info, unsigned int *words, int len)
{
    int          part;
    unsigned int i;
    unsigned int w;
    int          keyIndex;
    int          xorKey;
    int          offset;

    offset   = info->offset;
    xorKey   = info->xorKey;
    keyIndex = info->keyIndex;
    i        = info->i;

    for (w = 0; w < len; w++) {
        while (i < 0x10000) {
            xorKey += i;
            keyIndex += xorKey;
            xorKey += keyIndex;

            info->msg[offset]      = xorKey;
            info->msg[offset + 16] = keyIndex;
            info->msg[offset + 32] = i;
            offset++;

            part     = rotl32(keyIndex, 24);
            keyIndex = rotl32(info->key[part & 0xff] ^ part, 24);
            part     = rotl32(info->key[xorKey & 0xff] ^ xorKey, 8);
            keyIndex ^= info->key[keyIndex & 0xff];
            xorKey = rotl32(info->key[part & 0xff] ^ part, 8);

            i *= 2;
        }

        words[w] = keyIndex ^ xorKey;
        offset--;
        if (offset < 0) {
            offset = 0;
        }

        part   = rotl32(~info->msg[offset], 24);
        xorKey = rotl32(info->key[part & 0xff] ^ part, 24);
        part   = rotl32(info->key[info->msg[offset + 16] & 0xff] ^ info->msg[offset + 16], 8);
        xorKey ^= info->key[xorKey & 0xff];
        keyIndex = rotl32(info->key[part & 0xff] ^ part, 8);

        i = info->msg[offset + 32] + 1 + info->msg[offset + 32];
    }

    info->offset   = offset;
    info->xorKey   = xorKey;
    info->keyIndex = keyIndex;
    info->i        = i;
}

void init_crypt_key(unsigned char *src, unsigned int len, GCryptInfo *info)
{
    int index;
    int i, j, k;
    int tmp;

    info->wordPtr = nullptr;
    for (k = 0; k < 256; k++) {
        info->key[k] = 0;
    }

    for (i = 0; i < 4; i++) {
        for (k = 0; k < 256; k++) {
            info->key[k] = (info->key[k] << 8) + k;
        }

        index = i;

        for (j = 0; j < 2; j++) {
            for (k = 0; k < 256; k++) {
                index = ((info->key[k] & 0xff) + src[k % len] + index) & 0xff;

                tmp              = info->key[k];
                info->key[k]     = info->key[index];
                info->key[index] = tmp;
            }
        }
    }

    for (k = 0; k < 256; k++) {
        info->key[k] ^= k;
    }

    crypt_seek(info, 0, 0);
}

void crypt_docrypt(GCryptInfo *info, unsigned char *out, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        if (!info->wordPtr || ((char *)info->wordPtr - (char *)info->words) >= (sizeof(info->words) - 1)) {
            info->wordPtr = (unsigned char *)info->words;

            int lenw = sizeof(info->words) / sizeof(info->words[0]);
            crypt_encrypt(info, info->words, lenw);
        }

        out[i] ^= *info->wordPtr;
        info->wordPtr++;
    }
}

void encshare2(unsigned int *tbuff, unsigned int *tbuffp, int len) {
    unsigned int    t1,
        t2,
        t3,
        t4,
        t5,
        *limit,
        *p;

    t2 = tbuff[304];
    t1 = tbuff[305];
    t3 = tbuff[306];
    t5 = tbuff[307];
    limit = tbuffp + len;
    while(tbuffp < limit) {
        p = tbuff + t2 + 272;
        while(t5 < 65536) {
            t1 += t5;
            p++;
            t3 += t1;
            t1 += t3;
            p[-17] = t1;
            p[-1] = t3;
            t4 = (t3 << 24) | (t3 >> 8);
            p[15] = t5;
            t5 <<= 1;
            t2++;
            t1 ^= tbuff[t1 & 0xff];
            t4 ^= tbuff[t4 & 0xff];
            t3 = (t4 << 24) | (t4 >> 8);
            t4 = (t1 >> 24) | (t1 << 8);
            t4 ^= tbuff[t4 & 0xff];
            t3 ^= tbuff[t3 & 0xff];
            t1 = (t4 >> 24) | (t4 << 8);
        }
        t3 ^= t1;
        *tbuffp++ = t3;
        t2--;
        t1 = tbuff[t2 + 256];
        t5 = tbuff[t2 + 272];
        t1 = ~t1;
        t3 = (t1 << 24) | (t1 >> 8);
        t3 ^= tbuff[t3 & 0xff];
        t5 ^= tbuff[t5 & 0xff];
        t1 = (t3 << 24) | (t3 >> 8);
        t4 = (t5 >> 24) | (t5 << 8);
        t1 ^= tbuff[t1 & 0xff];
        t4 ^= tbuff[t4 & 0xff];
        t3 = (t4 >> 24) | (t4 << 8);
        t5 = (tbuff[t2 + 288] << 1) + 1;
    }
    tbuff[304] = t2;
    tbuff[305] = t1;
    tbuff[306] = t3;
    tbuff[307] = t5;
}



void encshare1(unsigned int *tbuff, unsigned char *datap, int len) {
    unsigned char   *p,
        *s;

    p = s = (unsigned char *)(tbuff + 309);
    encshare2(tbuff, (unsigned int *)p, 16);

    while(len--) {
        if((p - s) == 63) {
            p = s;
            encshare2(tbuff, (unsigned int *)p, 16);
        }
        *datap ^= *p;
        datap++;
        p++;
    }
}



void encshare3(unsigned int *data, int n1, int n2) {
    unsigned int    t1,
        t2,
        t3,
        t4;
    int             i;

    t2 = n1;
    t1 = 0;
    t4 = 1;
    data[304] = 0;
    for(i = 32768; i; i >>= 1) {
        t2 += t4;
        t1 += t2;
        t2 += t1;
        if(n2 & i) {
            t2 = ~t2;
            t4 = (t4 << 1) + 1;
            t3 = (t2 << 24) | (t2 >> 8);
            t3 ^= data[t3 & 0xff];
            t1 ^= data[t1 & 0xff];
            t2 = (t3 << 24) | (t3 >> 8);
            t3 = (t1 >> 24) | (t1 << 8);
            t2 ^= data[t2 & 0xff];
            t3 ^= data[t3 & 0xff];
            t1 = (t3 >> 24) | (t3 << 8);
        } else {
            data[data[304] + 256] = t2;
            data[data[304] + 272] = t1;
            data[data[304] + 288] = t4;
            data[304]++;
            t3 = (t1 << 24) | (t1 >> 8);
            t2 ^= data[t2 & 0xff];
            t3 ^= data[t3 & 0xff];
            t1 = (t3 << 24) | (t3 >> 8);
            t3 = (t2 >> 24) | (t2 << 8);
            t3 ^= data[t3 & 0xff];
            t1 ^= data[t1 & 0xff];
            t2 = (t3 >> 24) | (t3 << 8);
            t4 <<= 1;
        }
    }
    data[305] = t2;
    data[306] = t1;
    data[307] = t4;
    data[308] = n1;
    //    t1 ^= t2;
}



void encshare4(unsigned char *src, int size, unsigned int *dest) {
    unsigned int    tmp;
    int             i;
    unsigned char   pos,
        x,
        y;

    for(i = 0; i < 256; i++) dest[i] = 0;

    for(y = 0; y < 4; y++) {
        for(i = 0; i < 256; i++) {
            dest[i] = (dest[i] << 8) + i;
        }

        for(pos = y, x = 0; x < 2; x++) {
            for(i = 0; i < 256; i++) {
                tmp = dest[i];
                pos += tmp + src[i % size];
                dest[i] = dest[pos];
                dest[pos] = tmp;
            }
        }
    }

    for(i = 0; i < 256; i++) dest[i] ^= i;

    encshare3(dest, 0, 0);
}

unsigned char *enctype2_decoder(unsigned char *key, unsigned char *data, int *size) {
    unsigned int    dest[326];
    int             i;
    unsigned char   *datap;

    *data ^= 0xec;
    datap = data + 1;

    for(i = 0; key[i]; i++) datap[i] ^= key[i];

    for(i = 0; i < 326; i++) dest[i] = 0;
    if(*data) encshare4(datap, *data, dest);

    datap += *data;
    *size -= (*data + 1);
    if(*size < 6) {
        *size = 0;
        return(data);
    }

    encshare1(dest, datap, *size);

    *size -= 6;
    return(datap);
}



int enctype2_wrapper(unsigned char *key, unsigned char *data, int size) {
    unsigned char   *p;
    int     i;

    p = enctype2_decoder(key, data, &size);

    //memmove(data, p, size);   // memmove works but is untrusted
    if(p > data) {
        for(i = 0; i < size; i++) {
            data[i] = p[i];
        }
    }
    return(size);
}



/*
data must be big enough to contain also the following bytes: 1 + 8 + 6
*/
int enctype2_encoder(unsigned char *secKey, unsigned char *data, int size)
{
    GCryptInfo info = {0};
    int        i;
    int        header_size = 8;

    for (i = size - 1; i >= 0; i--) {
        data[1 + header_size + i] = data[i];
    }
    *data = header_size;

    for (i = 0; i < header_size; i++) {
        data[i + 1] = 0;
    }

    init_crypt_key(data + 1, header_size, &info);

    for (i = 0; i < 6; i++) {
        data[1 + header_size + size + i] = 0;
    }
    crypt_docrypt(&info, data + 1 + header_size, size + 6);

    for (i = 0; secKey[i]; i++) {
        data[i + 1] ^= secKey[i];
    }

    size += 1 + header_size + 6;
    *data ^= 0xec;
    return size;
}

