//
//  CAClipboard.h
//  CrossApp
//
//  Created by Zhujian on 15-2-2.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//


#ifndef __CrossAppx__CAClipboard__
#define __CrossAppx__CAClipboard__

#include <string>
#include "ccMacros.h"

NS_CC_BEGIN


class CC_DLL CAClipboard
{
public:
	static std::string getText();
	static void setText(const std::string& cszStrText);
};

NS_CC_END

#endif /* defined(__cocos2dx__CAAlertView__) */