//
//  CAAnimation.h
//  CrossApp
//
//  Created by 栗元峰 on 15/4/29.
//  Copyright (c) 2015年 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAAnimation__
#define __CrossApp__CAAnimation__

#include "basics/CAScheduler.h"

NS_CC_BEGIN

typedef void (CAObject::*SEL_CAAnimation)(float delay, float now, float total);
#define CAAnimation_selector(_SELECTOR) (SEL_CAAnimation)(&_SELECTOR)

namespace CAAnimation
{

    CC_DLL void schedule(SEL_CAAnimation selector, CAObject* target, float totalTime, float interval = 1/60.0f, float delay = 0.0f);

};


NS_CC_END

#endif /* defined(__CrossApp__CAAnimation__) */
