

#ifndef __CC_FILEUTILS_APPLE_H__
#define __CC_FILEUTILS_APPLE_H__

#include <string>
#include <vector>

#include "platform/CAFileUtils.h"
#include "platform/CCPlatformMacros.h"
#include "ccTypes.h"

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class CC_DLL FileUtilsApple : public FileUtils
{
public:
    FileUtilsApple();
    /* override functions */
    virtual std::string getWritablePath() override;
    virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const override;
    
    virtual CAValueMap getValueMapFromFile(const std::string& filename) override;
    virtual CAValueMap getValueMapFromData(const char* filedata, int filesize)override;
    virtual bool writeToFile(const CAValueMap& dict, const std::string& fullPath) override;
    
    virtual CAValueVector getValueVectorFromFile(const std::string& filename) override;
    void setBundle(NSBundle* bundle);
private:
    virtual bool isFileExistInternal(const std::string& filePath) const override;
    virtual bool removeDirectory(const std::string& dirPath) override;
    
    NSBundle* getBundle() const;
    NSBundle* _bundle;
};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_FILEUTILS_APPLE_H__
