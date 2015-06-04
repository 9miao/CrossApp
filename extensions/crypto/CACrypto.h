//
//  CACrypto.h
//  AppGift
//
//  Created by tanjie on 15/6/3.
//
//

#ifndef __AppGift__CACrypto__
#define __AppGift__CACrypto__

#include <stdio.h>

#include <iostream>
#include "CrossApp.h"
#include "CrossAppExt.h"

NS_CC_EXT_BEGIN

class CACrypto
{
public:
    static const int MD5_BUFFER_LENGTH = 16;
    
    /** @brief Encrypt data with XXTEA algorithm, return ciphertext, free ciphertext after used */
    static unsigned char* encryptXXTEA(unsigned char* plaintext,
                                       int plaintextLength,
                                       unsigned char* key,
                                       int keyLength,
                                       int* resultLength);
    
    /** @brief Decrypt data with XXTEA algorithm, return plaintext, free plaintext after used */
    static unsigned char* decryptXXTEA(unsigned char* ciphertext,
                                       int ciphertextLength,
                                       unsigned char* key,
                                       int keyLength,
                                       int* resultLength);
    
    /** @brief Get length of encoding data with Base64 algorithm */
    static int encodeBase64Len(const char* input, int inputLength);
    
    /** @brief Encoding data with Base64 algorithm, return encoded string length */
    static int encodeBase64(const char* input, int inputLength,
                            char* output, int outputBufferLength);
    
    /** @brief Get length of Decoding Base 64 */
    static int decodeBase64Len(const char* input);
    
    /** @brief Decoding Base64 string to data, return decoded data length */
    static int decodeBase64(const char* input,
                            char* output, int outputBufferLength);
    
    /** @brief Calculate MD5, get MD5 code (not string) */
    static void MD5(void* input, int inputLength,
                    unsigned char* output);
    
    static void MD5File(const char* path, unsigned char* output);
    
    
    
    static const string MD5String(void* input, int inputLength);
    
    static char *bin2hex(unsigned char* bin, int binLength);
    static char *hex2bin(char* hex, int hexLength);
#pragma mark -
#pragma mark private methods
    
private:
    CACrypto(void) {}
};

NS_CC_EXT_END

#endif /* defined(__AppGift__CACrypto__) */
