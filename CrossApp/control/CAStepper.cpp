//
//  CAStepper.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-15.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
//

#include "CAStepper.h"
#include "basics/CAScheduler.h"
#include "view/CAImageView.h"
#include "view/CAScale9ImageView.h"
#include "view/CAClippingView.h"
#include "view/CADrawView.h"
#include "view/CARenderImage.h"
#include "basics/CAApplication.h"
#include "platform/CADensityDpi.h"

NS_CC_BEGIN

CAStepper::CAStepper()
: m_bAutoRepeat(true)
, m_bContinuous(true)
, m_bWraps(false)
, m_value(0)
, m_minimumValue(0)
, m_maximumValue(100)
, m_stepValue(0)
, m_pDecrementImageView(NULL)
, m_pIncrementImageView(NULL)
, m_pBackgroundImageView(NULL)
, m_pBackgroundSelectedImageView(NULL)
, m_actionType(ActionNone)
, m_bTouchEffect(false)
, m_pDividerImageView(NULL)
, m_pCAStepperOrientation(CAStepperOrientationHorizontal)
,m_cTintColor(ccc4(54, 195, 240, 255))
{
    memset(m_pBackgroundImage, 0x00, sizeof(m_pBackgroundImage));
    memset(m_pIncrementImage, 0x00, sizeof(m_pIncrementImage));
    memset(m_pDecrementImage, 0x00, sizeof(m_pDecrementImage));
}

CAStepper::~CAStepper()
{
    for (int i=0; i<CAControlStateAll; i++) {
        CC_SAFE_RELEASE(m_pBackgroundImage[i]);
    }
    for (int i=0; i<CAControlStateAll; i++) {
        CC_SAFE_RELEASE(m_pIncrementImage[i]);
    }
    for (int i=0; i<CAControlStateAll; i++) {
        CC_SAFE_RELEASE(m_pDecrementImage[i]);
    }
    
    CC_SAFE_RELEASE(m_pBackgroundImageView);
    CC_SAFE_RELEASE(m_pBackgroundSelectedImageView);
    CC_SAFE_RELEASE(m_pIncrementImageView);
    CC_SAFE_RELEASE(m_pDecrementImageView);
    CC_SAFE_RELEASE(m_pDividerImageView);
}

