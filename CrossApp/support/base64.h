

#ifndef __SUPPORT_BASE64_H__
#define __SUPPORT_BASE64_H__

#ifdef __cplusplus
extern "C" {
#endif    
    
namespace CrossApp
{

/** @file
 base64 helper functions
 */

/** 
 * Decodes a 64base encoded memory. The decoded memory is
 * expected to be freed by the caller.
 *
 * @returns the length of the out buffer
 *
 @since v0.8.1
 */
int base64Decode(unsigned char *in, unsigned int inLength, unsigned char **out);

}//namespace   CrossApp 

#ifdef __cplusplus
}
#endif    

#endif // __SUPPORT_BASE64_H__
