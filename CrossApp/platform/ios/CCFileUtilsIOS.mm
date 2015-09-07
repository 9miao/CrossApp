
#import <Foundation/Foundation.h>
#import <UIKit/UIDevice.h>
#include <string>
#include <stack>
#include "CCFileUtils.h"
#include "CAApplication.h"
#include "CCSAXParser.h"
#include "support/zip_support/unzip.h"
#include "CCFileUtilsIOS.h"

NS_CC_BEGIN

CCFileUtils* CCFileUtils::sharedFileUtils()
{
    if (s_sharedFileUtils == NULL)
    {
        s_sharedFileUtils = new CCFileUtilsIOS();
        s_sharedFileUtils->init();
    }
    return s_sharedFileUtils;
}


static NSFileManager* s_fileManager = [NSFileManager defaultManager];

std::string CCFileUtilsIOS::getWritablePath()
{
    // save to document folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/CrossApp/");
    this->createDirectory(strRet.c_str());
    return strRet;
}

bool CCFileUtilsIOS::isFileExist(const std::string& strFilePath)
{
    if (0 == strFilePath.length())
    {
        return false;
    }

    bool bRet = false;
    
    if (strFilePath[0] != '/')
    {
        std::string path;
        std::string file;
        size_t pos = strFilePath.find_last_of("/");
        if (pos != std::string::npos)
        {
            file = strFilePath.substr(pos+1);
            path = strFilePath.substr(0, pos+1);
        }
        else
        {
            file = strFilePath;
        }
        
        NSString* fullpath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:file.c_str()]
                                                             ofType:nil
                                                        inDirectory:[NSString stringWithUTF8String:path.c_str()]];
        if (fullpath != nil)
        {
            bRet = true;
        }
    }
    else
    {
        // Search path is an absolute path.
        if ([s_fileManager fileExistsAtPath:[NSString stringWithUTF8String:strFilePath.c_str()]])
        {
            bRet = true;
        }
    }
    
    return bRet;
}

std::string CCFileUtilsIOS::getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename)
{
    if (strDirectory[0] != '/')
    {
        NSString* fullpath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:strFilename.c_str()]
                                                             ofType:nil
                                                        inDirectory:[NSString stringWithUTF8String:strDirectory.c_str()]];
        if (fullpath != nil) {
            return [fullpath UTF8String];
        }
    }
    else
    {
        std::string fullPath = strDirectory+strFilename;
        // Search path is an absolute path.
        if ([s_fileManager fileExistsAtPath:[NSString stringWithUTF8String:fullPath.c_str()]]) {
            return fullPath;
        }
    }
    return "";
}

bool CCFileUtilsIOS::isAbsolutePath(const std::string& strPath)
{
    NSString* path = [NSString stringWithUTF8String:strPath.c_str()];
    return [path isAbsolutePath] ? true : false;
}

NS_CC_END

