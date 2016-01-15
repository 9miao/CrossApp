//
//  CAStepper.h
//  CrossApp
//
//  Created by dai xinping on 14-8-15.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
//

#ifndef __CrossApp__CAStepper__
#define __CrossApp__CAStepper__

#include <iostream>
#include "CAControl.h"

NS_CC_BEGIN

typedef enum
{
    CAStepperOrientationHorizontal,
    CAStepperOrientationVertical
}CAStepperOrientation;

class CAImageView;
class CAScale9ImageView;
class CC_DLL CAStepper : public CAControl {
    
public:
    CAStepper(const CAStepperOrientation& type = CAStepperOrientationHorizontal);
    virtual ~CAStepper();

    static CAStepper* create(const CAStepperOrientation& type = CAStepperOrientationHorizontal);
    static CAStepper* createWithFrame(const DRect& rect
                                      , const CAStepperOrientation& type = CAStepperOrientationHorizontal);
    static CAStepper* createWithCenter(const DRect& rect
                                       , const CAStepperOrientation& type = CAStepperOrientationHorizontal);
    
    virtual bool init();
    
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
    // a Background image which will be 3-way stretched over the whole of the control. Each half of the stepper will paint the image appropriate for its state      
    void setBackgroundImage(CAImage* image, CAControlState state);
    CAImage* getBackgroundImageForState(CAControlState state);

    // the glyph image for the plus/increase button
    void setIncrementImage(CAImage* image, CAControlState state);
    CAImage* getIncrementImageForState(CAControlState state);

    // the glyph image for the minus/decrease button
    void setDecrementImage(CAImage* image, CAControlState state);
    CAImage* getDecrementImageForState(CAControlState state);
    
    // an image which will be painted in between the two stepper segments. The image is selected depending both segments' state
    void setDividerColor(CAColor4B color);
    CAColor4B getDividerColor();
    
    void setTailorImageAtIndex(int index);
    CAView* getTailorImageAtIndex(int index);

    CC_SYNTHESIZE(bool, m_bTouchEffect, TouchEffect); // default is false, alpha

public:

	virtual void addTarget(CAObject* target, SEL_CAControl selector);

	virtual void removeTarget(CAObject* target, SEL_CAControl selector);

	virtual void removeAllTargets();

    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
protected:

    void repeat(float dt);
    void click(CATouch* pTouch);
    void action();
    void setContentSize(const DSize & var);
    
private:
    
    CAImage* m_pBackgroundImage[CAControlStateAll];
    CAImage* m_pIncrementImage[CAControlStateAll];
    CAImage* m_pDecrementImage[CAControlStateAll];
    
    CAScale9ImageView* m_pBackgroundImageView;
    CAImageView* m_pBackgroundSelectedImageView;
    CAImageView* m_pIncrementImageView;
    CAImageView* m_pDecrementImageView;
    CAView* m_pDividerImageView;
    CAColor4B m_cTintColor;
    
    CAStepperOrientation m_pCAStepperOrientation;
    
private:
    
    enum ActionType
    {
        ActionNone,
        ActionIncrease,
        ActionDecrease
    };
    
    int m_actionType;
};

NS_CC_END

#endif /* defined(__CrossApp__CAStepper__) */
