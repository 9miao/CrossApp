//
//  CASlider.h
//  cocos2dx
//
//  Created by juguanhui on 14-5-30.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//


#ifndef cocos2dx_CASlider_h
#define cocos2dx_CASlider_h

#include "CAControl.h"

NS_CC_BEGIN

class CAImageView;

class CC_DLL CASlider : public CAControl
{
public:
    CASlider();
    virtual ~CASlider();
    
    virtual void onExitTransitionDidStart();
    
    virtual void onEnterTransitionDidFinish();
    
    static CASlider* createWithFrame(const CCRect& rect);
    
    static CASlider* createWithCenter(const CCRect& rect);
    
public:
    
    bool initWithFrame(const CCRect& rect);
    
    bool initWithCenter(const CCRect& rect);
    
    void addTarget(CCObject* target, SEL_CAControl selector);
    
    void removeTarget(CCObject* target, SEL_CAControl selector);
    
public:
	CC_SYNTHESIZE_IS_READONLY(bool, m_bTouchClick, TouchClick);
    
    CC_SYNTHESIZE_READONLY(float, m_value, Value);       // default 0.0.
    
    CC_SYNTHESIZE_READONLY(float, m_minValue, MinValue); // default 0.0.
    
    CC_SYNTHESIZE_READONLY(float, m_maxValue, MaxValue); // default 1.0.
    
    CC_SYNTHESIZE_READONLY(float, m_trackHeight, TrackHeight); // default 4.0;
    
	CC_SYNTHESIZE_READONLY(CAImage*, m_pMinTrackTintImage, MinTrackTintImage);
    
	CC_SYNTHESIZE_READONLY(CAImage*, m_pMaxTrackTintImage, MaxTrackTintImage);
    
	CC_SYNTHESIZE_READONLY(CAImage*, m_pThumbTintImage, ThumbTintImage);
    
public:
    virtual void setValue(float value);
    virtual void setMinValue(float minValue);
    virtual void setMaxValue(float maxValue);
    virtual void setTrackHeight(float trackHeight);
    virtual void setMinTrackTintImage(CAImage* minTrackTintImage);
    virtual void setMaxTrackTintImage(CAImage* maxTrackTintImage);
    virtual void setThumbTintImage(CAImage* thumbTintImage);
    
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void layoutSubViews();
    
protected:
    CAImage* getImage(CAImage* image, CCSize size);
    virtual void addTarget(CCObject* target, SEL_CAControl selector, CAControlEvents event);
    virtual void removeTarget(CCObject* target, SEL_CAControl selector, CAControlEvents event);
    void setContentSize(const CCSize & var);
protected:
    CAImageView *m_pMinTrackTintImageView;
    CAImageView *m_pMaxTrackTintImageView;
    CAImageView *m_pThumbTintImageView;
    
};

NS_CC_END

#endif //defined(cocos2dx_CASlider_h)

