
#ifndef __CC_FILEUTILS_IOS_H__
#define __CC_FILEUTILS_IOS_H__

#include "CCFileUtils.h"
#include <string>
#include <vector>
#include "CCPlatformMacros.h"
#include "ccTypes.h"
#include "ccTypeInfo.h"

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class CC_DLL CCFileUtilsIOS : public CCFileUtils
{
public:
    /* override funtions */
    virtual std::string getWritablePath();
    virtual bool isFileExist(const std::string& strFilePath);
    virtual bool isAbsolutePath(const std::string& strPath);
    virtual std::string getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename);
    
    virtual CAMap<CrossApp::CAObject*, CrossApp::CAObject*> createCCDictionaryWithContentsOfFile(const std::string& filename);
    virtual bool writeToFile(CCDictionary *dict, const std::string& fullPath);
    
    virtual CAVector<CAObject*> createCCArrayWithContentsOfFile(const std::string& filename);
};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_FILEUTILS_IOS_H__

