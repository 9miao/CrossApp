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

class CAViewModel : public CAObject
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
    short deltaColorR;
    short deltaColorG;
    short deltaColorB;
    short deltaColorA;
    
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
    ,deltaColorR(0)
    ,deltaColorG(0)
    ,deltaColorB(0)
    ,deltaColorA(0)
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
    CAViewAnimationModule module;
    module.animationID = animationID;
    module.context = context;
    module.duration = 0.2f;
    module.delay = 0;
    module.time = 0;
    module.curve = CAViewAnimationCurveLinear;
    CAViewAnimation::getInstance()->m_vWillModules.push_back(module);
}

void CAViewAnimation::commitAnimations()
{
    CAViewAnimation* manager = CAViewAnimation::getInstance();
    manager->m_bBeginAnimations = false;
    
    if (!manager->m_vWillModules.empty())
    {
        manager->m_vModules.push_back(manager->m_vWillModules.front());
        manager->m_vWillModules.pop_front();
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
    CC_RETURN_IF(CAViewAnimation::getInstance()->m_vWillModules.empty());
    CAViewAnimation::getInstance()->m_vWillModules.back().duration = duration;
}

void CAViewAnimation::setAnimationDelay(float delay)
{
    CC_RETURN_IF(CAViewAnimation::getInstance()->m_vWillModules.empty());
    CAViewAnimation::getInstance()->m_vWillModules.back().delay = delay;
}

void CAViewAnimation::setAnimationCurve(const CAViewAnimationCurve& curve)
{
    CC_RETURN_IF(CAViewAnimation::getInstance()->m_vWillModules.empty());
    CAViewAnimation::getInstance()->m_vWillModules.back().curve = curve;
}

void CAViewAnimation::setAnimationWillStartSelector(CAObject* target, SEL_CAViewAnimation selector)
{
    CC_RETURN_IF(CAViewAnimation::getInstance()->m_vWillModules.empty());

}

void CAViewAnimation::setAnimationDidStopSelector(CAObject* target, SEL_CAViewAnimation selector)
{
    CC_RETURN_IF(CAViewAnimation::getInstance()->m_vWillModules.empty());

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



void CAViewAnimation::update(float dt)
{
    std::vector<CAViewAnimation::CAViewAnimationModule>::iterator itr_module = m_vModules.begin();
    while (itr_module != m_vModules.end())
    {
        CAViewAnimationModule& module = *itr_module;
        module.time += dt;
        float time = module.time - module.delay;
        if (!(time < FLT_MIN))
        {
            float s = time / module.duration;
            s = MIN(s, 1.0f);
            
            CAMap<CAView*, CAObject*>& animations = itr_module->animations;
            CAMap<CAView*, CAObject*>::iterator itr_animation = animations.begin();
            while (itr_animation != animations.end())
            {
                CAView* view = itr_animation->first;
                CAViewModel* model = (CAViewModel*)(itr_animation->second);
                view->setPoint(model->startPoint + model->deltaPoint * s);
                view->setContentSize(model->startContentSize + model->deltaContentSize * s);
                view->setScaleX(model->startScaleX + model->deltaScaleX * s);
                view->setScaleY(model->startScaleY + model->deltaScaleY * s);
                view->setZOrder(model->startZOrder + model->deltaZOrder * s);
                view->setVertexZ(model->startVertexZ + model->deltaVertexZ * s);
                view->setSkewX(model->startSkewX + model->deltaSkewX * s);
                view->setSkewY(model->startSkewY + model->deltaSkewY * s);
                view->setRotationX(model->startRotationX + model->deltaRotationX * s);
                view->setRotationY(model->startRotationY + model->deltaRotationY * s);
                short colorR = model->startColor.r + model->deltaColorR * s;
                short colorG = model->startColor.g + model->deltaColorG * s;
                short colorB = model->startColor.b + model->deltaColorB * s;
                short colorA = model->startColor.a + model->deltaColorA * s;
                view->setColor(ccc4(colorR, colorG, colorB, colorA));
                view->setAlpha(model->startAlpha + model->deltaAlpha * s);
                CADipRect rect;
                rect.origin = model->startImageRect.origin + model->deltaImageRect.origin * s;
                rect.size = model->startImageRect.size + model->deltaImageRect.size * s;
                view->setImageRect(rect);

                if (time >= module.duration)
                {
                    view->setFlipX(model->endFlipX);
                    view->setFlipY(model->endFlipY);
                }
                
                ++itr_animation;
            }
            
            if (time >= module.duration)
            {
                itr_module->animations.clear();
                itr_module = m_vModules.erase(itr_module);
                continue;
            }

        }
        ++itr_module;
    }
    if (m_vModules.empty())
    {
        CAScheduler::unschedule(schedule_selector(CAViewAnimation::update), this);
    }
}

void CAViewAnimation::setPoint(const CCPoint& point, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endPoint = point;
    model->deltaPoint = point - model->startPoint;
}

void CAViewAnimation::setContentSize(const CCSize& size, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endContentSize = size;
    model->deltaContentSize = size - model->startContentSize;
}

void CAViewAnimation::setScaleX(float scaleX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endScaleX = scaleX;
    model->deltaScaleX = scaleX - model->startScaleX;
}

void CAViewAnimation::setScaleY(float scaleY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endScaleY = scaleY;
    model->deltaScaleY = scaleY - model->startScaleY;
}

void CAViewAnimation::setZOrder(int zOrder, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endZOrder = zOrder;
    model->deltaZOrder = zOrder - model->startZOrder;
}

void CAViewAnimation::setVertexZ(float vertexZ, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endVertexZ = vertexZ;
    model->deltaVertexZ = vertexZ - model->startVertexZ;
}

void CAViewAnimation::setSkewX(float skewX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endSkewX = skewX;
    model->deltaSkewX = skewX - model->startSkewX;
}

void CAViewAnimation::setSkewY(float skewY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endSkewY = skewY;
    model->deltaSkewY = skewY - model->startSkewY;
}

void CAViewAnimation::setRotationX(float rotationX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endRotationX = rotationX;
    model->deltaRotationX = rotationX - model->startRotationX;
}

void CAViewAnimation::setRotationY(float rotationY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endRotationY = rotationY;
    model->deltaRotationY = rotationY - model->startRotationY;
}

void CAViewAnimation::setColor(const CAColor4B& color, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endColor = color;
    model->deltaColorR = (short)color.r - (short)view->_realColor.r;
    model->deltaColorG = (short)color.g - (short)view->_realColor.g;
    model->deltaColorB = (short)color.b - (short)view->_realColor.b;
    model->deltaColorA = (short)color.a - (short)view->_realColor.a;
}

void CAViewAnimation::setAlpha(float alpha, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endAlpha = alpha;
    model->deltaAlpha = alpha - model->startAlpha;
}

void CAViewAnimation::setImageRect(const CCRect& imageRect, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endImageRect = imageRect;
    model->deltaImageRect.origin = imageRect.origin - model->startImageRect.origin;
    model->deltaImageRect.size = imageRect.size - model->startImageRect.size;
}

void CAViewAnimation::setFlipX(bool flipX, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endFlipX = flipX;
}

void CAViewAnimation::setFlipY(bool flipY, CAView* view)
{
    CAViewAnimation::allocCAViewModel(view);
    CAViewModel* model = (CAViewModel*)m_vWillModules.back().animations.getValue(view);
    model->endFlipY = flipY;
}

void CAViewAnimation::allocCAViewModel(CAView* view)
{
    CAViewAnimationModule& module = m_vWillModules.back();
    if (module.animations.getValue(view) == NULL)
    {
        module.animations.insert(view, CAViewModel::create(view));
    }

}




NS_CC_END

