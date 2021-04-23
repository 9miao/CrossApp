
#ifndef __CC_FILEUTILS_ANDROID_H__
#define __CC_FILEUTILS_ANDROID_H__

#include "platform/CAFileUtils.h"
#include "platform/CCPlatformMacros.h"
#include "ccTypes.h"
#include "ccTypeInfo.h"
#include <string>
#include <vector>

NS_CC_BEGIN

class ZipFile;

class CC_DLL FileUtilsAndroid : public FileUtils
{
    friend class FileUtils;
public:
    FileUtilsAndroid();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~FileUtilsAndroid();

    static ZipFile* getObbFile() { return obbfile; }
    
    /* override functions */
    bool init() override;
    
    virtual unsigned char* getFileData(const std::string& filename, const char* mode, unsigned long *size);
    unsigned char* getFileDataForAsync(const std::string& filename, const char* mode, unsigned long *size, bool forAsync);
    virtual std::string getNewFilename(const std::string &filename) const;
    
    virtual std::string getWritablePath();
    virtual bool isAbsolutePath(const std::string& strPath) const;
    
private:
    virtual bool isFileExistInternal(const std::string& strFilePath) const;
    virtual bool isDirectoryExistInternal(const std::string& dirPath) const;
    
    unsigned char* doGetFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize, bool forAsync);
    
    static ZipFile* obbfile;
};


NS_CC_END

#endif    // __CC_FILEUTILS_ANDROID_H__

