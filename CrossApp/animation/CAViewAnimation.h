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
NS_CC_BEGIN

typedef void (CAObject::*SEL_CAViewAnimation)(const std::string&, void*);
#define CAViewAnimation_selector(_SELECTOR) (SEL_CAViewAnimation)(&_SELECTOR)
class CC_DLL CAViewAnimation: public CAObject
{
public:
    
    typedef enum
    {
        CAViewAnimationCurveLinear,
        CAViewAnimationCurveEaseOut,          // slow at end
        CAViewAnimationCurveEaseIn,           // slow at beginning
        CAViewAnimationCurveEaseInOut         // slow at beginning and end
    }CAViewAnimationCurve;
    
    struct CAViewAnimationModule
    {
        std::string animationID;
        void* context;
        float duration;
        float delay;
        CAViewAnimationCurve curve;
        float time;
        CAMap<CAView*, CAObject*> animations;
        
        CAObject* willStartTarget;
        CAObject* didStopTarget;
        
        SEL_CAViewAnimation willStartSel;
        SEL_CAViewAnimation didStopSel;
        
        CAViewAnimationModule()
        :willStartTarget(NULL)
        ,didStopTarget(NULL)
        ,willStartSel(NULL)
        ,didStopSel(NULL)
        {}
    };
    
private:
    
    std::deque<CAViewAnimation::CAViewAnimationModule> m_vWillModules;
    
    std::vector<CAViewAnimation::CAViewAnimationModule> m_vModules;
    
public:
    
    static void beginAnimations(const std::string& animationID, void* context);
    
    static void commitAnimations();
    
    static void setAnimationDuration(float duration);// default(0.2)
    
    static void setAnimationDelay(float delay);// default(0)
    
    static void setAnimationCurve(const CAViewAnimationCurve& curve);// default(CAViewAnimationCurveLinear
    
    static void setAnimationWillStartSelector(CAObject* target, SEL_CAViewAnimation selector);
    
    static void setAnimationDidStopSelector(CAObject* target, SEL_CAViewAnimation selector);
    
    static void setAnimationsEnabled(bool enabled);
    
    static bool areAnimationsEnabled();

    static bool areBeginAnimations();
    
protected:

    static CAViewAnimation* getInstance();
    
    CAViewAnimation();
    
    virtual ~CAViewAnimation();
    
    void setPoint(const CCPoint& point, CAView* view);
    
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
};

NS_CC_END
#endif /* defined(__CrossApp__CAViewAnimation__) */
