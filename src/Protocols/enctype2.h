#ifndef ENC2_H
#define ENC2_H

#include <string.h>

typedef struct {
    int key[256];
    int msg[48];

    int offset;
    int xorKey;
    int keyIndex;
    int i;
    int start;

    unsigned int   words[16];
    unsigned char *wordPtr;
} GCryptInfo;

void init_crypt_key(unsigned char *src, unsigned int len, GCryptInfo *info);
void crypt_docrypt(GCryptInfo *info, unsigned char *out, int len);

int enctype2_encoder(unsigned char *secKey, unsigned char *data, int size);

#endif // ENC2_H

/*
This file contains code from both Luigi Auriemma and the OpenMoHAA project. Code is used
with either direct permission of the author(s) and/or within the boundaries of the included license.


GS enctype2 servers list decoder/encoder 0.1.2
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    aluigi.org


INTRODUCTION
============
This is the algorithm used to decrypt the data sent by the Gamespy
master server (or any other compatible server) using the enctype 2
method.


USAGE
=====
Add the following prototype at the beginning of your code:

  unsigned char *enctype2_decoder(unsigned char *, unsigned char *, int *);

then use:

        pointer = enctype2_decoder(
            gamekey,        // the gamekey
            buffer,         // all the data received from the master server
            &buffer_len);   // the size of the master server

The return value is a pointer to the decrypted zone of buffer and
buffer_len is modified with the size of the decrypted data.

A simpler way to use the function is just using:

  len = enctype2_wrapper(key, data, data_len);


THANX TO
========
REC (http://www.backerstreet.com/rec/rec.htm) which has helped me in many
parts of the code.


LICENSE
=======
    Copyright 2004-2014 Luigi Auriemma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    http://www.gnu.org/licenses/gpl.txt


    ===========================================================================
    Copyright (C) 2025 the OpenMoHAA team

    This file is part of OpenMoHAA source code.

    OpenMoHAA source code is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the License,
    or (at your option) any later version.

    OpenMoHAA source code is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenMoHAA source code; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
    ===========================================================================
*/
