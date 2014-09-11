#ifndef _Info_h_
#define _Info_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class Info :public CAObject
{
public:
    std::string name;
	std::string num;
	std::string gender;
	std::string occupation;
};

#endif