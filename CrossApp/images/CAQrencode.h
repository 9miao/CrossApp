

#ifndef __CAQrencode_H__
#define __CAQrencode_H__

#include "images/CAImage.h"

NS_CC_BEGIN

class CC_DLL CAQrencode: public CAObject
{
public:

    static CAImage* createWithQRString(const std::string& string, unsigned int sideLength);
    
    static CAImage* createWithQRString(const std::string& string, unsigned int sideLength, const CAColor4B& backgroundColor);

    static CAImage* createWithQRString(const std::string& string, unsigned int sideLength, const CAColor4B& backgroundColor, const CAColor4B& qrColor);
};


NS_CC_END

#endif // __CAQrencode_H__
