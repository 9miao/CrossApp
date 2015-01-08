
#ifndef __CC_FILEUTILS_ANDROID_H__
#define __CC_FILEUTILS_ANDROID_H__

#include "platform/CCFileUtils.h"
#include "platform/CCPlatformMacros.h"
#include "ccTypes.h"
#include "ccTypeInfo.h"
#include <string>
#include <vector>

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class CC_DLL CCFileUtilsAndroid : public CCFileUtils
{
    friend class CCFileUtils;
    CCFileUtilsAndroid();
public:
    virtual ~CCFileUtilsAndroid();

    /* override funtions */
    bool init();
    virtual unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);
    virtual std::string getWritablePath();
    virtual bool isFileExist(const std::string& strFilePath);
    virtual bool isAbsolutePath(const std::string& strPath);
    
    /** This function is android specific. It is used for CCTextureCache::addImageAsync(). 
     Don't use it in your codes.
     */
    unsigned char* getFileDataForAsync(const char* pszFileName, const char* pszMode, unsigned long * pSize);
    
private:
    unsigned char* doGetFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize, bool forAsync);
};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_FILEUTILS_ANDROID_H__

