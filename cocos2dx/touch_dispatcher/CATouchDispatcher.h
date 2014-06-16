//
//  CATouchDispatcher.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__
#define __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__

#include "cocoa/CAResponder.h"
#include "platform/CCPlatformMacros.h"
#include <vector>
#include <map>


NS_CC_BEGIN


class CCTouch;
class CCEvent;

/**
 * @js NA
 */
class CC_DLL EGLTouchDelegate
{
public:
    /**
     * @lua NA
     */
    virtual void touchesBegan(CCSet* touches, CCEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual void touchesMoved(CCSet* touches, CCEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual void touchesEnded(CCSet* touches, CCEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual void touchesCancelled(CCSet* touches, CCEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual ~EGLTouchDelegate() {}
};


class CC_DLL CATouchController: public CCObject
{
    
public:
    
    CATouchController();
    
    virtual ~CATouchController();

    void touchBegan();

    void touchMoved();

    void touchEnded();

    void touchCancelled();
    
    int getTouchID();
    
    CC_SYNTHESIZE_RETAIN(CCTouch*, m_pTouch, Touch);
    
    CC_SYNTHESIZE_RETAIN(CCEvent*, m_pEvent, Event);
    
protected:
    
    void passingTouchesViewCache(float dt = 0);
    
    std::vector<CAView*> getEventListener(CCTouch* touch, CAView* view);
    
protected:
    
    std::vector<CAView*> m_vWillTouchesViewCache;
    
    std::vector<CAView*> m_vTouchesViewCache;
    
    CCPoint m_tFirstPoint;
};

class CC_DLL CATouchDispatcher : public CCObject, public EGLTouchDelegate
{
public:
    /**
     * @lua NA
     */
    CATouchDispatcher();
    /**
     * @lua NA
     */
    ~CATouchDispatcher();
    /**
     * @lua NA
     */
    bool init(void);

    virtual void setDispatchEvents(bool dispatchEvents);
    
public:

    /**
     * @lua NA
     */
    virtual void touchesBegan(CCSet* touches, CCEvent* pEvent);
    /**
     * @lua NA
     */
    virtual void touchesMoved(CCSet* touches, CCEvent* pEvent);
    /**
     * @lua NA
     */
    virtual void touchesEnded(CCSet* touches, CCEvent* pEvent);
    /**
     * @lua NA
     */
    virtual void touchesCancelled(CCSet* touches, CCEvent* pEvent);

protected:

    CC_SYNTHESIZE_RETAIN(CAResponder*, m_pFirstResponder, FirstResponder);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bDispatchEvents, DispatchEvents);
    
    bool m_bLocked;
    
    std::map<int, CATouchController*> m_vTouchControllers;
};

// end of input group
/// @}

NS_CC_END

#endif // __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__
