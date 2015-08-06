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

    CCSize startContentSize;
    CCSize endContentSize;
    CCSize deltaContentSize;
    
    float startScaleX;
    float endScaleX;
    float deltaScaleX;
    
    float startScaleY;
    float endScaleY;
    float deltaScaleY;
    
    int startZOrder;
    int endZOrder;
    int deltaZOrder;
    
    float startVertexZ;
    float endVertexZ;
    float deltaVertexZ;
    
    float startSkewX;
    float endSkewX;
    float deltaSkewX;
    
    float startSkewY;
    float endSkewY;
    float deltaSkewY;
    
    float startRotationX;
    float endRotationX;
    float deltaRotationX;
    
    float startRotationY;
    float endRotationY;
    float deltaRotationY;
    
    CAColor4B startColor;
    CAColor4B endColor;
    short deltaColor[4];
    
    float startAlpha;
    float endAlpha;
    float deltaAlpha;
    
    CCRect startImageRect;
    CCRect endImageRect;
    CCRect deltaImageRect;
    
    bool startFlipX;
    bool endFlipX;
    
    bool startFlipY;
    bool endFlipY;
    
    CAView* view;

    CAViewModel(CAView* v)
    :view(v)
    ,deltaPoint(CCPointZero)
    ,deltaContentSize(CCSizeZero)
    ,deltaScaleX(0.0f)
    ,deltaScaleY(0.0f)
    ,deltaZOrder(0)
    ,deltaVertexZ(0.0f)
    ,deltaSkewX(0.0f)
    ,deltaSkewY(0.0f)
    ,deltaRotationX(0.0f)
    ,deltaRotationY(0.0f)
    ,deltaAlpha(0.0f)
    ,deltaImageRect(CCRectZero)
    ,startPoint(v->m_obPoint)
    ,startContentSize(v->m_obContentSize)
    ,startScaleX(v->m_fScaleX)
    ,startScaleY(v->m_fScaleY)
    ,startZOrder(v->m_nZOrder)
    ,startVertexZ(v->m_fVertexZ)
    ,startSkewX(v->m_fSkewX)
    ,startSkewY(v->m_fSkewY)
    ,startRotationX(v->m_fRotationX)
    ,startRotationY(v->m_fRotationY)
    ,startColor(v->_realColor)
    ,startAlpha(v->_realAlpha)
    ,startImageRect(v->m_obRect)
    ,startFlipX(v->m_bFlipX)
    ,startFlipY(v->m_bFlipY)
    ,endFlipX(v->m_bFlipX)
    ,endFlipY(v->m_bFlipY)
    {
        CC_SAFE_RETAIN(view);
        deltaColor[0] = 0;
        deltaColor[1] = 0;
        deltaColor[2] = 0;
        deltaColor[3] = 0;
    }
    
    virtual ~CAViewModel()
    {
        CC_SAFE_RELEASE(view);
    }
    
    static CAViewModel* create(CAView* v)
    {
        CAViewModel* model = new CAViewModel(v);
        if (model)
        {
            model->autorelease();
        }
        return model;
    }
};

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

