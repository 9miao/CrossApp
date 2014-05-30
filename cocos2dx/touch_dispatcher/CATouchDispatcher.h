//
//  CATouchDispatcher.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-23.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__
#define __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__

#include "cocoa/CCObject.h"
#include "cocoa/CCArray.h"
#include "platform/CCPlatformMacros.h"
NS_CC_BEGIN


class CCSet;
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

    CC_SYNTHESIZE_IS(bool, m_bDispatchEvents, DispatchEvents);
    
    bool m_bLocked;
    
};

// end of input group
/// @}

NS_CC_END

#endif // __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__
