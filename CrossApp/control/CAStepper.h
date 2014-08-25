//
//  CAStepper.h
//  CrossApp
//
//  Created by dai xinping on 14-8-15.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#ifndef __CrossApp__CAStepper__
#define __CrossApp__CAStepper__

#include <iostream>
#include "CAControl.h"

NS_CC_BEGIN

class CAImageView;
class CC_DLL CAStepper : public CAControl {
    
public:
    CAStepper();
    virtual ~CAStepper();

    static CAStepper* create();
    static CAStepper* createWithFrame(const CCRect& rect);
    static CAStepper* createWithCenter(const CCRect& rect);
    
    virtual bool init();
    virtual bool initWithFrame(const CCRect& rect);
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void visit();
    
    CC_SYNTHESIZE(bool, m_bContinuous, Continuous); // if true, value change events are sent any time the value changes during interaction. default = true
    CC_SYNTHESIZE(bool, m_bAutoRepeat, AutoRepeat); // if true, press & hold repeatedly alters value. default = true
    CC_SYNTHESIZE(bool, m_bWraps, Wraps);           // if true, value wraps from min <-> max. default = false
    
    CC_SYNTHESIZE(double, m_value, Value);              // default is 0. sends UIControlEventValueChanged. clamped to min/max
    CC_SYNTHESIZE(double, m_minimumValue, MinValue);    // default 0. must be less than maximumValue
    CC_SYNTHESIZE(double, m_maximumValue, MaxValue);    // default 100. must be greater than minimumValue
    CC_SYNTHESIZE(double, m_stepValue, StepValue);      // default 1. must be greater than 0
    
//    CC_SYNTHESIZE(CAColor4B, m_tintColor, TintColor);
    
public:
    // a background image which will be 3-way stretched over the whole of the control. Each half of the stepper will paint the image appropriate for its state      
    void setBackgroundImage(CAImage* image, CAControlState state);
    CAImage* getBackgroundImageForState(CAControlState state);

    // the glyph image for the plus/increase button
    void setIncrementImage(CAImage* image, CAControlState state);
    CAImage* getIncrementImageForState(CAControlState state);

    // the glyph image for the minus/decrease button
    void setDecrementImage(CAImage* image, CAControlState state);
    CAImage* getDecrementImageForState(CAControlState state);

    
protected:
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
protected:
    void repeat(float dt);
    void click(CATouch* pTouch);
    void action();
    
private:
    CAImage* m_pBackgroundImage[CAControlStateAll];
    CAImage* m_pIncrementImage[CAControlStateAll];
    CAImage* m_pDecrementImage[CAControlStateAll];
    
    CAImageView* m_pBackgroundImageView;
    CAImageView* m_pIncrementImageView;
    CAImageView* m_pDecrementImageView;
    
private:
    enum ActionType{
        ActionNone,
        ActionIncrease,
        ActionDecrease
    };
    int m_actionType;
};

NS_CC_END

#endif /* defined(__CrossApp__CAStepper__) */
