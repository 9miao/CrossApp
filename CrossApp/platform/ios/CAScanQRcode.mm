//
//  CAScanQRcode.cpp
//  CrossApp
//
//  Created by mac on 2021/3/12.
//  Copyright © 2021 CrossApp. All rights reserved.
//

#include "platform/CAScanQRcode.h"
#import "CAScanQRcodeView.h"
#import "CAScanQRcodeView.h"
#include "EAGLView.h"

NS_CC_BEGIN

void CAScanQRcode::showScanQRcode(const std::function<void(const std::string&)>& callback)
{
    [CAScanQRcodeView show:callback];
}
NS_CC_END
