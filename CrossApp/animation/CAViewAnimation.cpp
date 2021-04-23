//
//  CAViewAnimation.cpp
//  CrossApp
//
//  Created by 栗元峰 on 15-1-23.
//  Copyright (c) 2015年 http://www.9miao.com All rights reserved.
//

#include "CAViewAnimation.h"
#include "basics/CAScheduler.h"

NS_CC_BEGIN

class CC_DLL CAViewModel : public CAObject
{
public:
    
    DPoint startPoint;
    DPoint endPoint;
    DPoint deltaPoint;
    bool bPoint;
    
    DSize startContentSize;
    DSize endContentSize;
    DSize deltaContentSize;
    bool bContentSize;
    
    float startScaleX;
    float endScaleX;
    float deltaScaleX;
    bool bScaleX;
    
    float startScaleY;
    float endScaleY;
    float deltaScaleY;
    bool bScaleY;
    
    int startZOrder;
    int endZOrder;
    int deltaZOrder;
    bool bZOrder;
    
    float startPointZ;
    float endPointZ;
    float deltaPointZ;
    bool bPointZ;
    
    float startSkewX;
    float endSkewX;
    float deltaSkewX;
    bool bSkewX;
    
    float startSkewY;
    float endSkewY;
    float deltaSkewY;
    bool bSkewY;
    
    float startRotationX;
    float endRotationX;
    float deltaRotationX;
    bool bRotationX;
    
    float startRotationY;
    float endRotationY;
    float deltaRotationY;
    bool bRotationY;
    
    float startRotationZ;
    float endRotationZ;
    float deltaRotationZ;
    bool bRotation;
    
    CAColor4B startColor;
    CAColor4B endColor;
    short deltaColor[4];
    bool bColor;
    
    float startAlpha;
    float endAlpha;
    float deltaAlpha;
    bool bAlpha;
    
    DRect startImageRect;
    DRect endImageRect;
    DRect deltaImageRect;
    bool bImageRect;
    
    bool startFlipX;
    bool endFlipX;
    bool bFlipX;
    
    bool startFlipY;
    bool endFlipY;
    bool bFlipY;
    
    CAView* view;

    CAViewModel(CAView* v);
    
    virtual ~CAViewModel();
    
    static CAViewModel* newModel(CAView* v);
    
    void viewsAnimation(float t);
    
    void viewsAnimationStart();

    void viewsAnimationEnd();

    void getReady();
    
    bool bReady;
};


CAViewModel::CAViewModel(CAView* v)
:view(v)
,bPoint(false)
,bContentSize(false)
,bScaleX(false)
,bScaleY(false)
,bZOrder(false)
,bPointZ(false)
,bSkewX(false)
,bSkewY(false)
,bRotationX(false)
,bRotationY(false)
,bRotation(false)
,bColor(false)
,bAlpha(false)
,bImageRect(false)
,bFlipX(false)
,bFlipY(false)
,bReady(false)
{
    CC_SAFE_RETAIN(view);
}

CAViewModel::~CAViewModel()
{
    if (view)
    {
        view->autorelease();
        view = nullptr;
    }
}

CAViewModel* CAViewModel::newModel(CAView* v)
{
    return new CAViewModel(v);
}

