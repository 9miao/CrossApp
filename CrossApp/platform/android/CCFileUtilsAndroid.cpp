
#include "CCFileUtilsAndroid.h"
#include "support/zip_support/ZipUtils.h"
#include "platform/CACommon.h"
#include "jni/Java_org_CrossApp_lib_CrossAppHelper.h"
#include "platform/CCPlatformConfig.h"
#include <stdlib.h>
#include <sys/stat.h>

#define  LOG_TAG    "FileUtilsAndroid.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace std;

NS_CC_BEGIN

ZipFile* FileUtilsAndroid::obbfile = nullptr;

FileUtils* FileUtils::getInstance()
{
    if (s_sharedFileUtils == nullptr)
    {
        s_sharedFileUtils = new FileUtilsAndroid();
        if (!s_sharedFileUtils->init())
        {
            delete s_sharedFileUtils;
            s_sharedFileUtils = nullptr;
            CCLOG("ERROR: Could not init CCFileUtilsAndroid");
        }
    }
    return s_sharedFileUtils;
}

FileUtilsAndroid::FileUtilsAndroid()
{
}

FileUtilsAndroid::~FileUtilsAndroid()
{
    if (obbfile)
    {
        delete obbfile;
        obbfile = nullptr;
    }
}

bool FileUtilsAndroid::init()
{
    _defaultResRootPath = "assets/";
    
    std::string assetsPath(getApkPath());
    obbfile = new ZipFile(assetsPath, "assets/");
    
    return FileUtils::init();
}

std::string FileUtilsAndroid::getNewFilename(const std::string &filename) const
{
    std::string newFileName = FileUtils::getNewFilename(filename);
//    // ../xxx do not fix this path
//    auto pos = newFileName.find("../");
//    if (pos == std::string::npos || pos == 0)
//    {
//        return newFileName;
//    }
//    
//    std::vector<std::string> v(3);
//    v.resize(0);
//    auto change = false;
//    size_t size = newFileName.size();
//    size_t idx = 0;
//    bool noexit = true;
//    while (noexit)
//    {
//        pos = newFileName.find('/', idx);
//        std::string tmp;
//        if (pos == std::string::npos)
//        {
//            tmp = newFileName.substr(idx, size - idx);
//            noexit = false;
//        }else
//        {
//            tmp = newFileName.substr(idx, pos - idx + 1);
//        }
//        auto t = v.size();
//        if (t > 0 && v[t-1].compare("../") != 0 &&
//            (tmp.compare("../") == 0 || tmp.compare("..") == 0))
//        {
//            v.pop_back();
//            change = true;
//        }else
//        {
//            v.push_back(tmp);
//        }
//        idx = pos + 1;
//    }
//    
//    if (change)
//    {
//        newFileName.clear();
//        for (auto &s : v)
//        {
//            newFileName.append(s);
//        }
//    }
    return newFileName;
}

bool FileUtilsAndroid::isFileExistInternal(const std::string& strFilePath) const
{
    if (strFilePath.empty())
    {
        return false;
    }
    
    bool bFound = false;
    
    if (strFilePath[0] != '/')
    {
        std::string strPath = strFilePath;
        
        if (strFilePath.find(_defaultResRootPath) != 0)
        {
            strPath.insert(0, _defaultResRootPath);
        }
        
        if (obbfile && obbfile->fileExists(strPath))
        {
            bFound = true;
        }
    }
    else
    {
        FILE *fp = fopen(strFilePath.c_str(), "r");
        if (fp)
        {
            bFound = true;
            fclose(fp);
        }
    }
    return bFound;
}

bool FileUtilsAndroid::isDirectoryExistInternal(const std::string& dirPath) const
{
    if (dirPath.empty())
    {
        return false;
    }
    
    const char* s = dirPath.c_str();
    bool startWithAssets = (dirPath.find("assets/") == 0);
    int lenOfAssets = 7;
    
    std::string tmpStr;
    
    // find absolute path in flash memory
    if (s[0] == '/')
    {
        CCLOG("find in flash memory dirPath(%s)", s);
        struct stat st;
        if (stat(s, &st) == 0)
        {
            return S_ISDIR(st.st_mode);
        }
    }
    
    // find it in apk's assets dir
    // Found "assets/" at the beginning of the path and we don't want it
    CCLOG("find in apk dirPath(%s)", s);
    if (startWithAssets)
    {
        s += lenOfAssets;
    }
    return false;
}

unsigned char* FileUtilsAndroid::getFileData(const std::string& filename, const char* mode, unsigned long *size)
{
    return doGetFileData(filename.c_str(), mode, size, false);
}

unsigned char* FileUtilsAndroid::getFileDataForAsync(const std::string& filename, const char* mode, unsigned long *size, bool forAsync)
{
    return doGetFileData(filename.c_str(), mode, size, true);
}

unsigned char* FileUtilsAndroid::doGetFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize, bool forAsync)
{
    unsigned char * pData = 0;
    
    if ((! pszFileName) || (! pszMode) || 0 == strlen(pszFileName))
    {
        return 0;
    }
    
    string fullPath = fullPathForFilename(pszFileName);
    
    if (fullPath[0] != '/')
    {
        if (forAsync)
        {
            pData = obbfile->getFileData(fullPath.c_str(), pSize, obbfile->_dataThread);
        }
        else
        {
            pData = obbfile->getFileData(fullPath.c_str(), pSize);
        }
    }
    else
    {
        do
        {
            // read rrom other path than user set it
            //CCLOG("GETTING FILE ABSOLUTE DATA: %s", pszFileName);
            FILE *fp = fopen(fullPath.c_str(), pszMode);
            CC_BREAK_IF(!fp);
            
            unsigned long size;
            fseek(fp,0,SEEK_END);
            size = ftell(fp);
            fseek(fp,0,SEEK_SET);
            pData = new unsigned char[size];
            size = fread(pData,sizeof(unsigned char), size,fp);
            fclose(fp);
            
            if (pSize)
            {
                *pSize = size;
            }
        } while (0);
    }
    
    if (! pData)
    {
        std::string msg = "Get data from file(";
        msg.append(pszFileName).append(") failed!");
        CCLOG("%s", msg.c_str());
    }
    
    return pData;
}


bool FileUtilsAndroid::isAbsolutePath(const std::string& strPath) const
{
    // On Android, there are two situations for full path.
    // 1) Files in APK, e.g. assets/path/path/file.png
    // 2) Files not in APK, e.g. /data/data/org.cocos2dx.hellocpp/cache/path/path/file.png, or /sdcard/path/path/file.png.
    // So these two situations need to be checked on Android.
    if (strPath[0] == '/' || strPath.find(_defaultResRootPath) == 0)
    {
        return true;
    }
    return false;
}

string FileUtilsAndroid::getWritablePath()
{
    // Fix for Nexus 10 (Android 4.2 multi-user environment)
    // the path is retrieved through Java Context.getCacheDir() method
    string dir("");
    string tmp = getFileDirectoryJNI();
    
    if (tmp.length() > 0)
    {
        dir.append(tmp).append("/CrossApp/");
        this->createDirectory(dir);
        return dir;
    }
    else
    {
        return "";
    }
}

NS_CC_END

