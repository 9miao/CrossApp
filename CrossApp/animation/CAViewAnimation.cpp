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
    
    CCPoint startPoint;
    CCPoint endPoint;
    CCPoint deltaPoint;
    bool bPoint;
    
    CCSize startContentSize;
    CCSize endContentSize;
    CCSize deltaContentSize;
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
    
    float startVertexZ;
    float endVertexZ;
    float deltaVertexZ;
    bool bVertexZ;
    
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
    
    CAColor4B startColor;
    CAColor4B endColor;
    short deltaColor[4];
    bool bColor;
    
    float startAlpha;
    float endAlpha;
    float deltaAlpha;
    bool bAlpha;
    
    CCRect startImageRect;
    CCRect endImageRect;
    CCRect deltaImageRect;
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
    
    static CAViewModel* create(CAView* v);
    
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
,bVertexZ(false)
,bSkewX(false)
,bSkewY(false)
,bRotationX(false)
,bRotationY(false)
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
    CC_SAFE_RELEASE(view);
}

CAViewModel* CAViewModel::create(CAView* v)
{
    CAViewModel* model = new CAViewModel(v);
    if (model)
    {
        model->autorelease();
    }
    return model;
}

void CAViewModel::getReady()
{
    CC_RETURN_IF(bReady);
    
    startPoint = view->m_obPoint;
    startContentSize = view->m_obContentSize;
    startScaleX = view->m_fScaleX;
    startScaleY = view->m_fScaleY;
    startZOrder = view->m_nZOrder;
    startVertexZ = view->m_fVertexZ;
    startSkewX = view->m_fSkewX;
    startSkewY = view->m_fSkewY;
    startRotationX = view->m_fRotationX;
    startRotationY = view->m_fRotationY;
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
    if (bVertexZ)
    {
        deltaVertexZ = endVertexZ - startVertexZ;
    }
    if (bSkewX)
    {
        deltaSkewX = endSkewX - startSkewX;
    }
    if (bSkewY)
    {
        deltaSkewY = endSkewY - startSkewY;
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

static CAViewAnimation* _viewAnimation = NULL;

CAViewAnimation* CAViewAnimation::getInstance()
{
    if (_viewAnimation == NULL)
    {
        _viewAnimation = new CAViewAnimation();
    }
    return _viewAnimation;
}

CAViewAnimation::CAViewAnimation()
:m_bAnimationsEnabled(true)
,m_bBeginAnimations(false)
{

}

CAViewAnimation::~CAViewAnimation()
{

}

void CAViewAnimation::beginAnimations(const std::string& animationID, void* context)
{
    CAViewAnimation::getInstance()->m_bBeginAnimations = true;
    CAViewAnimationModule* module = new CAViewAnimationModule();
    module->animationID = animationID;
    module->context = context;
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
    
    do
    {
        CC_BREAK_IF(CAScheduler::isScheduled(schedule_selector(CAViewAnimation::update), manager));
        CAScheduler::schedule(schedule_selector(CAViewAnimation::update), manager, 1/60.0f);
    }
    while (0);
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

void CAViewAnimation::setAnimationCurve(const CAViewAnimationCurve& curve)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->curve = curve;
}

void CAViewAnimation::setAnimationRepeatCount(float repeatCount)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    repeatCount = MAX(0.0f, repeatCount);
    animation->m_vWillModules.back()->repeatCount = repeatCount;
}

void CAViewAnimation::setAnimationRepeatAutoreverses(bool repeatAutoreverses)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->repeatAutoreverses = repeatAutoreverses;
}

void CAViewAnimation::setAnimationWillStartSelector(CAObject* target, SEL_CAViewAnimation0 selector)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->willStartTarget = target;
    animation->m_vWillModules.back()->willStartSel0 = selector;
}

void CAViewAnimation::setAnimationWillStartSelector(CAObject* target, SEL_CAViewAnimation2 selector)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->willStartTarget = target;
    animation->m_vWillModules.back()->willStartSel2 = selector;
}

void CAViewAnimation::setAnimationDidStopSelector(CAObject* target, SEL_CAViewAnimation0 selector)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->didStopTarget = target;
    animation->m_vWillModules.back()->didStopSel0 = selector;
}

void CAViewAnimation::setAnimationDidStopSelector(CAObject* target, SEL_CAViewAnimation2 selector)
{
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    CC_RETURN_IF(animation->m_vWillModules.empty());
    animation->m_vWillModules.back()->didStopTarget = target;
    animation->m_vWillModules.back()->didStopSel2 = selector;
}

