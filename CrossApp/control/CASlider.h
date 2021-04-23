//
//  CASlider.h
//  CrossApp
//
//  Created by juguanhui on 14-5-30.
//  Copyright (c) 2014年 CrossApp-x. All rights reserved.
//


#ifndef CrossApp_CASlider_h
#define CrossApp_CASlider_h

#include "CAControl.h"
#include "view/CAScale9ImageView.h"
NS_CC_BEGIN

class CC_DLL CASlider : public CAControl
{
public:
    CASlider();
    virtual ~CASlider();
    
    static CASlider* create();
    static CASlider* createWithFrame(const DRect& rect);
    static CASlider* createWithCenter(const DRect& rect);
    static CASlider* createWithLayout(const DLayout& layout);
    
public:
    
    bool init();
    bool initWithFrame(const DRect& rect);
    bool initWithCenter(const DRect& rect);
    
    void setTarget(const std::function<void(float)>& function);
    void setTargetForTouchUpSide(const std::function<void(float)>& function);
    
    virtual void setValue(float value);
    virtual void setMinValue(float minValue);
    virtual void setMaxValue(float maxValue);
    
    virtual void setTrackHeight(float trackHeight);
    
    virtual void setMinTrackTintImage(CAImage* minTrackTintImage);
    virtual void setMaxTrackTintImage(CAImage* maxTrackTintImage);
    virtual void setThumbTintImage(CAImage* thumbTintImage);
    
public:
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bTouchClick, TouchClick);
    
    CC_SYNTHESIZE_READONLY(float, m_fValue, Value);       // default 0.0.
    
    CC_SYNTHESIZE_READONLY(float, m_fMinValue, MinValue); // default 0.0.
    
    CC_SYNTHESIZE_READONLY(float, m_fMaxValue, MaxValue); // default 1.0.
    
    CC_SYNTHESIZE_READONLY(float, m_fTrackHeight, TrackHeight); // default 4.0;
    
	CC_SYNTHESIZE_READONLY(CAImage*, m_pMinTrackTintImage, MinTrackTintImage);
    
	CC_SYNTHESIZE_READONLY(CAImage*, m_pMaxTrackTintImage, MaxTrackTintImage);
    
	CC_SYNTHESIZE_READONLY(CAImage*, m_pThumbTintImage, ThumbTintImage);

public:
    
    virtual void onExitTransitionDidStart();
    virtual void onEnterTransitionDidFinish();
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void layoutSubViews();
    
protected:
    
    void setContentSize(const DSize & var);
    
    void _setValue(float value);
    
protected:
    
    CAScale9ImageView *m_pMinTrackTintImageView;
    
    CAScale9ImageView *m_pMaxTrackTintImageView;
    
    CAImageView *m_pThumbTintImageView;
    
    std::function<void(float)> m_function{nullptr};
    
    std::function<void(float)> m_functionTouchUpSide{nullptr};
};

NS_CC_END

#endif //defined(CrossApp_CASlider_h)

