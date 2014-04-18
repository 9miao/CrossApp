/*
 * FSDirectoryManager.cpp
 *
 *  Created on: 2013-4-22
 *      Author: Clark
 */

#include "http_download/FSDirectoryManager.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace cocos2d;
using namespace std;

bool creatDir(std::string dir)
{
	if ("" == dir) {
		return false;
	}

	int i = 0;
	int ret;
	int len;
	char* pszDir;
	try
    {
		pszDir = strdup(dir.c_str());
		len = strlen(pszDir);

		for (i = 0; i < len; i++)
        {
			if ((pszDir[i] == '\\' || pszDir[i] == '/') && i > 0)
            {

				pszDir[i] = '\0';
//				LOGD(pszDir);

				/**
				 *access函数 mode:表示测试的模式可能的值有：
				 *R_OK:是否具有读权限
				 *W_OK:是否具有可写权限
				 *X_OK:是否具有可执行权限
				 *F_OK:文件是否存在
				 */
				ret = access(pszDir, F_OK);
				if (ret != 0)
                {
					ret = mkdir(pszDir, 0755);
					if (ret != 0)
                    {
//						LOGD("创建失败");
						free(pszDir);
						return false;
					}
//					LOGD("创建成功");
				}
				pszDir[i] = '/';
			}
		}
		ret = mkdir(pszDir, 0755);
		free(pszDir);
	}
    catch (exception &e)
    {
		if (pszDir)
        {
			free(pszDir);
			pszDir = NULL;
		}
		return false;
	}
	return ret == 0;
}

std::string FSDirectoryManager::getRootDirectoryByType(tFSDirectoryType dType)
{
	std::string rootPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    
	switch (dType)
    {
	case kFSDirTypeDocument:
		//rootPath.append("doc");
		rootPath = rootPath.substr(0, rootPath.size());
		break;
	case kFSDirTypeCaches:
		rootPath.append("cache/");
		creatDir(rootPath);
		break;
	case kFSDirTypeTmp:
		rootPath.append("tmp/");
		creatDir(rootPath);
		break;
	case kFSDirTypeLibrary:
		rootPath.append("libs/");
		creatDir(rootPath);
		break;
	case kFSDirTypeBundle:
		rootPath = "assets/";
		break;
	default:
		break;
	}
	return rootPath;
}

std::string FSDirectoryManager::getFileDirectoryByType(std::string filePath,
		tFSDirectoryType dType)
{
	std::string resultStr = FSDirectoryManager::getRootDirectoryByType(dType);
	resultStr.append(filePath);
	return resultStr;
}

bool FSDirectoryManager::creatChildDirectoryAtPath(std::string childPath,
		tFSDirectoryType dType)
{
	std::string dir = FSDirectoryManager::getFileDirectoryByType(childPath,
			dType);
	return creatDir(dir);
}

bool FSDirectoryManager::existFileAtPath(std::string childPath,
		tFSDirectoryType dType, bool *isDirectory)
{
	std::string filename = FSDirectoryManager::getFileDirectoryByType(childPath,
			dType);
	struct stat __stat;
	int r = stat(filename.c_str(), &__stat);
	if (r == 0)
    {
		if (isDirectory)
        {
			if (__stat.st_mode & S_IFDIR)
            {
				*isDirectory = true;
			}
            else
            {
				*isDirectory = false;
			}
		}
		return true;
	}
	return false;
}

