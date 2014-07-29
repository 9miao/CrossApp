

#include <stdio.h>
#include <stdlib.h>
#include "base64.h"

namespace CrossApp {

unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int _base64Decode( unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len );

int _base64Decode( unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len )
{
    static char inalphabet[256], decoder[256];
    int i, bits, c = 0, char_count, errors = 0;
    unsigned int input_idx = 0;
    unsigned int output_idx = 0;

    for (i = (sizeof alphabet) - 1; i >= 0 ; i--) {
        inalphabet[alphabet[i]] = 1;
        decoder[alphabet[i]] = i;
    }

    char_count = 0;
    bits = 0;
    for( input_idx=0; input_idx < input_len ; input_idx++ ) {
        c = input[ input_idx ];
        if (c == '=')
            break;
        if (c > 255 || ! inalphabet[c])
            continue;
        bits += decoder[c];
        char_count++;
        if (char_count == 4) {
            output[ output_idx++ ] = (bits >> 16);
            output[ output_idx++ ] = ((bits >> 8) & 0xff);
            output[ output_idx++ ] = ( bits & 0xff);
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 6;
        }
    }
    
    if( c == '=' ) {
        switch (char_count) {
            case 1:
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
                std::fprintf(stderr, "base64Decode: encoding incomplete: at least 2 bits missing");
#endif
                errors++;
                break;
            case 2:
                output[ output_idx++ ] = ( bits >> 10 );
                break;
            case 3:
                output[ output_idx++ ] = ( bits >> 16 );
                output[ output_idx++ ] = (( bits >> 8 ) & 0xff);
                break;
            }
    } else if ( input_idx < input_len ) {
        if (char_count) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
            std::fprintf(stderr, "base64 encoding incomplete: at least %d bits truncated",
                    ((4 - char_count) * 6));
#endif
            errors++;
        }
    }
    
    *output_len = output_idx;
    return errors;
}

int base64Decode(unsigned char *in, unsigned int inLength, unsigned char **out)
{
    unsigned int outLength = 0;
    
    //should be enough to store 6-bit buffers in 8-bit buffers
    *out = new unsigned char[(size_t)(inLength * 3.0f / 4.0f + 1)];
    if( *out ) {
        int ret = _base64Decode(in, inLength, *out, &outLength);
        
        if (ret > 0 )
        {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
            printf("Base64Utils: error decoding");
#endif
            delete [] *out;
            *out = NULL;            
            outLength = 0;
        }
    }
    return outLength;
}

}//namespace   CrossApp 
