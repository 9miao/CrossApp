
#include "ccUtils.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iomanip>

namespace CrossApp {

CC_DLL unsigned long ccNextPOT(unsigned long x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return x + 1;
}

CC_DLL std::vector<std::string> Parse2StrVector(const std::string& str, const std::string& strDelimiter, bool bIncludeEmptyStr)
{
	std::vector<std::string> strVector;

	size_t pos = 0;
	if (!str.empty())
	{
		size_t first = str.find(strDelimiter);
		while (first != std::string::npos)
		{
			std::string cszStrTemp = str.substr(pos, first - pos);
			if (!cszStrTemp.empty() || bIncludeEmptyStr)
				strVector.push_back(cszStrTemp);

			pos = first + strDelimiter.size();
			first = str.find(strDelimiter, pos);
		}
		std::string cszStrTemp = str.substr(pos, str.size() - pos);
		if (!cszStrTemp.empty() || bIncludeEmptyStr)
			strVector.push_back(cszStrTemp);
	}
	return strVector;
}
    
CC_DLL unsigned int hex2Int(const std::string& hexStr)
{
    unsigned int x = 0xffffffff;
    
    sscanf(hexStr.c_str(), "%x", &x);
    
    return x;
}
    
CC_DLL std::string int2hex(int i)
{
    std::stringstream ioss;
    std::string s_temp;
    //ioss << std::setiosflags(std::ios::uppercase) << std::hex << i; //以十六制(大写)形式输出
    ioss << std::resetiosflags(std::ios::uppercase) << std::hex << i; //以十六制(小写)形式输出//取消大写的设置
    ioss >> s_temp;
    return s_temp;
}

}
