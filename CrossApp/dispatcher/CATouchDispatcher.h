//
//  CATouchDispatcher.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __TOUCH_DISPATCHER_CATouch_DISPATCHER_H__
#define __TOUCH_DISPATCHER_CATouch_DISPATCHER_H__

#include "basics/CAResponder.h"
#include "platform/CCPlatformMacros.h"
#include "basics/CASTLContainer.h"
#include "cocoa/CCSet.h"
NS_CC_BEGIN


class CATouch;
class CAEvent;

/**
 * @js NA
 */
class CC_DLL CCEGLTouchDelegate
{
public:
    /**
     * @lua NA
     */
    virtual void touchesBegan(CCSet* touches, CAEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual void touchesMoved(CCSet* touches, CAEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual void touchesEnded(CCSet* touches, CAEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual void touchesCancelled(CCSet* touches, CAEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual void mouseMoved(CATouch* pTouch, CAEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual void mouseScrollWheel(CATouch* pTouch, float off_x, float off_y, CAEvent* pEvent) = 0;
    /**
     * @lua NA
     */
    virtual ~CCEGLTouchDelegate() {}
};


class CC_DLL CATouchController: public CAObject
{
    
public:
    
    CATouchController();
    
    virtual ~CATouchController();

    void touchBegan();

    void touchMoved();

    void touchEnded();

    void touchCancelled();
    
    int getTouchID();
    
    CC_SYNTHESIZE_RETAIN(CATouch*, m_pTouch, Touch);
    
    CC_SYNTHESIZE_RETAIN(CAEvent*, m_pEvent, Event);
    
protected:
    
    void passingTouchesViews(float dt = 0);
    
    std::vector<CAResponder*> getEventListener(CATouch* touch, CAView* view);

protected:

    CAVector<CAResponder*> m_vTouchMovedsView;
    
    CAVector<CAResponder*> m_vTouchMovedsViewCache;
    
    CAVector<CAResponder*> m_vTouchesViews;
    
    DPoint m_tFirstPoint;
};

class CC_DLL CATouchDispatcher : public CAObject, public CCEGLTouchDelegate
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
    
    virtual bool isDispatchEvents() { return m_iDispatchEvents > 0; }
    
    void setDispatchEventsTrue();
    
    void setDispatchEventsFalse();
    
    int getTouchCount();
    
    void addMouseMovedResponder(CAResponder* responder);
    
    void removeMouseMovedResponder(CAResponder* responder);
    
    void addMouseScrollWheel(CAResponder* responder);
    
    void removeMouseScrollWheel(CAResponder* responder);
    
public:

    /**
     * @lua NA
     */
    virtual void touchesBegan(CCSet* touches, CAEvent* pEvent);
    /**
     * @lua NA
     */
    virtual void touchesMoved(CCSet* touches, CAEvent* pEvent);
    /**
     * @lua NA
     */
    virtual void touchesEnded(CCSet* touches, CAEvent* pEvent);
    /**
     * @lua NA
     */
    virtual void touchesCancelled(CCSet* touches, CAEvent* pEvent);
    /**
     * @lua NA
     */
    virtual void mouseMoved(CATouch* pTouch, CAEvent* pEvent);
    /**
     * @lua NA
     */
    virtual void mouseScrollWheel(CATouch* pTouch, float off_x, float off_y, CAEvent* pEvent);
    
protected:

    CC_SYNTHESIZE_RETAIN(CAResponder*, m_pFirstResponder, FirstResponder);

    int m_iDispatchEvents;
    
    bool m_bLocked;
    
    std::map<int, CATouchController*> m_vTouchControllers;
    
    std::set<CAResponder*> m_pMouseMoveds;
    
    std::set<CAResponder*> m_pMouseScrollWheels;
};

// end of input group
/// @}

NS_CC_END

#endif // __TOUCH_DISPATCHER_CATouch_DISPATCHER_H__
