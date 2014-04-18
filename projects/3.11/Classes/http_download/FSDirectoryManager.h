//
//  FSDirectoryManager.h
//  IceBaller
//
//  Created by Lawis on 12-11-13.
//  Copyright (c) 2012年 Mobile Arts. All rights reserved.
//

#ifndef __IceBaller__FSDirectoryManager__
#define __IceBaller__FSDirectoryManager__

#include <string>
#include "cocos2d.h"
typedef enum
{
	kFSDirTypeDocument = 0,
	kFSDirTypeCaches,
	kFSDirTypeTmp,
	kFSDirTypeLibrary,
	kFSDirTypeBundle,
} tFSDirectoryType; //获取的文件相对位置

using namespace cocos2d;

class FSDirectoryManager
{

public:
	static std::string getRootDirectoryByType(tFSDirectoryType dType);
	static std::string getFileDirectoryByType(std::string filePath,
			tFSDirectoryType dType);
	static bool creatChildDirectoryAtPath(std::string childPath,
			tFSDirectoryType dType);
	static bool existFileAtPath(std::string childPath, tFSDirectoryType dType,
			bool *isDirectory);

};
#endif /* defined(__IceBaller__FSDirectoryManager__) */
