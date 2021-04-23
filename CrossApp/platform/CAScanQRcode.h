//
//  CAScanQRcode.hpp
//  CrossApp
//
//  Created by mac on 2021/3/12.
//  Copyright © 2021 cocos2d-x. All rights reserved.
//

#ifndef CAScanQRcode_hpp
#define CAScanQRcode_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include "ccMacros.h"

NS_CC_BEGIN


class CC_DLL CAScanQRcode
{
public:
    
    static void showScanQRcode(const std::function<void(const std::string&)>& callback);
};

NS_CC_END
#endif /* CAScanQRcode_hpp */
