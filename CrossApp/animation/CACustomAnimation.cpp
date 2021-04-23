//
//  CACustomAnimation.cpp
//  CrossApp
//
//  Created by 栗元峰 on 15/4/29.
//  Copyright (c) 2015年 9秒社区. All rights reserved.
//

#include "CACustomAnimation.h"
#include "basics/CASTLContainer.h"
#include <set>
NS_CC_BEGIN

struct CACustomAnimationM : public CAObject
{
    struct Info
    {
        CACustomAnimation::Model model;
        std::string animationID;
        float delay{0.0f};
        CACustomAnimation::Callback callback{nullptr};
    };
    
    void startAnimation(const CACustomAnimation::Callback& callback, const std::string& animationID, float totalTime, float interval, float delay);
    
    void end();
    
    Info info;
};

static std::map<std::string, CACustomAnimationM*> _map;

void CACustomAnimationM::startAnimation(const CACustomAnimation::Callback& callback, const std::string& animationID, float totalTime, float interval, float delay)
{
    info.model.total = totalTime;
    info.model.dt = interval;
    info.delay = delay;
    info.callback = callback;
    info.animationID = animationID;
    CAScheduler::getScheduler()->schedule([&](float dt){
        if (info.delay > 0)
        {
            info.delay -= dt;
            return;
        }
        
        if (info.delay < 0)
        {
            dt += info.delay;
            info.delay = 0.0f;
        }
        
        info.model.dt = dt;
        info.model.now += dt;
        info.model.now = MIN(info.model.now, info.model.total);
        
        if (info.model.now == info.model.total)
        {
            this->end();
        }
        else if (info.callback)
        {
            info.callback(info.model);
        }
        
    }, "update", this, interval);
}

void CACustomAnimationM::end()
{
    info.model.end = true;
    _map.erase(info.animationID);
    CAScheduler::getScheduler()->unschedule("update", this);
    if (info.callback)
    {
        info.callback(info.model);
    }
    this->release();
}

bool CACustomAnimation::isSchedule(const std::string& animationID)
{
    return _map.find(animationID) != _map.end();
}

void CACustomAnimation::schedule(const CACustomAnimation::Callback& callback, const std::string& animationID, float totalTime)
{
    CACustomAnimation::schedule(callback, animationID, totalTime, 0, 0.0f);
}

void CACustomAnimation::schedule(const CACustomAnimation::Callback& callback, const std::string& animationID, float totalTime, float interval)
{
    CACustomAnimation::schedule(callback, animationID, totalTime, interval, 0.0f);
}

void CACustomAnimation::schedule(const CACustomAnimation::Callback& callback, const std::string& animationID, float totalTime, float interval, float delay)
{
    CC_RETURN_IF(_map.find(animationID) != _map.end());
    CACustomAnimationM* obj = new CACustomAnimationM();
    obj->startAnimation(callback, animationID, totalTime, interval, delay);
    _map.insert(std::make_pair(animationID, obj));
}

void CACustomAnimation::unschedule(const std::string& animationID)
{
    CC_RETURN_IF(_map.find(animationID) == _map.end());
    _map.at(animationID)->end();
}


NS_CC_END
