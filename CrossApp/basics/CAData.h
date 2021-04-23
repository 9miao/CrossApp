

#ifndef __DATA_H__
#define __DATA_H__

#include "platform/CCPlatformMacros.h"
#include <stdint.h> // for ssize_t on android
#include <string>   // for ssize_t on linux
#include "CCStdC.h" // for ssize_t on window
#include "basics/CAObject.h"
#include "ccTypes.h"
NS_CC_BEGIN

class CC_DLL CAData : public CAObject
{
public:
    
    CAData();
    
    virtual ~CAData();
    
    CREATE_FUNC(CAData);
    
    void copy(const unsigned char* bytes, const ssize_t length);

    void fastSet(unsigned char* bytes, const ssize_t length);
    
    void copyString(const std::string& var);

    bool isNull() const;
    
    unsigned char* getBytes() const;

    ssize_t getLength() const;
    
    std::string toString();
    
    void clear();
    
private:
    
    unsigned char*  m_pBytes;
    
    ssize_t         m_iLength;
};

NS_CC_END

#endif // __DATA_H__