CAStepper* CAStepper::create()
{
    CAStepper* page = new CAStepper();
    
    if (page && page->init())
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

CAStepper* CAStepper::createWithFrame(const DRect& rect)
{
    CAStepper* page = new CAStepper();
    
    if (page && page->initWithFrame(rect))
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

CAStepper* CAStepper::createWithCenter(const DRect& rect)
{
    CAStepper* page = new CAStepper();
    
    if (page && page->initWithCenter(rect))
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

bool CAStepper::init()
{
    if (!CAControl::init())
    {
        return false;
    }
    setBackgroundImage(CAImage::create("source_material/btn_rounded_normal.png"), CAControlStateNormal);
    setBackgroundImage(CAImage::create("source_material/btn_rounded_highlighted.png"), CAControlStateHighlighted);
    setIncrementImage(CAImage::create("source_material/stepper_inc_h.png"), CAControlStateAll);
    setIncrementImage(CAImage::create("source_material/stepper_inc_n.png"), CAControlStateNormal);
    setDecrementImage(CAImage::create("source_material/stepper_dec_h.png"), CAControlStateAll);
    setDecrementImage(CAImage::create("source_material/stepper_dec_n.png"), CAControlStateNormal);
    return true;
}

bool CAStepper::initWithFrame(const DRect& rect)
{
    if (!CAControl::initWithFrame(rect))
    {
        return false;
    }
    
    return true;
}

bool CAStepper::initWithCenter(const DRect& rect)
{
    if (!CAControl::initWithCenter(rect))
    {
        return false;
    }
    return true;
}

void CAStepper::setBackgroundImage(CrossApp::CAImage *image, CAControlState state)
{
    if (state == CAControlStateAll) {
        for (int i=0; i<CAControlStateAll; i++) {
            CC_SAFE_RELEASE_NULL(m_pBackgroundImage[i]);
            m_pBackgroundImage[i] = image;
            CC_SAFE_RETAIN(m_pBackgroundImage[i]);        
        }
    } else {
        CC_SAFE_RELEASE_NULL(m_pBackgroundImage[state]);
        m_pBackgroundImage[state] = image;
        CC_SAFE_RETAIN(m_pBackgroundImage[state]);        
    }
}

CAImage* CAStepper::getBackgroundImageForState(CAControlState state)
{
    return m_pBackgroundImage[state];
}

void CAStepper::setIncrementImage(CrossApp::CAImage *image, CAControlState state)
{
    if (state == CAControlStateAll) {
        for (int i=0; i<CAControlStateAll; i++) {
            CC_SAFE_RELEASE_NULL(m_pIncrementImage[i]);
            m_pIncrementImage[i] = image;
            CC_SAFE_RETAIN(m_pIncrementImage[i]);        
        }
    } else {
        CC_SAFE_RELEASE_NULL(m_pIncrementImage[state]);
        m_pIncrementImage[state] = image;
        CC_SAFE_RETAIN(m_pIncrementImage[state]);
    }
    if (m_pIncrementImageView!=NULL&& m_pIncrementImage[CAControlStateNormal]) {
        m_pIncrementImageView->setImage(m_pIncrementImage[CAControlStateNormal]);
    }
}

CAImage* CAStepper::getIncrementImageForState(CAControlState state)
{
    return m_pIncrementImage[state];
}

void CAStepper::setDecrementImage(CrossApp::CAImage *image, CAControlState state)
{
    if (state == CAControlStateAll) {
        for (int i=0; i<CAControlStateAll; i++) {
            CC_SAFE_RELEASE_NULL(m_pDecrementImage[i]);
            m_pDecrementImage[i] = image;
            CC_SAFE_RETAIN(m_pDecrementImage[i]);
        }
    } else {
        CC_SAFE_RELEASE_NULL(m_pDecrementImage[state]);
        m_pDecrementImage[state] = image;
        CC_SAFE_RETAIN(m_pDecrementImage[state]);
    }
    if (m_pDecrementImageView!=NULL&& m_pDecrementImage[CAControlStateNormal]) {
        m_pDecrementImageView->setImage(m_pDecrementImage[CAControlStateNormal]);
    }
}

CAImage* CAStepper::getDecrementImageForState(CAControlState state)
{
    return m_pDecrementImage[state];
}

void CAStepper::setDividerColor(CAColor4B color)
{

    m_cTintColor = color;
    if (m_pDividerImageView!=NULL) {
        m_pDividerImageView->setColor(m_cTintColor);
    }
}

CAColor4B CAStepper::getDividerColor()
{
    return m_cTintColor;
}

void CAStepper::setTailorImageAtIndex(int index)
{
    if (m_pBackgroundImageView && m_pBackgroundImage[CAControlStateHighlighted]) {
        if (m_pBackgroundSelectedImageView) {
            this->removeSubview(m_pBackgroundSelectedImageView);
            m_pBackgroundSelectedImageView = NULL;
        }
        m_pBackgroundSelectedImageView = (CAImageView*)getTailorImageAtIndex(index);
        this->insertSubview(m_pBackgroundSelectedImageView,1);
    }
}

CAView* CAStepper::getTailorImageAtIndex(int i)
{
    CAImageView* imageView = NULL;
    
    CAScale9ImageView* clipNode = CAScale9ImageView::createWithFrame(m_pBackgroundImageView->getBounds());
    clipNode->setImage(m_pBackgroundImage[CAControlStateHighlighted]);
    
    DSize size = clipNode->getFrame().size;
    CARenderImage* render = CARenderImage::create(size.width, size.height);
    render->printscreenWithView(clipNode);
    
    DRect rect;
    if (m_pCAStepperOrientation==CAStepperOrientationHorizontal) {
        rect.origin = DPoint(i*m_pBackgroundImageView->getBounds().size.width/2,0);
        rect.size   = DSize(m_pBackgroundImageView->getBounds().size.width/2,m_pBackgroundImageView->getBounds().size.height);
    }else{
        rect.origin = DPoint(0,i*m_pBackgroundImageView->getBounds().size.height/2);
        rect.size   = DSize(m_pBackgroundImageView->getBounds().size.width,m_pBackgroundImageView->getBounds().size.height/2);
    }
    
    imageView = CAImageView::createWithImage(render->getImageView()->getImage());
    imageView->setFrame(rect);
    imageView->setImageRect(rect);
    return imageView;
}

bool CAStepper::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    m_actionType = ActionNone; // lazy init;
    m_pIncrementImageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    
    click(pTouch);
    action();
    switch (m_actionType)
    {
        case ActionDecrease:
            m_pDecrementImageView->setImage(m_pDecrementImage[CAControlStateHighlighted]);
            m_pIncrementImageView->setImage(m_pIncrementImage[CAControlStateNormal]);
            if (m_pCAStepperOrientation==CAStepperOrientationHorizontal) {
                setTailorImageAtIndex(0);
            }else{
                setTailorImageAtIndex(1);
            }
            if (m_bTouchEffect) {
                m_pDecrementImageView->setVisible(false);
                m_pIncrementImageView->setVisible(true);
            }
            break;
            
        case ActionIncrease:
            m_pDecrementImageView->setImage(m_pDecrementImage[CAControlStateNormal]);
            m_pIncrementImageView->setImage(m_pIncrementImage[CAControlStateHighlighted]);
            if (m_pCAStepperOrientation==CAStepperOrientationHorizontal) {
                setTailorImageAtIndex(1);
            }else{
                setTailorImageAtIndex(0);
            }
            if (m_bTouchEffect) {
                m_pDecrementImageView->setVisible(true);
                m_pIncrementImageView->setVisible(false);
            }
            break;
            
        case ActionNone:
            m_pDecrementImageView->setImage(m_pDecrementImage[CAControlStateNormal]);
            m_pIncrementImageView->setImage(m_pIncrementImage[CAControlStateNormal]);
            if (m_bTouchEffect) {
                m_pDecrementImageView->setVisible(true);
                m_pIncrementImageView->setVisible(true);
            }
            break;
        default:
            break;
    }
    
    if (m_bAutoRepeat)
    {
        CAScheduler::schedule(schedule_selector(CAStepper::repeat), this, 0.1f, kCCRepeatForever, 0.5f);
    }
    
    return true;
}

void CAStepper::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{

}

void CAStepper::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    m_pDecrementImageView->setImage(m_pDecrementImage[CAControlStateNormal]);
    m_pIncrementImageView->setImage(m_pIncrementImage[CAControlStateNormal]);
    
    if (m_bTouchEffect)
    {
        m_pDecrementImageView->setVisible(true);
        m_pIncrementImageView->setVisible(true);
    }
    
    if (m_pBackgroundSelectedImageView)
    {
        this->removeSubview(m_pBackgroundSelectedImageView);
        m_pBackgroundSelectedImageView = NULL;
    }
    CAScheduler::unschedule(schedule_selector(CAStepper::repeat), this);
}

void CAStepper::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    m_pDecrementImageView->setImage(m_pDecrementImage[CAControlStateNormal]);
    m_pIncrementImageView->setImage(m_pIncrementImage[CAControlStateNormal]);
    
    if (m_bTouchEffect)
    {
        m_pDecrementImageView->setVisible(true);
        m_pIncrementImageView->setVisible(true);
    }
    
    if (m_pBackgroundSelectedImageView)
    {
        this->removeSubview(m_pBackgroundSelectedImageView);
        m_pBackgroundSelectedImageView = NULL;
    }
    CAScheduler::unschedule(schedule_selector(CAStepper::repeat), this);
}

