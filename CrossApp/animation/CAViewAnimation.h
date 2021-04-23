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

class CAViewModel;
class CC_DLL CAViewAnimation: public CAObject
{
public:
    
    enum class Curve : int
    {
        Linear = 0,
        
        EaseSineOut,
        EaseSineIn,
        EaseSineInOut,
        
        EaseBackOut,
        EaseBackIn,
        EaseBackInOut,
        
        EaseQuadOut,
        EaseQuadIn,
        EaseQuadInOut,
        
        EaseCubicOut,
        EaseCubicIn,
        EaseCubicInOut,
        
        EaseQuartOut,
        EaseQuartIn,
        EaseQuartInOut,
        
        EaseExpoOut,
        EaseExpoIn,
        EaseExpoInOut,
        
        EaseOut = EaseSineOut,              // slow at end
        EaseIn = EaseSineIn,                // slow at beginning
        EaseInOut = EaseSineInOut           // slow at beginning and end
    };

    typedef std::function<float(float t,float b,float c)> CurveCallback;

private:
        struct Module : public CAObject
        {
            std::string                     animationID;
            CAMap<CAView*, CAObject*>       animations;
            float                           duration{0.2f};
            float                           delay{0.0f};
            unsigned int                    repeatCount{1};
            unsigned int                    repeatTimes{0};
            bool                            repeatAutoreverses{false};
            float                           time{0.0f};
            CAViewAnimation::Curve          curve{CAViewAnimation::Curve::Linear};
            CAViewAnimation::CurveCallback  curveFunction{nullptr};
            std::function<void()>           willStartFunction{nullptr};
            std::function<void()>           didStopFunction{nullptr};
            bool                            alreadyRunning{false};
        };
        
public:
        
    static void beginAnimations(const std::string& animationID);
    
    static void commitAnimations();
    
    static void setAnimationDuration(float duration);// default(0.2)
    
    static void setAnimationDelay(float delay);// default(0)
    
    static void setAnimationCurve(CAViewAnimation::Curve curve);// default(CAViewAnimation::Curve::Linear)
    
    static void setAnimationCurveCallback(const CAViewAnimation::CurveCallback& function); // t, b, c and return t;

    static void setAnimationRepeatCount(unsigned int repeatCount);// default(1.0)
    
    static void setAnimationRepeatAutoreverses(bool repeatAutoreverses);// default(false), if true, repeatCount must be greater than 1;
    
    static void setAnimationWillStartSelector(const std::function<void()>& function);

    static void setAnimationDidStopSelector(const std::function<void()>& function);
    
    static void removeAnimations(const std::string& animationID);
    
    static void removeAnimationsWithView(CAView* view);

    static void removeAllAnimations();
        
    static void setAnimationsEnabled(bool enabled);
    
    static bool areAnimationsEnabled();

    static bool areBeginAnimations();
    
    static bool areBeginAnimationsWithID(const std::string& animationID);
        
protected:

    static CAViewAnimation* getInstance();
    
    static void destroyInstance();
        
    CAViewAnimation();
    
    virtual ~CAViewAnimation();

    void setPoint(const DPoint& point, CAView* view);
    
    void setContentSize(const DSize& size, CAView* view);
    
    void setScaleX(float scaleX, CAView* view);
    
    void setScaleY(float scaleY, CAView* view);
    
    void setZOrder(int zOrder, CAView* view);
    
    void setPointZ(float PointZ, CAView* view);
    
    void setSkewX(float skewX, CAView* view);
    
    void setSkewY(float skewY, CAView* view);
    
    void setRotation(float rotation, CAView* view);
    
    void setRotationX(float rotationX, CAView* view);
    
    void setRotationY(float rotationY, CAView* view);
    
    void setColor(const CAColor4B& color, CAView* view);
    
    void setAlpha(float alpha, CAView* view);
    
    void setImageRect(const DRect& imageRect, CAView* view);
        
    void setFlipX(bool flipX, CAView* view);
    
    void setFlipY(bool flipY, CAView* view);
    
    void allocCAViewModel(CAView* view);
    
    void update(float dt);
    
    bool m_bAnimationsEnabled;
    
    bool m_bBeginAnimations;
    
    CADeque<CAViewAnimation::Module*> m_vWillModules;
        
    CAVector<CAViewAnimation::Module*> m_vModules;
        
    friend class CAView;
    
    friend class CAImageView;
    
    friend class CAScale9ImageView;
        
    friend class CAApplication;
};

NS_CC_END
#endif /* defined(__CrossApp__CAViewAnimation__) */