#define VALID_FLOAT(_DELTA_) (!(fabsf(_DELTA_) < FLT_EPSILON))
#define VALID_INT(_DELTA_) (_DELTA_ != 0)
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
            if (module->willStartTarget)
            {
                if (module->willStartSel2)
                {
                    ((CAObject *)module->willStartTarget->*module->willStartSel2)(module->animationID, module->context);
                    module->willStartSel2 = NULL;
                }
                else if (module->willStartSel0)
                {
                    ((CAObject *)module->willStartTarget->*module->willStartSel0)();
                    module->willStartSel0 = NULL;
                }
                module->willStartTarget = NULL;
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
                
                if (VALID_FLOAT(model->deltaScaleX))
                {
                    view->setScaleX(model->startScaleX + model->deltaScaleX * s);
                }
                if (VALID_FLOAT(model->deltaScaleY))
                {
                    view->setScaleY(model->startScaleY + model->deltaScaleY * s);
                }
                if (VALID_FLOAT(model->deltaPoint.x) || VALID_FLOAT(model->deltaPoint.y))
                {
                    view->setPoint(model->startPoint + model->deltaPoint * s);
                }
                if (VALID_FLOAT(model->deltaContentSize.width) || VALID_FLOAT(model->deltaContentSize.height))
                {
                    view->setContentSize(model->startContentSize + model->deltaContentSize * s);
                }
                if (VALID_INT(model->deltaZOrder))
                {
                    view->setZOrder(model->startZOrder + model->deltaZOrder * s);
                }
                if (VALID_FLOAT(model->deltaVertexZ))
                {
                    view->setVertexZ(model->startVertexZ + model->deltaVertexZ * s);
                }
                if (VALID_FLOAT(model->deltaSkewX))
                {
                    view->setSkewX(model->startSkewX + model->deltaSkewX * s);
                }
                if (VALID_FLOAT(model->deltaSkewY))
                {
                    view->setSkewY(model->startSkewY + model->deltaSkewY * s);
                }
                if (VALID_FLOAT(model->deltaRotationX))
                {
                    view->setRotationX(model->startRotationX + model->deltaRotationX * s);
                }
                if (VALID_FLOAT(model->deltaRotationY))
                {
                    view->setRotationY(model->startRotationY + model->deltaRotationY * s);
                }
                if (VALID_INT(model->deltaColor[0]) ||
                    VALID_INT(model->deltaColor[1]) ||
                    VALID_INT(model->deltaColor[2]) ||
                    VALID_INT(model->deltaColor[3]))
                {
                    short r = model->startColor.r + model->deltaColor[0] * s;
                    short g = model->startColor.g + model->deltaColor[1] * s;
                    short b = model->startColor.b + model->deltaColor[2] * s;
                    short a = model->startColor.a + model->deltaColor[3] * s;
                    view->setColor(ccc4(r, g, b, a));
                }
                if (VALID_FLOAT(model->deltaAlpha))
                {
                    view->setAlpha(model->startAlpha + model->deltaAlpha * s);
                }
                if (!model->deltaImageRect.equals(CCRectZero))
                {
                    CADipRect rect;
                    rect.origin = model->startImageRect.origin + model->deltaImageRect.origin * s;
                    rect.size = model->startImageRect.size + model->deltaImageRect.size * s;
                    view->setImageRect(rect);
                }
                if (time >= module->duration)
                {
                    view->setFlipX(model->endFlipX);
                    view->setFlipY(model->endFlipY);
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
    model->endPoint = point;
    model->deltaPoint = point - model->startPoint;
}

void CAViewAnimation::setContentSize(const CCSize& size, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endContentSize = size;
    model->deltaContentSize = size - model->startContentSize;
}

void CAViewAnimation::setScaleX(float scaleX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endScaleX = scaleX;
    model->deltaScaleX = scaleX - model->startScaleX;
}

void CAViewAnimation::setScaleY(float scaleY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endScaleY = scaleY;
    model->deltaScaleY = scaleY - model->startScaleY;
}

void CAViewAnimation::setZOrder(int zOrder, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endZOrder = zOrder;
    model->deltaZOrder = zOrder - model->startZOrder;
}

void CAViewAnimation::setVertexZ(float vertexZ, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endVertexZ = vertexZ;
    model->deltaVertexZ = vertexZ - model->startVertexZ;
}

void CAViewAnimation::setSkewX(float skewX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endSkewX = skewX;
    model->deltaSkewX = skewX - model->startSkewX;
}

void CAViewAnimation::setSkewY(float skewY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endSkewY = skewY;
    model->deltaSkewY = skewY - model->startSkewY;
}

void CAViewAnimation::setRotationX(float rotationX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endRotationX = rotationX;
    model->deltaRotationX = rotationX - model->startRotationX;
}

void CAViewAnimation::setRotationY(float rotationY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endRotationY = rotationY;
    model->deltaRotationY = rotationY - model->startRotationY;
}

void CAViewAnimation::setColor(const CAColor4B& color, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endColor = color;
    model->deltaColor[0] = (short)model->endColor.r - (short)model->startColor.r;
    model->deltaColor[1] = (short)model->endColor.g - (short)model->startColor.g;
    model->deltaColor[2] = (short)model->endColor.b - (short)model->startColor.b;
    model->deltaColor[3] = (short)model->endColor.a - (short)model->startColor.a;
}

void CAViewAnimation::setAlpha(float alpha, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endAlpha = alpha;
    model->deltaAlpha = alpha - model->startAlpha;
}

void CAViewAnimation::setImageRect(const CCRect& imageRect, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endImageRect = imageRect;
    model->deltaImageRect.origin = imageRect.origin - model->startImageRect.origin;
    model->deltaImageRect.size = imageRect.size - model->startImageRect.size;
}

void CAViewAnimation::setImageRect(const CCRect& imageRect, const CCSize& untrimmedSize, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endImageRect = imageRect;
    model->deltaImageRect.origin = imageRect.origin - model->startImageRect.origin;
    model->deltaImageRect.size = imageRect.size - model->startImageRect.size;
}

void CAViewAnimation::setFlipX(bool flipX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
    model->endFlipX = flipX;
}

void CAViewAnimation::setFlipY(bool flipY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back()->animations.getValue(view);
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

