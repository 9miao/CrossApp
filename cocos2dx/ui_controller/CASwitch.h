//
//  CASwitch.h
//  cocos2dx
//
//  Created by juguanhui on 14-5-29.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef cocos2dx_CASwitch_h
#define cocos2dx_CASwitch_h

#include "CAControl.h"

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
    
    void addTarget(CCObject* target, SEL_CAControl selector);
    
    void removeTarget(CCObject* target, SEL_CAControl selector);
    
public:
    
    bool initWithFrame(const CCRect& rect);
    
    bool initWithCenter(const CCRect& rect);
    
protected:
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
public:
	CC_SYNTHESIZE_READONLY(CAImage*, m_onImage, OnImage);
	CC_SYNTHESIZE_READONLY(CAImage*, m_offImage, OffImage);
	CC_SYNTHESIZE_READONLY(CAImage*, m_thumbTintImage, ThumbTintImage);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_isOn, On);
	CC_SYNTHESIZE_IS_READONLY(bool, m_bTouchClick, TouchClick);
    
    void setIsOn(bool on);
    virtual void setOnImage(CAImage* onImage);
    virtual void setOffImage(CAImage* offImage);
    void setThumbTintImage(CAImage* thumbTintImage);
protected:
    CAImage* getImage(CAImage* image, CCSize size);
    void updateSwitchState();
    virtual void addTarget(CCObject* target, SEL_CAControl selector, CAControlEvents event);
    virtual void removeTarget(CCObject* target, SEL_CAControl selector, CAControlEvents event);
    void setContentSize(const CCSize & var);
protected:
    CAImageView *m_onImageView;
    CAImageView *m_offImageView;
    CAImageView *m_thumbTintImageView;
};

NS_CC_END

#endif /*defined(cocos2dx_CASwitch_h)*/
