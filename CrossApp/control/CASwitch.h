//
//  CASwitch.h
//  CrossApp
//
//  Created by juguanhui on 14-5-29.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef CrossApp_CASwitch_h
#define CrossApp_CASwitch_h

#include "CAControl.h"
#include "view/CAScale9ImageView.h"
NS_CC_BEGIN

class CAImageView;
class CAImage;

class CC_DLL CASwitch : public CAControl 
{
public:
    CASwitch();
    virtual ~CASwitch();
    
    virtual void onExitTransitionDidStart();
    
    virtual void onEnterTransitionDidFinish();
    
    static CASwitch* createWithFrame(const CCRect& rect);
    
    static CASwitch* createWithCenter(const CCRect& rect);
    
    void addTarget(CAObject* target, SEL_CAControl selector);
    
    void removeTarget(CAObject* target, SEL_CAControl selector);
    
public:
    
    bool initWithFrame(const CCRect& rect);
    
    bool initWithCenter(const CCRect& rect);
    
protected:
    bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
public:
	CC_SYNTHESIZE_READONLY(CAImage*, m_onImage, OnImage);
	CC_SYNTHESIZE_READONLY(CAImage*, m_offImage, OffImage);
	CC_SYNTHESIZE_READONLY(CAImage*, m_thumbTintImage, ThumbTintImage);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_isOn, On);
	CC_SYNTHESIZE_IS_READONLY(bool, m_bTouchClick, TouchClick);
    
    void setIsOn(bool on, bool animated);
    virtual void setOnImage(CAImage* onImage);
    virtual void setOffImage(CAImage* offImage);
    void setThumbTintImage(CAImage* thumbTintImage);
    
protected:
    void updateSwitchState(bool animated, bool callfunced);
    void updateValueChanged();
    void setContentSize(const CCSize & var);
    using CAControl::addTarget;
    using CAControl::removeTarget;
    
protected:
    
    CAImageView *m_pOnImageView;
    CAImageView *m_pOffImageView;
    CAImageView *m_pThumbTintImageView;
};

NS_CC_END

#endif /*defined(CrossApp_CASwitch_h)*/