void CAViewModel::viewsAnimation(float t)
{
    view->m_bIsAnimation = true;
    
    if (this->bScaleX)
    {
        view->setScaleX(this->startScaleX + this->deltaScaleX * t);
    }
    if (this->bScaleY)
    {
        view->setScaleY(this->startScaleY + this->deltaScaleY * t);
    }
    if (this->bPoint)
    {
        view->setPoint(this->startPoint + this->deltaPoint * t);
    }
    if (this->bContentSize)
    {
        view->setContentSize(this->startContentSize + this->deltaContentSize * t);
    }
    if (this->bZOrder)
    {
        view->setZOrder(this->startZOrder + this->deltaZOrder * t);
    }
    if (this->bPointZ)
    {
        view->setPointZ(this->startPointZ + this->deltaPointZ * t);
    }
    if (this->bSkewX)
    {
        view->setSkewX(this->startSkewX + this->deltaSkewX * t);
    }
    if (this->bSkewY)
    {
        view->setSkewY(this->startSkewY + this->deltaSkewY * t);
    }
    if (this->bRotation)
    {
        view->setRotation(this->startRotationZ + this->deltaRotationZ * t);
    }
    if (this->bRotationX)
    {
        view->setRotationX(this->startRotationX + this->deltaRotationX * t);
    }
    if (this->bRotationY)
    {
        view->setRotationY(this->startRotationY + this->deltaRotationY * t);
    }
    
    if (this->bColor)
    {
        short r = this->startColor.r + this->deltaColor[0] * t;
        short g = this->startColor.g + this->deltaColor[1] * t;
        short b = this->startColor.b + this->deltaColor[2] * t;
        short a = this->startColor.a + this->deltaColor[3] * t;
        view->setColor(CAColor4B(r, g, b, a));
    }
    if (this->bAlpha)
    {
        view->setAlpha(this->startAlpha + this->deltaAlpha * t);
    }
    if (this->bImageRect)
    {
        DRect rect;
        rect.origin = this->startImageRect.origin + this->deltaImageRect.origin * t;
        rect.size = this->startImageRect.size + this->deltaImageRect.size * t;
        view->setImageRect(rect);
    }
    view->m_bIsAnimation = false;
}

void CAViewModel::viewsAnimationStart()
{
    if (this->bScaleX)
    {
        view->setScaleX(this->startScaleX);
    }
    if (this->bScaleY)
    {
        view->setScaleY(this->startScaleY);
    }
    if (this->bPoint)
    {
        view->setPoint(this->startPoint);
    }
    if (this->bContentSize)
    {
        view->setContentSize(this->startContentSize);
    }
    if (this->bZOrder)
    {
        view->setZOrder(this->startZOrder);
    }
    if (this->bPointZ)
    {
        view->setPointZ(this->startPointZ);
    }
    if (this->bSkewX)
    {
        view->setSkewX(this->startSkewX);
    }
    if (this->bSkewY)
    {
        view->setSkewY(this->startSkewY);
    }
    if (this->bRotation)
    {
        view->setRotation(this->startRotationZ);
    }
    if (this->bRotationX)
    {
        view->setRotationX(this->startRotationX);
    }
    if (this->bRotationY)
    {
        view->setRotationY(this->startRotationY);
    }
    if (this->bColor)
    {
        view->setColor(this->startColor);
    }
    if (this->bAlpha)
    {
        view->setAlpha(this->startAlpha);
    }
    if (this->bImageRect)
    {
        DRect rect;
        rect.origin = this->startImageRect.origin;
        rect.size = this->startImageRect.size;
        view->setImageRect(rect);
    }
    if (this->bFlipX)
    {
        view->setFlipX(this->startFlipX);
    }
    if (this->bFlipY)
    {
        view->setFlipY(this->startFlipY);
    }
}

void CAViewModel::viewsAnimationEnd()
{
    if (this->bScaleX)
    {
        view->setScaleX(this->endScaleX);
    }
    if (this->bScaleY)
    {
        view->setScaleY(this->endScaleY);
    }
    if (this->bPoint)
    {
        view->setPoint(this->endPoint);
    }
    if (this->bContentSize)
    {
        view->setContentSize(this->endContentSize);
    }
    if (this->bZOrder)
    {
        view->setZOrder(this->endZOrder);
    }
    if (this->bPointZ)
    {
        view->setPointZ(this->endPointZ);
    }
    if (this->bSkewX)
    {
        view->setSkewX(this->endSkewX);
    }
    if (this->bSkewY)
    {
        view->setSkewY(this->endSkewY);
    }
    if (this->bRotation)
    {
        view->setRotation(this->endRotationZ);
    }
    if (this->bRotationX)
    {
        view->setRotationX(this->endRotationX);
    }
    if (this->bRotationY)
    {
        view->setRotationY(this->endRotationY);
    }
    if (this->bColor)
    {
        view->setColor(this->endColor);
    }
    if (this->bAlpha)
    {
        view->setAlpha(this->endAlpha);
    }
    if (this->bImageRect)
    {
        DRect rect;
        rect.origin = this->endImageRect.origin;
        rect.size = this->endImageRect.size;
        view->setImageRect(rect);
    }
    if (this->bFlipX)
    {
        view->setFlipX(this->endFlipX);
    }
    if (this->bFlipY)
    {
        view->setFlipY(this->endFlipY);
    }
}