void CAStepper::onEnter()
{
    CAControl::onEnter();

    if (m_value<m_minimumValue) {
        m_value = m_minimumValue;
    }
    // init background
    if (!m_pBackgroundImageView && m_pBackgroundImage[CAControlStateNormal]) {
        m_pBackgroundImageView = CAScale9ImageView::createWithImage(m_pBackgroundImage[CAControlStateNormal]);
        m_pBackgroundImageView->retain();
        m_pBackgroundImageView->setCenter(DRect(getFrame().size.width/2, getFrame().size.height/2,getFrame().size.width,getFrame().size.height));
        addSubview(m_pBackgroundImageView);
    }
    // init divider
    if (!m_pDividerImageView ) {
        m_pDividerImageView = CAImageView::createWithColor(m_cTintColor);
        m_pDividerImageView->retain();
        if (m_pCAStepperOrientation==CAStepperOrientationHorizontal)
        {
            m_pDividerImageView->setCenter(DRect(getBounds().size.width/2,
                                                  getBounds().size.height/2,
                                                  s_px_to_dip(2),
                                                  getBounds().size.height));
        }else{
            m_pDividerImageView->setCenter(DRect(getBounds().size.width/2,
                                                  getBounds().size.height/2,
                                                  getBounds().size.width,
                                                  s_px_to_dip(2)));
            m_pDividerImageView->setRotation(90);
        }
        
        
        
        addSubview(m_pDividerImageView);
    }
    
    float div = (m_pDividerImageView) ? m_pDividerImageView->getBounds().size.width/2 : 0;
    
    // init increment
    if (!m_pIncrementImageView && m_pIncrementImage[CAControlStateNormal]) {
        m_pIncrementImageView = CAImageView::createWithImage(m_pIncrementImage[CAControlStateNormal]);
        m_pIncrementImageView->retain();
        if (m_pCAStepperOrientation==CAStepperOrientationHorizontal) {
            m_pIncrementImageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
            m_pIncrementImageView->setCenter(DRect(getBounds().size.width/4*3 + div, getBounds().size.height/2,
                                                   getBounds().size.width/2 - div,
                                                   getBounds().size.height));
        }else{
            m_pIncrementImageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByVertical);
            m_pIncrementImageView->setCenter(DRect(getBounds().size.width/2, getBounds().size.height/4,
                                                   getBounds().size.width,
                                                   getBounds().size.height/2 - div));
        }
        
        insertSubview(m_pIncrementImageView,10);
    }
    
    // init decrement
    if (!m_pDecrementImageView && m_pDecrementImage[CAControlStateNormal]) {
        m_pDecrementImageView = CAImageView::createWithImage(m_pDecrementImage[CAControlStateNormal]);
        m_pDecrementImageView->retain();
        if (m_pCAStepperOrientation==CAStepperOrientationHorizontal) {
            m_pDecrementImageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
            m_pDecrementImageView->setCenter(DRect(getBounds().size.width/4, getBounds().size.height/2,
                                                   getBounds().size.width/2 - div,
                                                   getBounds().size.height));
        }else{
            m_pDecrementImageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByVertical);
            m_pDecrementImageView->setCenter(DRect(getBounds().size.width/2, getBounds().size.height/4*3 + div,
                                                   getBounds().size.width,
                                                   getBounds().size.height/2 - div));
        }
        
        insertSubview(m_pDecrementImageView,10);
    }
}

