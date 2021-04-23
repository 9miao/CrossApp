//
//  CACustomAnimation.h
//  CrossApp
//
//  Created by 栗元峰 on 15/4/29.
//  Copyright (c) 2015年 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CACustomAnimation__
#define __CrossApp__CACustomAnimation__

#include "basics/CAScheduler.h"

NS_CC_BEGIN

class CC_DLL CACustomAnimation
{
public:
    
    struct CC_DLL Model
    {
        float dt{0.0f};
        float now{0.0f};
        float total{0.0f};
        bool  end{false};
    };
    
    typedef std::function<void(const CACustomAnimation::Model&)> Callback;
    
    // defaule: interval = 1/60.f, delay = 0.0f;
    static void schedule(const CACustomAnimation::Callback& callback, const std::string& animationID, float totalTime);
    
    static void schedule(const CACustomAnimation::Callback& callback, const std::string& animationID, float totalTime, float interval);
    
    static void schedule(const CACustomAnimation::Callback& callback, const std::string& animationID, float totalTime, float interval, float delay);
    
    static void unschedule(const std::string& animationID);
    
    static bool isSchedule(const std::string& animationID);
    
};


NS_CC_END

#endif /* defined(__CrossApp__CACustomAnimation__) */