void CAViewModel::getReady()
{
    CC_RETURN_IF(bReady);
    
    startPoint = view->m_obPoint;
    startContentSize = view->m_obContentSize;
    startScaleX = view->m_fScaleX;
    startScaleY = view->m_fScaleY;
    startZOrder = view->m_nZOrder;
    startPointZ = view->m_fPointZ;
    startSkewX = view->m_fSkewX;
    startSkewY = view->m_fSkewY;
    startRotationX = view->m_fRotationX;
    startRotationY = view->m_fRotationY;
    startRotationZ = view->m_fRotationZ;
    startColor = view->_realColor;
    startAlpha = view->_realAlpha;
    startImageRect = view->m_obRect;
    startFlipX = view->m_bFlipX;
    startFlipY = view->m_bFlipY;
    
    if (bPoint)
    {
        deltaPoint = endPoint - startPoint;
    }
    if (bContentSize)
    {
        deltaContentSize = endContentSize - startContentSize;
    }
    if (bScaleX)
    {
        deltaScaleX = endScaleX - startScaleX;
    }
    if (bScaleY)
    {
        deltaScaleY = endScaleY - startScaleY;
    }
    if (bZOrder)
    {
        deltaZOrder = endZOrder - startZOrder;
    }
    if (bPointZ)
    {
        deltaPointZ = endPointZ - startPointZ;
    }
    if (bSkewX)
    {
        deltaSkewX = endSkewX - startSkewX;
    }
    if (bSkewY)
    {
        deltaSkewY = endSkewY - startSkewY;
    }
    if (bRotation)
    {
        deltaRotationZ = endRotationZ - startRotationZ;
    }
    if (bRotationX)
    {
        deltaRotationX = endRotationX - startRotationX;
    }
    if (bRotationY)
    {
        deltaRotationY = endRotationY - startRotationY;
    }
    
    if (bColor)
    {
        deltaColor[0] = (short)endColor.r - (short)startColor.r;
        deltaColor[1] = (short)endColor.g - (short)startColor.g;
        deltaColor[2] = (short)endColor.b - (short)startColor.b;
        deltaColor[3] = (short)endColor.a - (short)startColor.a;
    }
    if (bAlpha)
    {
        deltaAlpha = endAlpha - startAlpha;
    }
    if (bImageRect)
    {
        deltaImageRect.origin = endImageRect.origin - startImageRect.origin;
        deltaImageRect.size = endImageRect.size - startImageRect.size;
    }
    
    bReady = true;
}

namespace CAViewAnimation_Curve {

namespace Sine
{
    inline float EaseOut(float t,float b,float c)
    {
        t = c * std::sin(t * (M_PI/2)) + b;
        return MIN(1.0, t);
    }

    inline float EaseIn(float t,float b,float c)
    {
        t= c * (1 - std::cos(t * (M_PI/2))) + b;
        return MIN(1.0, t);
    }

    inline float EaseInOut(float t,float b,float c)
    {
        float split = 0.8;
        if (t < split)
        {
            t = c*split * (1 - std::cos(t/split * (M_PI/2))) + b;
            return MIN(split, t);
        }
        else
        {
            t = split + c*(1-split) * std::sin((t-split)/(1-split) * (M_PI/2)) + b;
            return MIN(1.0, t);
        }
    }
}

namespace Back
{
    inline float EaseOut(float t,float b,float c)
    {
        float s = 1.70158;
        t=t-1;
        return c*(t*t*((s+1)*t + s) + 1) + b;

    }

