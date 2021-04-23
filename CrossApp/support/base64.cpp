

#include <stdio.h>
#include <stdlib.h>
#include "base64.h"
#include <string.h>

namespace CrossApp
{
    
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    int _base64Decode(const char *input, int input_len, char *output, int *output_len )
    {
        static char inalphabet[256], decoder[256];
        int i, bits, c = 0, char_count, errors = 0;
        int input_idx = 0;
        int output_idx = 0;
        
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
                    fprintf(stderr, "base64Decode: encoding incomplete: at least 2 bits missing");
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
                fprintf(stderr, "base64 encoding incomplete: at least %d bits truncated",
                        ((4 - char_count) * 6));
#endif
                errors++;
            }
        }
        
        *output_len = output_idx;
        return errors;
    }
    
    void _base64Encode( const char *input, int input_len, char *output )
    {
        int char_count;
        int bits;
        int input_idx = 0;
        int output_idx = 0;
        
        char_count = 0;
        bits = 0;
        for( input_idx=0; input_idx < input_len ; input_idx++ ) {
            bits |= input[ input_idx ];
            
            char_count++;
            if (char_count == 3) {
                output[ output_idx++ ] = alphabet[(bits >> 18) & 0x3f];
                output[ output_idx++ ] = alphabet[(bits >> 12) & 0x3f];
                output[ output_idx++ ] = alphabet[(bits >> 6) & 0x3f];
                output[ output_idx++ ] = alphabet[bits & 0x3f];
                bits = 0;
                char_count = 0;
            } else {
                bits <<= 8;
            }
        }
        
        if (char_count) {
            if (char_count == 1) {
                bits <<= 8;
            }
            
            output[ output_idx++ ] = alphabet[(bits >> 18) & 0x3f];
            output[ output_idx++ ] = alphabet[(bits >> 12) & 0x3f];
            if (char_count > 1) {
                output[ output_idx++ ] = alphabet[(bits >> 6) & 0x3f];
            } else {
                output[ output_idx++ ] = '=';
            }
            output[ output_idx++ ] = '=';
        }
        
        output[ output_idx++ ] = 0;
    }
    
    std::string base64Decode(const std::string& string)
    {
        std::string result;
        
        int outLength = 0;
        //should be enough to store 6-bit buffers in 8-bit buffers
        char* out = (char*)malloc(string.length() / 4 * 3 + 1);
        if( out )
        {
            int ret = _base64Decode(string.c_str(), (int)string.length(), out, &outLength);
            
            if (ret > 0 )
            {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
                printf("Base64Utils: error decoding");
#endif
            }
            else
            {
                result.resize(outLength);
                for (int i=0; i<outLength; i++)
                {
                    result[i] = out[i];
                }
            }
            free(out);
            outLength = 0;
        }

        return result;
    }
    
    std::string base64Encode(const std::string& string)
    {
        std::string result;
        
        int outLength = ((int)string.length() + 2) / 3 * 4;
        //should be enough to store 8-bit buffers in 6-bit buffers
        char* out = (char*)malloc(outLength + 1);
        if( out )
        {
            _base64Encode(string.c_str(), (int)string.length(), out);
        }
        
        result.resize(outLength);
        for (int i=0; i<outLength; i++)
        {
            result[i] = out[i];
        }
        
        free(out);
        outLength = 0;
        
        return result;
    }
}//namespace   CrossApp 
