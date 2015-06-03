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

#include "xxtea.h"
#include <memory.h>
#include <stdlib.h>

static void xxtea_long_encrypt(xxtea_long *v, xxtea_long len, xxtea_long *k)
{
    xxtea_long n = len - 1;
    xxtea_long z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = 0, e;
    if (n < 1) {
        return;
    }
    while (0 < q--) {
        sum += XXTEA_DELTA;
        e = sum >> 2 & 3;
        for (p = 0; p < n; p++) {
            y = v[p + 1];
            z = v[p] += XXTEA_MX;
        }
        y = v[0];
        z = v[n] += XXTEA_MX;
    }
}

static void xxtea_long_decrypt(xxtea_long *v, xxtea_long len, xxtea_long *k)
{
    xxtea_long n = len - 1;
    xxtea_long z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = q * XXTEA_DELTA, e;
    if (n < 1) {
        return;
    }
    while (sum != 0) {
        e = sum >> 2 & 3;
        for (p = n; p > 0; p--) {
            z = v[p - 1];
            y = v[p] -= XXTEA_MX;
        }
        z = v[n];
        y = v[0] -= XXTEA_MX;
        sum -= XXTEA_DELTA;
    }
}

static xxtea_byte *fix_key_length(const xxtea_byte *key, xxtea_long key_len)
{
    xxtea_byte *tmp = (xxtea_byte *)malloc(16);
    memcpy(tmp, key, key_len);
    memset(tmp + key_len, '\0', 16 - key_len);
    return tmp;
}

static xxtea_long *xxtea_to_long_array(const xxtea_byte *data, xxtea_long len, int include_length, xxtea_long *ret_len) {
    xxtea_long i, n, *result;
    
    n = len >> 2;
    n = (((len & 3) == 0) ? n : n + 1);
    if (include_length) {
        result = (xxtea_long *)malloc((n + 1) << 2);
        result[n] = len;
        *ret_len = n + 1;
    } else {
        result = (xxtea_long *)malloc(n << 2);
        *ret_len = n;
    }
    memset(result, 0, n << 2);
    for (i = 0; i < len; i++) {
        result[i >> 2] |= (xxtea_long)data[i] << ((i & 3) << 3);
    }
    
    return result;
}

static xxtea_byte *xxtea_to_byte_array(const xxtea_long *data, xxtea_long len, int include_length, xxtea_long *ret_len) {
    xxtea_long i, n, m;
    xxtea_byte *result;
    
    n = len << 2;
    if (include_length) {
        m = data[len - 1];
        if ((m < n - 7) || (m > n - 4)) return NULL;
        n = m;
    }
    result = (xxtea_byte *)malloc(n + 1);
    for (i = 0; i < n; i++) {
        result[i] = (xxtea_byte)((data[i >> 2] >> ((i & 3) << 3)) & 0xff);
    }
    result[n] = '\0';
    *ret_len = n;
    
    return result;
}

static xxtea_byte *do_xxtea_encrypt(const xxtea_byte *data, xxtea_long len, const xxtea_byte *key, xxtea_long *ret_len) {
    xxtea_byte *result;
    xxtea_long *v, *k, v_len, k_len;
    
    v = xxtea_to_long_array(data, len, 1, &v_len);
    k = xxtea_to_long_array(key, 16, 0, &k_len);
    xxtea_long_encrypt(v, v_len, k);
    result = xxtea_to_byte_array(v, v_len, 0, ret_len);
    free(v);
    free(k);
    
    return result;
}

static xxtea_byte *do_xxtea_decrypt(const xxtea_byte *data, xxtea_long len, const xxtea_byte *key, xxtea_long *ret_len) {
    xxtea_byte *result;
    xxtea_long *v, *k, v_len, k_len;
    
    v = xxtea_to_long_array(data, len, 0, &v_len);
    k = xxtea_to_long_array(key, 16, 0, &k_len);
    xxtea_long_decrypt(v, v_len, k);
    result = xxtea_to_byte_array(v, v_len, 1, ret_len);
    free(v);
    free(k);
    
    return result;
}

xxtea_byte *xxtea_encrypt(const xxtea_byte *data, xxtea_long data_len, const xxtea_byte *key, xxtea_long key_len, xxtea_long *ret_length)
{
    xxtea_byte *result;
    
    *ret_length = 0;
    
    if (key_len < 16) {
        xxtea_byte *key2 = fix_key_length(key, key_len);
        result = do_xxtea_encrypt(data, data_len, key2, ret_length);
        free(key2);
    }
    else
    {
        result = do_xxtea_encrypt(data, data_len, key, ret_length);
    }
    
    return result;
}

xxtea_byte *xxtea_decrypt(const xxtea_byte *data, xxtea_long data_len, const xxtea_byte *key, xxtea_long key_len, xxtea_long *ret_length)
{
    xxtea_byte *result;
    
    *ret_length = 0;
    
    if (key_len < 16) {
        xxtea_byte *key2 = fix_key_length(key, key_len);
        result = do_xxtea_decrypt(data, data_len, key2, ret_length);
        free(key2);
    }
    else
    {
        result = do_xxtea_decrypt(data, data_len, key, ret_length);
    }
    
    return result;
}

/* }}} */
