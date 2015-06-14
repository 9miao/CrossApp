//
//  CACrypto.cpp
//  AppGift
//
//  Created by tanjie on 15/6/3.
//
//

#include "CACrypto.h"

extern "C" {
#include "crypto/base64/libbase64.h"
#include "crypto/xxtea/xxtea.h"
#include "crypto/md5/md5.h"
}

NS_CC_EXT_BEGIN

unsigned char* CACrypto::encryptXXTEA(unsigned char* plaintext,
                                      int plaintextLength,
                                      unsigned char* key,
                                      int keyLength,
                                      int* resultLength)
{
    xxtea_long len;
    unsigned char* result = xxtea_encrypt(plaintext, (xxtea_long)plaintextLength, key, (xxtea_long)keyLength, &len);
    *resultLength = (int)len;
    return result;
}

unsigned char* CACrypto::decryptXXTEA(unsigned char* ciphertext,
                                      int ciphertextLength,
                                      unsigned char* key,
                                      int keyLength,
                                      int* resultLength)
{
    xxtea_long len;
    unsigned char* result = xxtea_decrypt(ciphertext, (xxtea_long)ciphertextLength, key, (xxtea_long)keyLength, &len);
    *resultLength = (int)len;
    return result;
}

int CACrypto::encodeBase64Len(const char* input, int inputLength)
{
    return Base64encode_len(inputLength);
}

int CACrypto::encodeBase64(const char* input,
                           int inputLength,
                           char* output,
                           int outputBufferLength)
{
    CCAssert(Base64encode_len(inputLength) <= outputBufferLength, "CACrypto::encodeBase64() - outputBufferLength too small");
    return Base64encode(output, input, inputLength);
}

int CACrypto::decodeBase64Len(const char* input)
{
    return Base64decode_len(input);
}

int CACrypto::decodeBase64(const char* input,
                           char* output,
                           int outputBufferLength)
{
    CCAssert(Base64decode_len(input) <= outputBufferLength, "CACrypto::decodeBase64() - outputBufferLength too small");
    return Base64decode(output, input);
}

void CACrypto::MD5(void* input, int inputLength, unsigned char* output)
{
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, input, inputLength);
    MD5_Final(output, &ctx);
}

void CACrypto::MD5File(const char* path, unsigned char* output)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
        return;
    
    MD5_CTX ctx;
    MD5_Init(&ctx);
    
    size_t i;
    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    while ((i = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        MD5_Update(&ctx, buffer, (unsigned) i);
    }
    
    fclose(file);
    MD5_Final(output, &ctx);
}

const string CACrypto::MD5String(void* input, int inputLength)
{
    unsigned char buffer[MD5_BUFFER_LENGTH];
    MD5(static_cast<void*>(input), inputLength, buffer);
    
    char* hex = bin2hex(buffer, MD5_BUFFER_LENGTH);
    string ret(hex);
    delete[] hex;
    return ret;
}

char *CACrypto::bin2hex(unsigned char* bin, int binLength)
{
    static const char* hextable = "0123456789abcdef";
    
    int hexLength = binLength * 2 + 1;
    char* hex = new char[hexLength];
    memset(hex, 0, sizeof(char) * hexLength);
    
    int ci = 0;
    for (int i = 0; i < binLength; ++i)
    {
        unsigned char c = bin[i];
        hex[ci++] = hextable[(c >> 4) & 0x0f];
        hex[ci++] = hextable[c & 0x0f];
    }
    
    return hex;
}

char *CACrypto::hex2bin(char* hex, int hexLength)
{
    if (hexLength % 2) {
        CCLog("hexLength should be even number.");
        return NULL;
    }
    
    int binLength = hexLength / 2 + 1;
    char *bin = new char[binLength];
    memset(bin, 0, sizeof(char) * binLength);
    
    int ci = 0;
    for (int i = 0; i < hexLength; i += 2) {
        char high = hex[i];
        char low = hex[i+1];
        
        if (high >= '0' && high <= '9') {
            high = high - '0';
        } else if (high >= 'A' && high <= 'F') {
            high = high - 'A' + 10;
        } else if (high >= 'a' && high <= 'f') {
            high = high - 'a' + 10;
        } else {
            assert(0);
            high = 0;
        }
        
        if (low >= '0' && low <= '9') {
            low = low - '0';
        } else if (low >= 'A' && low <= 'F') {
            low = low - 'A' + 10;
        } else if (low >= 'a' && low <= 'f') {
            low = low - 'a' + 10;
        } else {
            assert(0);
            low = 0;
        }
        
        bin[ci++] = (high << 4) | (low);
    }
    
    return bin;
}


NS_CC_EXT_END