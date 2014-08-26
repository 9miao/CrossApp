//
//  CASlider.h
//  CrossAppx
//
//  Created by juguanhui on 14-5-30.
//  Copyright (c) 2014年 CrossApp-x. All rights reserved.
//


#ifndef CrossAppx_CASlider_h
#define CrossAppx_CASlider_h

#include "CAControl.h"
#include "view/CAScale9ImageView.h"
NS_CC_BEGIN

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
    
    void addTarget(CAObject* target, SEL_CAControl selector);
    
    void removeTarget(CAObject* target, SEL_CAControl selector);
    
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
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void layoutSubViews();
    
protected:
    void setContentSize(const CCSize & var);
    using CAControl::addTarget;
    using CAControl::removeTarget;
    
protected:
    CAScale9ImageView *m_pMinTrackTintImageView;
    CAScale9ImageView *m_pMaxTrackTintImageView;
    CAScale9ImageView *m_pThumbTintImageView;
    
};

NS_CC_END

#endif //defined(CrossAppx_CASlider_h)