void CAViewAnimation::removeAnimations(const std::string& animationID)
{
    CCAssert(animationID.length() > 0, "");
    CAViewAnimation* animation = CAViewAnimation::getInstance();
    
    for (CADeque<CAViewAnimationModule*>::iterator itr=animation->m_vWillModules.begin();
         itr!=animation->m_vWillModules.end();)
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
    
    for (CAVector<CAViewAnimationModule*>::iterator itr=animation->m_vModules.begin();
         itr!=animation->m_vModules.end();)
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
    
    for (CADeque<CAViewAnimationModule*>::iterator itr=animation->m_vWillModules.begin();
         itr!=animation->m_vWillModules.end(); itr++)
    {
        CAViewAnimationModule* module = *itr;
        module->animations.erase(view);
    }
    
    for (CAVector<CAViewAnimationModule*>::iterator itr=animation->m_vModules.begin();
         itr!=animation->m_vModules.end(); itr++)
    {
        CAViewAnimationModule* module = *itr;
        module->animations.erase(view);
    }
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
    
    CADeque<CAViewAnimationModule*>* willModule = &CAViewAnimation::getInstance()->m_vWillModules;
    CADeque<CAViewAnimationModule*>::const_iterator itr = willModule->begin();
    for (; itr!=willModule->end(); itr++)
    {
        if ((*itr)->animationID.compare(animationID) == 0)
        {
            bRet = true;
            break;
        }
        
    }
    
    if (bRet == false)
    {
        CAVector<CAViewAnimationModule*>* module = &CAViewAnimation::getInstance()->m_vModules;
        CAVector<CAViewAnimationModule*>::const_iterator itr2 = module->begin();
        for (; itr2!=module->end(); itr2++)
        {
            if ((*itr2)->animationID.compare(animationID) == 0)
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
    CAVector<CAViewAnimationModule*> modules = CAVector<CAViewAnimationModule*>(m_vModules);
    m_vModules.clear();
    CAVector<CAViewAnimationModule*>::iterator itr_module = modules.begin();
    while (itr_module != modules.end())
    {
        CAViewAnimationModule* module = *itr_module;
        module->time += dt;
        float time = module->time - module->delay;

        if (time > -FLT_MIN)
        {
            if (!module->isAlreadyRunning())
            {
                if (module->willStartSel2 && module->willStartTarget)
                {
                    ((CAObject *)module->willStartTarget->*module->willStartSel2)(module->animationID, module->context);
                    module->willStartSel2 = NULL;
                }
                else if (module->willStartSel0 && module->willStartTarget)
                {
                    ((CAObject *)module->willStartTarget->*module->willStartSel0)();
                    module->willStartSel0 = NULL;
                }
                
                CAMap<CAView*, CAObject*>& animations = (*itr_module)->animations;
                CAMap<CAView*, CAObject*>::iterator itr_animation = animations.begin();
                while (itr_animation != animations.end())
                {
                    CAView* view = itr_animation->first;
                    view->m_bIsAnimation = true;
                    CAViewModel* model = (CAViewModel*)(itr_animation->second);
                    model->getReady();
                    ++itr_animation;
                }
                module->setAlreadyRunning(true);
            }
            
            float times = 0;
            do
            {
                CC_BREAK_IF(time < module->duration);
                CC_BREAK_IF(time - dt < module->duration);
                time -= module->duration;
                times += 1.0f;
            }
            while (true);
            
            bool isReverses = module->repeatAutoreverses ? ((int)times) % 2 == 1 : false;
            float s = MIN(time / module->duration, 1.0f);
            times += s;
            times = module->repeatAutoreverses ? times / 2 : times;
            s = isReverses ? 1.0f - s : s;

            switch (module->curve)
            {
                case CAViewAnimationCurveEaseOut:
                {
                    s = (s + sqrtf(1 - powf(1 - s, 2))) / 2;
                }
                    break;
                case CAViewAnimationCurveEaseIn:
                {
                    s = (s + 1 - sqrtf(1 - powf(s, 2))) / 2;
                }
                    break;
                case CAViewAnimationCurveEaseInOut:
                {
                    s = (s < 0.5f)
                        ? (s + 0.5f - sqrtf(0.25f - powf(s, 2))) / 2
                        : (s + sqrtf(0.25 - powf(1 - s, 2)) + 0.5f) / 2;
                }
                    break;
                default:
                    break;
            }
            

            CAMap<CAView*, CAObject*>& animations = (*itr_module)->animations;
            CAMap<CAView*, CAObject*>::iterator itr_animation = animations.begin();
            while (itr_animation != animations.end())
            {
                CAView* view = itr_animation->first;
                view->m_bIsAnimation = true;
                CAViewModel* model = (CAViewModel*)(itr_animation->second);
                
                if (model->bScaleX)
                {
                    view->setScaleX(model->startScaleX + model->deltaScaleX * s);
                }
                if (model->bScaleY)
                {
                    view->setScaleY(model->startScaleY + model->deltaScaleY * s);
                }
                if (model->bPoint)
                {
                    view->setPoint(model->startPoint + model->deltaPoint * s);
                }
                if (model->bContentSize)
                {
                    view->setContentSize(model->startContentSize + model->deltaContentSize * s);
                }
                if (model->bZOrder)
                {
                    view->setZOrder(model->startZOrder + model->deltaZOrder * s);
                }
                if (model->bVertexZ)
                {
                    view->setVertexZ(model->startVertexZ + model->deltaVertexZ * s);
                }
                if (model->bSkewX)
                {
                    view->setSkewX(model->startSkewX + model->deltaSkewX * s);
                }
                if (model->bSkewY)
                {
                    view->setSkewY(model->startSkewY + model->deltaSkewY * s);
                }
                if (model->bRotationX)
                {
                    view->setRotationX(model->startRotationX + model->deltaRotationX * s);
                }
                if (model->bRotationY)
                {
                    view->setRotationY(model->startRotationY + model->deltaRotationY * s);
                }
                if (model->bColor)
                {
                    short r = model->startColor.r + model->deltaColor[0] * s;
                    short g = model->startColor.g + model->deltaColor[1] * s;
                    short b = model->startColor.b + model->deltaColor[2] * s;
                    short a = model->startColor.a + model->deltaColor[3] * s;
                    view->setColor(ccc4(r, g, b, a));
                }
                if (model->bAlpha)
                {
                    view->setAlpha(model->startAlpha + model->deltaAlpha * s);
                }
                if (model->bImageRect)
                {
                    CCRect rect;
                    rect.origin = model->startImageRect.origin + model->deltaImageRect.origin * s;
                    rect.size = model->startImageRect.size + model->deltaImageRect.size * s;
                    view->setImageRect(rect);
                }
                if (time >= module->duration)
                {
                    if (model->bFlipX)
                    {
                        view->setFlipX(model->endFlipX);
                    }
                    if (model->bFlipY)
                    {
                        view->setFlipY(model->endFlipY);
                    }
                }
                view->m_bIsAnimation = false;
                ++itr_animation;
            }
            
            if (times >= module->repeatCount && module->repeatCount < 1048576)
            {
                if (module->didStopTarget)
                {
                    if (module->didStopSel2)
                    {
                        ((CAObject *)module->didStopTarget->*module->didStopSel2)(module->animationID, module->context);
                    }
                    else if (module->didStopSel0)
                    {
                        ((CAObject *)module->didStopTarget->*module->didStopSel0)();
                    }
                }
                (*itr_module)->animations.clear();
                ++itr_module;
                continue;
            }
        }
        m_vModules.pushBack(*itr_module);
        ++itr_module;
    }
    modules.clear();
    
    if (m_vModules.empty())
    {
        CAScheduler::unschedule(schedule_selector(CAViewAnimation::update), this);
    }
}

void CAViewAnimation::setPoint(const CCPoint& point, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bPoint = true;
    model->endPoint = point;
}

void CAViewAnimation::setContentSize(const CCSize& size, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bContentSize = true;
    model->endContentSize = size;
}

void CAViewAnimation::setScaleX(float scaleX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bScaleX = true;
    model->endScaleX = scaleX;
}

void CAViewAnimation::setScaleY(float scaleY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bScaleY = true;
    model->endScaleY = scaleY;
}

void CAViewAnimation::setZOrder(int zOrder, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bZOrder = true;
    model->endZOrder = zOrder;
}

void CAViewAnimation::setVertexZ(float vertexZ, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bVertexZ = true;
    model->endVertexZ = vertexZ;
}

void CAViewAnimation::setSkewX(float skewX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bSkewX = true;
    model->endSkewX = skewX;
}

void CAViewAnimation::setSkewY(float skewY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bSkewY = true;
    model->endSkewY = skewY;
}

void CAViewAnimation::setRotationX(float rotationX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bRotationX = true;
    model->endRotationX = rotationX;
}

void CAViewAnimation::setRotationY(float rotationY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bRotationY = true;
    model->endRotationY = rotationY;
}

void CAViewAnimation::setColor(const CAColor4B& color, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bColor = true;
    model->endColor = color;
}

void CAViewAnimation::setAlpha(float alpha, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bAlpha = true;
    model->endAlpha = alpha;
}

void CAViewAnimation::setImageRect(const CCRect& imageRect, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bImageRect = true;
    model->endImageRect = imageRect;
}

void CAViewAnimation::setFlipX(bool flipX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bFlipX = true;
    model->endFlipX = flipX;
}

void CAViewAnimation::setFlipY(bool flipY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->bFlipY = true;
    model->endFlipY = flipY;
}

void CAViewAnimation::allocCAViewModel(CAView* view)
{
    CAViewAnimationModule* module = m_vWillModules.back();
    if (module->animations.getValue(view) == NULL)
    {
        module->animations.insert(view, CAViewModel::create(view));
    }

}




NS_CC_END