    inline float EaseIn(float t,float b,float c)
    {
        float s = 1.70158;
        return c*t*t*((s+1)*t - s) + b;
    }

    inline float EaseInOut(float t,float b,float c)
    {
        t *= 2;
        float s = 1.70158;
        s*=1.525;
        if (t < 1)
            return c/2*t*t*((s+1)*t - s) + b;
        
        t-=2;
        return c/2*(t*t*((s+1)*t + s) + 2) + b;
    }
}

namespace Quad
{
    inline float EaseOut(float t,float b,float c)
    {
        t = -c*t*(t-2) + b;
        return MIN(1.0, t);
    }

    inline float EaseIn(float t,float b,float c)
    {
        t = c*t*t + b;
        return MIN(1.0, t);
    }

    inline float EaseInOut(float t,float b,float c)
    {
        t *= 2;
        if (t < 1)
        {
            t = 0.5*c*t*t + b;
            return MIN(1.0, t);
        }
        else
        {
            t -= 1;
            t = -0.5*c*(t*(t-2)-1) + b;
            return MIN(1.0, t);
        }
    }
}

namespace Cubic
{
    inline float EaseOut(float t,float b,float c)
    {
        t -= 1;
        t = c*(t*t*t + 1) + b;
        return MIN(1.0, t);
    }

    inline float EaseIn(float t,float b,float c)
    {
        t = c*t*t*t + b;
        return MIN(1.0, t);
    }

    inline float EaseInOut(float t,float b,float c)
    {
        t *= 2;
        if (t < 1)
        {
            t = 0.5*c*t*t + b;
            return MIN(1.0, t);
        }
        else
        {
            t -= 2;
            t = 0.5*c*(t*t*t+2) + b;
            return MIN(1.0, t);
        }
    }
}

namespace Quart
{
    inline float EaseOut(float t,float b,float c)
    {
        t -= 1;
        t = -c * (t*t*t*t - 1) + b;
        return MIN(1.0, t);
    }

    inline float EaseIn(float t,float b,float c)
    {
        t = c*t*t*t*t + b;
        return MIN(1.0, t);
    }

    inline float EaseInOut(float t,float b,float c)
    {
        t *= 2;
        if (t < 1)
        {
            t = 0.5*c*t*t*t*t + b;
            return MIN(1.0, t);
        }
        else
        {
            t -= 2;
            t = -0.5*c*(t*t*t*t-2) + b;
            return MIN(1.0, t);
        }
    }
}

namespace Expo
{
    inline float EaseOut(float t,float b,float c)
    {
        t = (t == 1) ? b+c : c * (-std::powf(2, -10 * t) + 1) + b;
        return MIN(1.0, t);
    }

    inline float EaseIn(float t,float b,float c)
    {
        t = (t == 0) ? b : c * std::powf(2, 10 * (t - 1)) + b;
        return MIN(1.0, t);
    }

    inline float EaseInOut(float t,float b,float c)
    {
        if (t == 0)
        {
            t = b;
        }
        else if (t == 1)
        {
            t = b + c;
        }
        else if (t < 0.5)
        {
            t = 0.5 * c * std::powf(2, 10 * (t - 1)) + b;
        }
        else
        {
            t -= 1;
            t = 0.5 * c * (-std::powf(2, -10 * t) + 2) + b;
        }
        return MIN(1.0, t);
    }
}


}

static CAViewAnimation* _viewAnimation = NULL;

CAViewAnimation* CAViewAnimation::getInstance()
{
    if (_viewAnimation == NULL)
    {
        _viewAnimation = new CAViewAnimation();
    }
    return _viewAnimation;
}

void CAViewAnimation::destroyInstance()
{    
    CC_SAFE_DELETE(_viewAnimation);
    _viewAnimation = nullptr;
}

CAViewAnimation::CAViewAnimation()
:m_bAnimationsEnabled(true)
,m_bBeginAnimations(false)
{

}

