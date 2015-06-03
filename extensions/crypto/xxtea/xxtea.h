/***********************************************************************
 
 Copyright 2006-2009 Ma Bingyao
 Copyright 2013 Gao Chunhui, Liu Tao
 
 These sources is free software. Redistributions of source code must
 retain the above copyright notice. Redistributions in binary form
 must reproduce the above copyright notice. You can redistribute it
 freely. You can use it with any free or commercial software.
 
 These sources is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY. Without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 
 github: https://github.com/liut/pecl-xxtea
 
 *************************************************************************/

#ifndef XXTEA_H
#define XXTEA_H

#include <stddef.h> /* for size_t & NULL declarations */

#if defined(_MSC_VER)

typedef unsigned __int32 xxtea_long;

#else

#if defined(__FreeBSD__) && __FreeBSD__ < 5
/* FreeBSD 4 doesn't have stdint.h file */
#include <inttypes.h>
#else
#include <stdint.h>
#endif

typedef uint32_t xxtea_long;
typedef unsigned char xxtea_byte;

#endif /* end of if defined(_MSC_VER) */

#define XXTEA_MX (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z)
#define XXTEA_DELTA 0x9e3779b9

#ifdef __cplusplus
extern "C" {
#endif

xxtea_byte *xxtea_encrypt(const xxtea_byte *data, xxtea_long data_len, const xxtea_byte *key, xxtea_long key_len, xxtea_long *ret_length);
xxtea_byte *xxtea_decrypt(const xxtea_byte *data, xxtea_long data_len, const xxtea_byte *key, xxtea_long key_len, xxtea_long *ret_length);

#ifdef __cplusplus
}
#endif

#endif