void CAStepper::onExit()
{
    CAControl::onExit();
}

void CAStepper::visit()
{
    CAControl::visit();
}

void CAStepper::action()
{
    bool bValueChange = true;
    
    switch (m_actionType) {
        case ActionIncrease: // increase
            if (m_value == m_maximumValue) {
                if (m_bWraps) {
                    m_value = m_minimumValue;
                } else {
                    bValueChange = false;
                }
                break;
            }
            m_value += m_stepValue;
            if (m_value > m_maximumValue) {
                m_value = m_maximumValue;
            }
            break;
            
        case ActionDecrease: // decrease
            if (m_value == m_minimumValue) {
                if (m_bWraps) {
                    m_value = m_maximumValue;
                } else {
                    bValueChange = false;
                }
                break;
            }
            m_value -= m_stepValue;
            if (m_value < m_minimumValue) {
                m_value = m_minimumValue;
            }
            break;
            
        case 0:
        default:
            bValueChange = false;
            break;
    }
    
    // send value change event
    if (bValueChange && m_bContinuous) {
        if ( m_pTarget[CAControlEventTouchValueChanged] && 
             m_selTouch[CAControlEventTouchValueChanged] ) 
        {
            (m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, DPointZero);
        }
    }
}

void CAStepper::repeat(float dt)
{
    action();
}

