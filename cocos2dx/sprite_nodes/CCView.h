//
//  CCView.h
//  cocos2dx
//
//  Created by 栗元峰 on 14-4-4.
//  Copyright (c) 2014年 厦门雅基软件有限公司. All rights reserved.
//

#ifndef __cocos2dx__CCView__
#define __cocos2dx__CCView__

#include <iostream>
#include "base_nodes/CCNode.h"
#include "CCProtocols.h"
#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "platform/CCAccelerometerDelegate.h"
#include "keypad_dispatcher/CCKeypadDelegate.h"
#include "cocoa/CCArray.h"
#ifdef EMSCRIPTEN
#include "base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

//
// CCLayerColor
//
/** @brief CCLayerColor is a subclass of CCLayer that implements the CCRGBAProtocol protocol.
 
 All features from CCLayer are valid, plus the following new features:
 - opacity
 - RGB colors
 */

class CAViewDelegate
{
public:
    
    virtual void getSuperViewRect(const CCRect& rect) = 0;

    virtual void viewOnEnterTransitionDidFinish() = 0;
    
    virtual void viewOnExitTransitionDidStart() = 0;
};

class CCLayerColor;
typedef CCLayerColor CAView;
class CC_DLL CCLayerColor : public CCNodeRGBA, public CCBlendProtocol

#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
    CC_SYNTHESIZE(CAViewDelegate*, m_pViewDelegate, ViewDelegate);
    
protected:
    ccVertex2F m_pSquareVertices[4];
    ccColor4F  m_pSquareColors[4];
    
public:
    /**
     *  @js ctor
     */
    CCLayerColor();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCLayerColor();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual void draw();
    virtual void setContentSize(const CCSize & var);
    
    static CCLayerColor * createWithFrame(const CCRect& rect);

    virtual bool init();
    
    virtual bool initWithFrame(const CCRect& rect);
    
    /** BlendFunction. Conforms to CCBlendProtocol protocol */
    CC_PROPERTY(ccBlendFunc, m_tBlendFunc, BlendFunc)
    
    virtual void setColor(const ccColor3B &color);
    virtual void setOpacity(GLubyte opacity);
    
protected:
    virtual void updateColor();
};

NS_CC_END

#endif /* defined(__cocos2dx__CCView__) */
