//
//  CAViewAnimation.h
//  CrossApp
//
//  Created by 栗元峰 on 15-1-23.
//  Copyright (c) 2015年 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAViewAnimation__
#define __CrossApp__CAViewAnimation__

#include "view/CAView.h"
#include "view/CAImageView.h"
#include "view/CAScale9ImageView.h"
NS_CC_BEGIN

typedef void (CAObject::*SEL_CAViewAnimation2)(const std::string&, void*);
#define CAViewAnimation2_selector(_SELECTOR) (SEL_CAViewAnimation2)(&_SELECTOR)

typedef void (CAObject::*SEL_CAViewAnimation0)();
#define CAViewAnimation0_selector(_SELECTOR) (SEL_CAViewAnimation0)(&_SELECTOR)

typedef enum
{
    CAViewAnimationCurveLinear = 0,
    CAViewAnimationCurveEaseOut,          // slow at end
    CAViewAnimationCurveEaseIn,           // slow at beginning
    CAViewAnimationCurveEaseInOut         // slow at beginning and end
}CAViewAnimationCurve;

class CAViewAnimationModule;

class CC_DLL CAViewAnimation: public CAObject
{
    CADeque<CAViewAnimationModule*> m_vWillModules;
    
    CAVector<CAViewAnimationModule*> m_vModules;
    
public:
    
    static void beginAnimations(const std::string& animationID, void* context);
    
    static void commitAnimations();
    
    static void setAnimationDuration(float duration);// default(0.2)
    
    static void setAnimationDelay(float delay);// default(0)
    
    static void setAnimationCurve(const CAViewAnimationCurve& curve);// default(CAViewAnimationCurveLinear
    
    static void setAnimationWillStartSelector(CAObject* target, SEL_CAViewAnimation0 selector);
    
    static void setAnimationWillStartSelector(CAObject* target, SEL_CAViewAnimation2 selector);
    
    static void setAnimationDidStopSelector(CAObject* target, SEL_CAViewAnimation0 selector);
    
    static void setAnimationDidStopSelector(CAObject* target, SEL_CAViewAnimation2 selector);
    
    static void setAnimationsEnabled(bool enabled);
    
    static bool areAnimationsEnabled();

    static bool areBeginAnimations();
    
protected:

    static CAViewAnimation* getInstance();
    
    CAViewAnimation();
    
    virtual ~CAViewAnimation();
    
    void setFrameOrgin(const CCPoint& point, CAView* view);
    
    void setCenterOrgin(const CCPoint& point, CAView* view);
    
    void setContentSize(const CCSize& size, CAView* view);
    
    void setScaleX(float scaleX, CAView* view);
    
    void setScaleY(float scaleY, CAView* view);
    
    void setZOrder(int zOrder, CAView* view);
    
    void setVertexZ(float vertexZ, CAView* view);
    
    void setSkewX(float skewX, CAView* view);
    
    void setSkewY(float skewY, CAView* view);
    
    void setRotationX(float rotationX, CAView* view);
    
    void setRotationY(float rotationY, CAView* view);
    
    void setColor(const CAColor4B& color, CAView* view);
    
    void setAlpha(float alpha, CAView* view);
    
    void setImageRect(const CCRect& imageRect, CAView* view);
    
    void setFlipX(bool flipX, CAView* view);
    
    void setFlipY(bool flipY, CAView* view);
    
    void allocCAViewModel(CAView* view);
    
    void update(float dt);
    
    bool m_bAnimationsEnabled;
    
    bool m_bBeginAnimations;
    
    friend class CAView;
    
    friend class CAImageView;
    
    friend class CAScale9ImageView;
};

NS_CC_END
#endif /* defined(__CrossApp__CAViewAnimation__) */