void CAStepper::click(CATouch* pTouch)
{
    DRect increseRect, decreaseRect;
    if (m_pCAStepperOrientation==CAStepperOrientationHorizontal) {
        increseRect = getBounds();
        increseRect.origin.x = getBounds().size.width/2;
        increseRect.size.width = getBounds().size.width/2;
        decreaseRect = increseRect;
        decreaseRect.origin.x = 0;
    }else{
        decreaseRect = getBounds();
        decreaseRect.origin.y = getBounds().size.height/2;
        decreaseRect.size.height = getBounds().size.height/2;
        increseRect = decreaseRect;
        increseRect.origin.y = 0;
    }

    
    if (increseRect.containsPoint(convertToNodeSpace(pTouch->getLocation()))) {
        m_actionType = ActionIncrease;
    } else if (decreaseRect.containsPoint(convertToNodeSpace(pTouch->getLocation()))) {
        m_actionType = ActionDecrease;
    } else {
        m_actionType = ActionNone;
    }
}

void CAStepper::addTarget(CAObject* target, SEL_CAControl selector)
{
	CAControl::addTarget(target, selector, CAControlEventTouchValueChanged);
}

void CAStepper::removeTarget(CAObject* target, SEL_CAControl selector)
{
	CAControl::removeTarget(target, selector, CAControlEventTouchValueChanged);
}

void CAStepper::removeAllTargets()
{
	CAControl::removeAllTargets();
}

void CAStepper::setContentSize(const DSize & var)
{
    CAControl::setContentSize(var);
    if (m_pBackgroundImageView) {
        m_pBackgroundImageView->setFrame(this->getBounds());
    }
    
    if (m_pDividerImageView) {

        if (m_pCAStepperOrientation==CAStepperOrientationHorizontal)
        {
            m_pDividerImageView->setCenter(DRect(getBounds().size.width/2,
                                                  getBounds().size.height/2,
                                                  s_px_to_dip(2.0f),
                                                  getBounds().size.height));
        }else{
            m_pDividerImageView->setCenter(DRect(getBounds().size.width/2,
                                                  getBounds().size.height/2,
                                                  getBounds().size.width,
                                                  s_px_to_dip(2.0f)));
        }
    }
    
    float div = (m_pDividerImageView) ? m_pDividerImageView->getBounds().size.width/2 : 0;
    
    // init increment
    if (m_pIncrementImageView) {
        if (m_pCAStepperOrientation==CAStepperOrientationHorizontal) {
            m_pIncrementImageView->setCenter(DRect(getBounds().size.width/4*3 + div, getBounds().size.height/2,
                                                    getBounds().size.width/2 - div,
                                                    getBounds().size.height));
        }else{
            m_pIncrementImageView->setCenter(DRect(getBounds().size.width/2, getBounds().size.height/4,
                                                    getBounds().size.width,
                                                    getBounds().size.height/2 - div));
        }
    }
    
    // init decrement
    if (m_pDecrementImageView) {
        if (m_pCAStepperOrientation==CAStepperOrientationHorizontal) {
            m_pDecrementImageView->setCenter(DRect(getBounds().size.width/4, getBounds().size.height/2,
                                                    getBounds().size.width/2 - div,
                                                    getBounds().size.height));
        }else{
            m_pDecrementImageView->setCenter(DRect(getBounds().size.width/2, getBounds().size.height/4*3 + div,
                                                    getBounds().size.width,
                                                    getBounds().size.height/2 - div));
        }
    }
}



NS_CC_END