//
//  CAAnimation.cpp
//  CrossApp
//
//  Created by 栗元峰 on 15/4/29.
//  Copyright (c) 2015年 9秒社区. All rights reserved.
//

#include "CAAnimation.h"
#include "basics/CASTLContainer.h"
#include <set>
NS_CC_BEGIN

namespace CAAnimation
{
    struct Info
    {
        float interval;
        float now;
        float total;
        float delay;
        SEL_CAAnimation selector;
        CAObject* target;
    };

    class CC_DLL Animation : public CAObject
    {
    public:
        
        Animation();
        
        virtual ~Animation();
        
        void startAnimation(SEL_CAAnimation selector, CAObject* target, float totalTime, float interval, float delay);
        
        void update(float dt);
        
        Info m_obInfo;
        
    };
    
    static CADeque<Animation*> _deque;
    
    Animation::Animation()
    {
        
        m_obInfo.now = 0;
        m_obInfo.total = 0;
        m_obInfo.interval = 0;
        m_obInfo.selector = NULL;
        m_obInfo.target = NULL;
    }
    
    Animation::~Animation()
    {
        CC_SAFE_RELEASE(m_obInfo.target);
    }
    
    void Animation::startAnimation(SEL_CAAnimation selector, CAObject* target, float totalTime, float interval, float delay)
    {
        m_obInfo.total = totalTime;
        m_obInfo.interval = interval;
        m_obInfo.delay = delay;
        m_obInfo.selector = selector;
        m_obInfo.target = target->retain();
        CAScheduler::schedule(schedule_selector(Animation::update), this, interval);
    }
    
    void Animation::update(float dt)
    {
        if (m_obInfo.now >= m_obInfo.total)
        {
            _deque.eraseObject(this);
            return;
        }
        
        if (m_obInfo.delay > 0)
        {
            m_obInfo.delay -= dt;
            return;
        }
        
        if (m_obInfo.delay < 0)
        {
            dt += m_obInfo.delay;
            m_obInfo.delay = 0.0f;
        }
        
        m_obInfo.interval = dt;
        m_obInfo.now += dt;
        m_obInfo.now = MIN(m_obInfo.now, m_obInfo.total);

        if (m_obInfo.target && m_obInfo.selector)
        {
            ((CAObject *)m_obInfo.target->*m_obInfo.selector)(m_obInfo.interval, m_obInfo.now, m_obInfo.total);
        }
    }
    
    
    bool isSchedule(SEL_CAAnimation selector, CAObject* target)
    {
        bool ret = false;
        
        for (CADeque<Animation*>::iterator itr=_deque.begin(); itr!=_deque.end(); itr++)
        {
            Animation* obj = *itr;
            if (obj->m_obInfo.selector == selector && obj->m_obInfo.target == target)
            {
                ret = true;
                break;
            }
        }
        return ret;
    }
    
    void schedule(SEL_CAAnimation selector, CAObject* target, float totalTime, float interval, float delay)
    {
        CC_RETURN_IF(isSchedule(selector, target));
        Animation* obj = new Animation();
        _deque.pushBack(obj);
        obj->startAnimation(selector, target, totalTime, interval, delay);
        obj->release();
    }

    void unschedule(SEL_CAAnimation selector, CAObject* target)
    {
        for (CADeque<Animation*>::iterator itr=_deque.begin(); itr!=_deque.end(); itr++)
        {
            Animation* obj = *itr;
            if (obj->m_obInfo.selector == selector && obj->m_obInfo.target == target)
            {
                
                CAScheduler::unscheduleAllForTarget(obj);
                _deque.erase(itr);
                break;
            }
        }
    }

}
NS_CC_END