CAViewAnimation::~CAViewAnimation()
{

}

void CAViewAnimation::beginAnimations(const std::string& animationID)
{
    CAViewAnimation::getInstance()->m_bBeginAnimations = true;
    CAViewAnimation::Module* module = new CAViewAnimation::Module();
    module->animationID = animationID;
    CAViewAnimation::getInstance()->m_vWillModules.pushBack(module);
    module->release();
}

void CAViewAnimation::commitAnimations()
{
    CAViewAnimation* manager = CAViewAnimation::getInstance();
    manager->m_bBeginAnimations = false;
    
    if (!manager->m_vWillModules.empty())
    {
        manager->m_vModules.pushBack(manager->m_vWillModules.front());
        manager->m_vWillModules.popFront();
    }
    
    CAScheduler::getScheduler()->scheduleUpdate(manager, CAScheduler::PRIORITY_SYSTEM, false);
}

void CAViewAnimation::setAnimationDuration(float duration)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    duration = MAX(1/60.0f, duration);
    animation->m_vWillModules.back()->duration = duration;
}

void CAViewAnimation::setAnimationDelay(float delay)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    delay = MAX(0, delay);
    animation->m_vWillModules.back()->delay = delay;
}

void CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve curve)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->curve = curve;
}

void CAViewAnimation::setAnimationCurveCallback(const CAViewAnimation::CurveCallback &function)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->curveFunction = function;
}

void CAViewAnimation::setAnimationRepeatCount(unsigned int repeatCount)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    repeatCount = MAX(0, repeatCount);
    animation->m_vWillModules.back()->repeatCount = repeatCount;
}

void CAViewAnimation::setAnimationRepeatAutoreverses(bool repeatAutoreverses)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->repeatAutoreverses = repeatAutoreverses;
}

void CAViewAnimation::setAnimationWillStartSelector(const std::function<void()>& function)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->willStartFunction = function;
}

void CAViewAnimation::setAnimationDidStopSelector(const std::function<void()>& function)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->didStopFunction = function;
}

