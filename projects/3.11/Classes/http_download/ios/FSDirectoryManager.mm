//
//  FSDirectoryManager.cpp
//  IceBaller
//
//  Created by Lawis on 12-11-13.
//  Copyright (c) 2012年 Mobile Arts. All rights reserved.
//

#include "FSDirectoryManager.h"

std::string FSDirectoryManager::getRootDirectoryByType(tFSDirectoryType dType)
{
    NSArray *paths;
    NSString *resultDirectory;
    
    switch (dType)
    {
        case kFSDirTypeDocument:
            paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            resultDirectory = [paths objectAtIndex:0];
            break;
        case kFSDirTypeCaches:
            paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
            resultDirectory = [paths objectAtIndex:0];
            break;
        case kFSDirTypeTmp:
            resultDirectory = NSTemporaryDirectory();
            break;
        case kFSDirTypeLibrary:
            paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
            resultDirectory = [paths objectAtIndex:0];
            break;
        case kFSDirTypeBundle:
            resultDirectory = [[NSBundle mainBundle] resourcePath];
            break;
        default:
            break;
    }
    std::string strRet = [resultDirectory UTF8String];
    strRet.append("/");
    return strRet;
}
std::string FSDirectoryManager::getFileDirectoryByType(std::string filePath,tFSDirectoryType dType)
{
    std::string resultStr = FSDirectoryManager::getRootDirectoryByType(dType);
    resultStr.append(filePath);
    return resultStr;
}

bool        FSDirectoryManager::creatChildDirectoryAtPath(std::string childPath,tFSDirectoryType dType)
{
    std::string rootPath = FSDirectoryManager::getRootDirectoryByType(dType);
    rootPath.append(childPath);
    NSString *path = [NSString stringWithUTF8String:rootPath.c_str()];
    return [[NSFileManager defaultManager] createDirectoryAtPath:[path substringToIndex:[path rangeOfString:@"/" options:NSBackwardsSearch].location] withIntermediateDirectories:YES attributes:nil error:nil];
}

bool        FSDirectoryManager::existFileAtPath(std::string childPath,tFSDirectoryType dType,bool *isDirectory)
{
    std::string rootPath = FSDirectoryManager::getRootDirectoryByType(dType);
    rootPath.append(childPath);
    NSString *path = [NSString stringWithUTF8String:rootPath.c_str()];
    return [[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:(BOOL *)isDirectory];
}