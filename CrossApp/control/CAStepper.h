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

class CAImageView;
class CAScale9ImageView;
class CC_DLL CAStepper : public CAControl
{

public:
    
    enum class Orientation : int
    {
        Horizontal = 0,
        Vertical
    };
    
public:
    
    CAStepper(CAStepper::Orientation type = CAStepper::Orientation::Horizontal);
    virtual ~CAStepper();

    static CAStepper* create(CAStepper::Orientation type = CAStepper::Orientation::Horizontal);
    static CAStepper* createWithFrame(const DRect& rect
                                      , CAStepper::Orientation type = CAStepper::Orientation::Horizontal);
    static CAStepper* createWithCenter(const DRect& rect
                                       , CAStepper::Orientation type = CAStepper::Orientation::Horizontal);
    static CAStepper* createWithLayout(const DLayout& layout
                                      , CAStepper::Orientation type = CAStepper::Orientation::Horizontal);
    virtual bool init();
    
    CC_SYNTHESIZE(bool, m_bAutoRepeat, AutoRepeat); // if true, press & hold repeatedly alters value. default = true
    CC_SYNTHESIZE(bool, m_bWraps, Wraps);           // if true, value wraps from min <-> max. default = false
    
    CC_SYNTHESIZE(float, m_value, Value);              // default is 0. sends UIControlEventValueChanged. clamped to min/max
    CC_SYNTHESIZE(float, m_minimumValue, MinValue);    // default 0. must be less than maximumValue
    CC_SYNTHESIZE(float, m_maximumValue, MaxValue);    // default 100. must be greater than minimumValue
    CC_SYNTHESIZE(float, m_stepValue, StepValue);      // default 1. must be greater than 0
    
//    CC_SYNTHESIZE(CAColor4B, m_tintColor, TintColor);
    
public:
    // a Background image which will be 3-way stretched over the whole of the control. Each half of the stepper will paint the image appropriate for its state      
    void setBackgroundImage(CAStepper::State state, CAImage* image);

    // the glyph image for the plus/increase button
    void setIncrementImage(CAStepper::State state, CAImage* image);

    // the glyph image for the minus/decrease button
    void setDecrementImage(CAStepper::State state, CAImage* image);
    
    // an image which will be painted in between the two stepper segments. The image is selected depending both segments' state
    void setDividerColor(CAColor4B color);
    CAColor4B getDividerColor();
    
    void setTailorImageAtIndex(int index);
    CAView* getTailorImageAtIndex(int index);

    CC_SYNTHESIZE(bool, m_bTouchEffect, TouchEffect); // default is false, alpha

    void setTarget(const std::function<void(float)>& function);
    
public:

    virtual void onExitTransitionDidStart();
    virtual void onEnterTransitionDidFinish();
        
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
    
    std::function<void(float)> m_function{nullptr};
    
    CAMap<CAStepper::State, CAImage*> m_mBackgroundImages;
    CAMap<CAStepper::State, CAImage*> m_mIncrementImages;
    CAMap<CAStepper::State, CAImage*> m_mDecrementImages;
    

    CAScale9ImageView* m_pBackgroundImageView;
    CAImageView* m_pBackgroundHighlightedmageView;
    CAImageView* m_pIncrementImageView;
    CAImageView* m_pDecrementImageView;
    CAView* m_pDividerImageView;
    CAColor4B m_cTintColor;
    
    CAStepper::Orientation m_pOrientation;
    
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