void CAViewAnimation::removeAnimations(const std::string& animationID)
{
    CCAssert(animationID.length() > 0, "");
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    
    for (auto itr=animation->m_vWillModules.begin(); itr!=animation->m_vWillModules.end();)
    {
        if ((*itr)->animationID.compare(animationID) == 0)
        {
            itr = animation->m_vWillModules.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (auto itr=animation->m_vModules.begin(); itr!=animation->m_vModules.end();)
    {
        if ((*itr)->animationID.compare(animationID) == 0)
        {
            itr = animation->m_vModules.erase(itr);
        }
        else
        {
            itr++;
        }
    }
}

void CAViewAnimation::removeAnimationsWithView(CAView* view)
{
    CC_RETURN_IF(view == NULL);
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    
    for (auto& module : animation->m_vWillModules)
    {
        module->animations.erase(view);
    }
    
    for (auto& module : animation->m_vModules)
    {
        module->animations.erase(view);
    }
}

void CAViewAnimation::removeAllAnimations()
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    
    CAScheduler::getScheduler()->unscheduleAllForTarget(animation);
    animation->m_vWillModules.clear();
    animation->m_vModules.clear();
}

void CAViewAnimation::setAnimationsEnabled(bool enabled)
{
    CAViewAnimation::getInstance()->m_bAnimationsEnabled = enabled;
}

bool CAViewAnimation::areAnimationsEnabled()
{
    return CAViewAnimation::getInstance()->m_bAnimationsEnabled;
}

bool CAViewAnimation::areBeginAnimations()
{
    return CAViewAnimation::getInstance()->m_bBeginAnimations;
}

bool CAViewAnimation::areBeginAnimationsWithID(const std::string& animationID)
{
    bool bRet = false;
    
    const CAVector<CAViewAnimation::Module*>& willModule = CAViewAnimation::getInstance()->m_vWillModules;
    for (auto& var : willModule)
    {
        if (var->animationID.compare(animationID) == 0)
        {
            bRet = true;
            break;
        }
        
    }
    
    if (bRet == false)
    {
        const CAVector<CAViewAnimation::Module*>& module = CAViewAnimation::getInstance()->m_vModules;
        for (auto& var : module)
        {
            if (var->animationID.compare(animationID) == 0)
            {
                bRet = true;
                break;
            }
            
        }
    }
    return bRet;
}

void CAViewAnimation::update(float dt)
{
    auto modules = CAVector<CAViewAnimation::Module*>(m_vModules);
    m_vModules.clear();
	for (auto& module : modules)
    {
        float time = module->time - module->delay;

        if (time > -FLT_MIN)
        {
            if (!module->alreadyRunning)
            {
                if (module->willStartFunction)
                {
                    module->willStartFunction();
                    module->willStartFunction = nullptr;
                }
                
				CAMap<CAView*, CAObject*>& animations = module->animations;
                CAMap<CAView*, CAObject*>::iterator itr_animation = animations.begin();
                while (itr_animation != animations.end())
                {
                    CAView* view = itr_animation->first;
                    view->m_bIsAnimation = true;
                    CAViewModel* model = (CAViewModel*)(itr_animation->second);
                    model->getReady();
                    ++itr_animation;
                }
                module->alreadyRunning = true;
            }
            
            bool isReverses = module->repeatAutoreverses ? ((int)module->repeatTimes) % 2 == 1 : false;
            if (module->time - module->delay >= module->duration)
            {
                module->time = module->delay;
                module->repeatTimes += 1;
                
                if (isReverses)
                {
                    CAMap<CAView*, CAObject*>& animations = module->animations;
                    auto itr_animation = animations.begin();
                    while (itr_animation != animations.end())
                    {
                        CAViewModel* model = (CAViewModel*)(itr_animation->second);
                        model->viewsAnimationStart();
                        ++itr_animation;
                    }
                }
                else
                {
                    CAMap<CAView*, CAObject*>& animations = module->animations;
                    auto itr_animation = animations.begin();
                    while (itr_animation != animations.end())
                    {
                        CAViewModel* model = (CAViewModel*)(itr_animation->second);
                        model->viewsAnimationEnd();
                        ++itr_animation;
                    }
                }
            }
            
            if (module->repeatTimes >= module->repeatCount && (module->repeatCount > 0 && module->repeatCount < 1048576))
            {
                if (module->didStopFunction)
                {
                    module->didStopFunction();
                    module->didStopFunction = nullptr;
                }
                continue;
            }
            
            float t_before = time / module->duration;
            float b = dt / module->duration;
            float c = 1;
            
            if (isReverses)
            {
                t_before = 1.0f - t_before;
                b = -b;
            }
            float t_after = 0;
//            CCLog("before: %f", MIN(t_before+b, 1.0f));
            if (module->curveFunction != nullptr)
            {
                t_after = module->curveFunction(t_before, b, c);
            }
            else
            {
                switch (module->curve)
                {
                    case CAViewAnimation::Curve::EaseSineOut:
                    {
                        t_after = CAViewAnimation_Curve::Sine::EaseOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseSineIn:
                    {
                        t_after = CAViewAnimation_Curve::Sine::EaseIn(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseSineInOut:
                    {
                        t_after = CAViewAnimation_Curve::Sine::EaseInOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseBackOut:
                    {
                        t_after = CAViewAnimation_Curve::Back::EaseOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseBackIn:
                    {
                        t_after = CAViewAnimation_Curve::Back::EaseIn(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseBackInOut:
                    {
                        t_after = CAViewAnimation_Curve::Back::EaseInOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseQuadOut:
                    {
                        t_after = CAViewAnimation_Curve::Quad::EaseOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseQuadIn:
                    {
                        t_after = CAViewAnimation_Curve::Quad::EaseIn(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseQuadInOut:
                    {
                        t_after = CAViewAnimation_Curve::Quad::EaseInOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseCubicOut:
                    {
                        t_after = CAViewAnimation_Curve::Cubic::EaseOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseCubicIn:
                    {
                        t_after = CAViewAnimation_Curve::Cubic::EaseIn(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseCubicInOut:
                    {
                        t_after = CAViewAnimation_Curve::Cubic::EaseInOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseQuartOut:
                    {
                        t_after = CAViewAnimation_Curve::Quart::EaseOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseQuartIn:
                    {
                        t_after = CAViewAnimation_Curve::Quart::EaseIn(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseQuartInOut:
                    {
                        t_after = CAViewAnimation_Curve::Quart::EaseInOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseExpoOut:
                    {
                        t_after = CAViewAnimation_Curve::Expo::EaseOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseExpoIn:
                    {
                        t_after = CAViewAnimation_Curve::Expo::EaseIn(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::EaseExpoInOut:
                    {
                        t_after = CAViewAnimation_Curve::Expo::EaseInOut(t_before, b, c);
                    }
                        break;
                    case CAViewAnimation::Curve::Linear:
                    default:
                        t_after = MIN(1.0, t_before + b);
                        break;
                }
            }
//            CCLog("after: %f\n", t_after);

			CAMap<CAView*, CAObject*>& animations = module->animations;
            auto itr_animation = animations.begin();
            while (itr_animation != animations.end())
            {
                CAViewModel* model = (CAViewModel*)(itr_animation->second);
                model->viewsAnimation(t_after);
                ++itr_animation;
            }
        }
        
        module->time += dt;
        m_vModules.pushBack(module);
    }
    modules.clear();
    
    if (m_vModules.empty())
    {
        CAScheduler::getScheduler()->scheduleUpdate(this, CAScheduler::PRIORITY_SYSTEM, true);
    }
}

void CAViewAnimation::setPoint(const DPoint& point, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bPoint = true;
    model->endPoint = point;
}

void CAViewAnimation::setContentSize(const DSize& size, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bContentSize = true;
    model->endContentSize = size;
}

void CAViewAnimation::setScaleX(float scaleX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bScaleX = true;
    model->endScaleX = scaleX;
}

void CAViewAnimation::setScaleY(float scaleY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bScaleY = true;
    model->endScaleY = scaleY;
}

void CAViewAnimation::setZOrder(int zOrder, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bZOrder = true;
    model->endZOrder = zOrder;
}

void CAViewAnimation::setPointZ(float PointZ, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bPointZ = true;
    model->endPointZ = PointZ;
}

void CAViewAnimation::setSkewX(float skewX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bSkewX = true;
    model->endSkewX = skewX;
}

void CAViewAnimation::setSkewY(float skewY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bSkewY = true;
    model->endSkewY = skewY;
}

void CAViewAnimation::setRotation(float rotation, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bRotation = true;
    model->endRotationZ = model->endRotationZ = rotation;
}

void CAViewAnimation::setRotationX(float rotationX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bRotationX = true;
    model->endRotationX = rotationX;
}

void CAViewAnimation::setRotationY(float rotationY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bRotationY = true;
    model->endRotationY = rotationY;
}

void CAViewAnimation::setColor(const CAColor4B& color, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bColor = true;
    model->endColor = color;
}

void CAViewAnimation::setAlpha(float alpha, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bAlpha = true;
    model->endAlpha = alpha;
}

void CAViewAnimation::setImageRect(const DRect& imageRect, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bImageRect = true;
    model->endImageRect = imageRect;
}

void CAViewAnimation::setFlipX(bool flipX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bFlipX = true;
    model->endFlipX = flipX;
}

void CAViewAnimation::setFlipY(bool flipY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.at(view);
    model->bFlipY = true;
    model->endFlipY = flipY;
}

void CAViewAnimation::allocCAViewModel(CAView* view)
{
    CAViewAnimation::Module* module = m_vWillModules.back();
    if (module->animations.at(view) == NULL)
    {
        auto model = CAViewModel::newModel(view);
        module->animations.insert(view, model);
        model->release();
    }

}




NS_CC_END

