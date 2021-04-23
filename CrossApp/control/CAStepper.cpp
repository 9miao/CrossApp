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
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

CAStepper::CAStepper(CAStepper::Orientation type)
: m_bAutoRepeat(true)
, m_bWraps(false)
, m_value(0)
, m_minimumValue(0)
, m_maximumValue(100)
, m_stepValue(1.0f)
, m_pDecrementImageView(nullptr)
, m_pIncrementImageView(nullptr)
, m_pBackgroundImageView(nullptr)
, m_pBackgroundHighlightedmageView(nullptr)
, m_actionType(ActionNone)
, m_bTouchEffect(false)
, m_pDividerImageView(nullptr)
, m_pOrientation(type)
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAStepper");
    this->setBackgroundImage(CAStepper::State::Normal, CAImage::create(map.at("backgroundView_normal")));
    this->setBackgroundImage(CAStepper::State::Highlighted, CAImage::create(map.at("backgroundView_selected")));
    this->setIncrementImage(CAStepper::State::Normal, CAImage::create(map.at("incImage_normal")));
    this->setIncrementImage(CAStepper::State::Highlighted, CAImage::create(map.at("incImage_highlighted")));
    this->setDecrementImage(CAStepper::State::Normal, CAImage::create(map.at("decImage_normal")));
    this->setDecrementImage(CAStepper::State::Highlighted, CAImage::create(map.at("decImage_highlighted")));
    
    m_cTintColor = ccc4Int(CrossApp::hex2Int(map.at("tintColor")));
}

CAStepper::~CAStepper()
{
    CC_SAFE_RELEASE(m_pBackgroundImageView);
    CC_SAFE_RELEASE(m_pBackgroundHighlightedmageView);
    CC_SAFE_RELEASE(m_pIncrementImageView);
    CC_SAFE_RELEASE(m_pDecrementImageView);
    CC_SAFE_RELEASE(m_pDividerImageView);
}

CAStepper* CAStepper::create(CAStepper::Orientation type)
{
    CAStepper* page = new CAStepper(type);
    
    if (page && page->init())
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

CAStepper* CAStepper::createWithFrame(const DRect& rect, CAStepper::Orientation type)
{
    CAStepper* page = new CAStepper(type);
    
    if (page && page->initWithFrame(rect))
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

CAStepper* CAStepper::createWithCenter(const DRect& rect, CAStepper::Orientation type)
{
    CAStepper* page = new CAStepper(type);
    
    if (page && page->initWithCenter(rect))
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

CAStepper* CAStepper::createWithLayout(const CrossApp::DLayout &layout, CAStepper::Orientation type)
{
    CAStepper* page = new CAStepper(type);
    
    if (page && page->initWithLayout(layout))
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
    
    return true;
}

void CAStepper::setBackgroundImage(CAStepper::State state, CrossApp::CAImage *image)
{
    m_mBackgroundImages.erase(state);
    if (image)
    {
        m_mBackgroundImages.insert(state, image);
    }
}

void CAStepper::setIncrementImage(CAStepper::State state, CrossApp::CAImage *image)
{
    m_mIncrementImages.erase(state);
    if (image)
    {
        m_mIncrementImages.insert(state, image);
    }
}

void CAStepper::setDecrementImage(CAStepper::State state, CrossApp::CAImage *image)
{
    m_mDecrementImages.erase(state);
    if (image)
    {
        m_mDecrementImages.insert(state, image);
    }
}

void CAStepper::setDividerColor(CAColor4B color)
{
    m_cTintColor = color;
    if (m_pDividerImageView!=nullptr)
    {
        m_pDividerImageView->setColor(m_cTintColor);
    }
}

CAColor4B CAStepper::getDividerColor()
{
    return m_cTintColor;
}

void CAStepper::setTailorImageAtIndex(int index)
{
    if (m_pBackgroundImageView && m_mBackgroundImages.contains(CAStepper::State::Highlighted))
    {
        if (m_pBackgroundHighlightedmageView)
        {
            this->removeSubview(m_pBackgroundHighlightedmageView);
            m_pBackgroundHighlightedmageView = nullptr;
        }
        m_pBackgroundHighlightedmageView = (CAImageView*)getTailorImageAtIndex(index);
        this->insertSubview(m_pBackgroundHighlightedmageView,1);
    }
}

CAView* CAStepper::getTailorImageAtIndex(int i)
{
    CAImageView* imageView = nullptr;
    
    CAScale9ImageView* clipNode = CAScale9ImageView::createWithFrame(m_pBackgroundImageView->getBounds());
    clipNode->setImage(m_mBackgroundImages.at(CAStepper::State::Highlighted));
    clipNode->setScale(s_px_to_dip(1));
    
    DSize size = clipNode->getFrame().size;
    CARenderImage* render = CARenderImage::create(s_px_to_dip(size.width), s_px_to_dip(size.height));
    render->printscreenWithView(clipNode);
    
    DRect rect;
    if (m_pOrientation==CAStepper::Orientation::Horizontal)
    {
        rect.origin = DPoint(i*m_pBackgroundImageView->m_obContentSize.width/2,0);
        rect.size   = DSize(m_pBackgroundImageView->m_obContentSize.width/2,m_pBackgroundImageView->m_obContentSize.height);
    }
    else
    {
        rect.origin = DPoint(0,i*m_pBackgroundImageView->m_obContentSize.height/2);
        rect.size   = DSize(m_pBackgroundImageView->m_obContentSize.width,m_pBackgroundImageView->m_obContentSize.height/2);
    }
    
    imageView = CAImageView::createWithImage(render->getImageView()->getImage());
    imageView->setFrame(rect);
    imageView->setImageRect(rect);
    return imageView;
}

bool CAStepper::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    m_actionType = ActionNone; // lazy init;
    m_pIncrementImageView->setScaleType(CAImageView::ScaleType::FitViewByHorizontal);
    
    click(pTouch);
    action();
    switch (m_actionType)
    {
        case ActionDecrease:
            m_pDecrementImageView->setImage(m_mDecrementImages.at(CAStepper::State::Highlighted));
            m_pIncrementImageView->setImage(m_mIncrementImages.at(CAStepper::State::Normal));
            if (m_pOrientation==CAStepper::Orientation::Horizontal)
            {
                setTailorImageAtIndex(0);
            }
            else
            {
                setTailorImageAtIndex(1);
            }
            if (m_bTouchEffect) {
                m_pDecrementImageView->setVisible(false);
                m_pIncrementImageView->setVisible(true);
            }
            break;
            
        case ActionIncrease:
            m_pDecrementImageView->setImage(m_mDecrementImages.at(CAStepper::State::Normal));
            m_pIncrementImageView->setImage(m_mIncrementImages.at(CAStepper::State::Highlighted));
            if (m_pOrientation==CAStepper::Orientation::Horizontal) {
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
            m_pDecrementImageView->setImage(m_mDecrementImages.at(CAStepper::State::Normal));
            m_pIncrementImageView->setImage(m_mIncrementImages.at(CAStepper::State::Normal));
            if (m_bTouchEffect)
            {
                m_pDecrementImageView->setVisible(true);
                m_pIncrementImageView->setVisible(true);
            }
            break;
        default:
            break;
    }
    
    if (m_bAutoRepeat)
    {
        CAScheduler::getScheduler()->schedule(schedule_selector(CAStepper::repeat), this, 0.1f, kCCRepeatForever, 0.5f);
    }
    
    return true;
}

void CAStepper::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{

}

void CAStepper::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    m_pDecrementImageView->setImage(m_mDecrementImages.at(CAStepper::State::Normal));
    m_pIncrementImageView->setImage(m_mIncrementImages.at(CAStepper::State::Normal));
    
    if (m_bTouchEffect)
    {
        m_pDecrementImageView->setVisible(true);
        m_pIncrementImageView->setVisible(true);
    }
    
    if (m_pBackgroundHighlightedmageView)
    {
        this->removeSubview(m_pBackgroundHighlightedmageView);
        m_pBackgroundHighlightedmageView = NULL;
    }
    CAScheduler::getScheduler()->unschedule(schedule_selector(CAStepper::repeat), this);
}

void CAStepper::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    m_pDecrementImageView->setImage(m_mDecrementImages.at(CAStepper::State::Normal));
    m_pIncrementImageView->setImage(m_mIncrementImages.at(CAStepper::State::Normal));
    
    if (m_bTouchEffect)
    {
        m_pDecrementImageView->setVisible(true);
        m_pIncrementImageView->setVisible(true);
    }
    
    if (m_pBackgroundHighlightedmageView)
    {
        this->removeSubview(m_pBackgroundHighlightedmageView);
        m_pBackgroundHighlightedmageView = NULL;
    }
    CAScheduler::getScheduler()->unschedule(schedule_selector(CAStepper::repeat), this);
}

void CAStepper::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();

    if (m_value<m_minimumValue)
    {
        m_value = m_minimumValue;
    }
    
    // init Background
    if (!m_pBackgroundImageView)
    {
        m_pBackgroundImageView = CAScale9ImageView::createWithImage(m_mBackgroundImages.at(CAStepper::State::Normal));
        m_pBackgroundImageView->retain();
        m_pBackgroundImageView->setCenter(DRect(getFrame().size.width/2, getFrame().size.height/2,getFrame().size.width,getFrame().size.height));
        addSubview(m_pBackgroundImageView);
    }
    
    // init divider
    if (!m_pDividerImageView )
    {
        m_pDividerImageView = CAImageView::createWithColor(m_cTintColor);
        m_pDividerImageView->retain();
        if (m_pOrientation==CAStepper::Orientation::Horizontal)
        {
            m_pDividerImageView->setCenter(DRect(m_obContentSize.width/2,
                                                  m_obContentSize.height/2,
                                                  LINE_WIDTH,
                                                  m_obContentSize.height));
        }
        else
        {
            m_pDividerImageView->setCenter(DRect(m_obContentSize.width/2,
                                                  m_obContentSize.height/2,
                                                  m_obContentSize.width,
                                                  LINE_WIDTH));
        }
        
        
        
        addSubview(m_pDividerImageView);
    }
    
    //float div = (m_pDividerImageView) ? m_pDividerImageView->m_obContentSize.width/2 : 0;
    
    // init increment
    if (!m_pIncrementImageView)
    {
        m_pIncrementImageView = CAImageView::createWithImage(m_mIncrementImages.at(CAStepper::State::Normal));
        m_pIncrementImageView->retain();
        if (m_pOrientation==CAStepper::Orientation::Horizontal) {
            int tempw = MIN(m_obContentSize.width, m_obContentSize.height);
            m_pIncrementImageView->setScaleType(CAImageView::ScaleType::FitViewByHorizontal);
            m_pIncrementImageView->setCenter(DRect(m_obContentSize.width/4*3, m_obContentSize.height/2,
                                                   tempw,
                                                   tempw));
        }else{
            int tempw = MIN(m_obContentSize.width, m_obContentSize.height);
            m_pIncrementImageView->setScaleType(CAImageView::ScaleType::FitViewByVertical);
            m_pIncrementImageView->setCenter(DRect(m_obContentSize.width/2, m_obContentSize.height/4,
                                                   tempw,
                                                   tempw));
        }
        
        insertSubview(m_pIncrementImageView,10);
    }
    
    // init decrement
    if (!m_pDecrementImageView)
    {
        m_pDecrementImageView = CAImageView::createWithImage(m_mDecrementImages.at(CAStepper::State::Normal));
        m_pDecrementImageView->retain();
        if (m_pOrientation==CAStepper::Orientation::Horizontal) {
            int tempw = MIN(m_obContentSize.width, m_obContentSize.height);
            m_pDecrementImageView->setScaleType(CAImageView::ScaleType::FitViewByHorizontal);
            m_pDecrementImageView->setCenter(DRect(m_obContentSize.width/4, m_obContentSize.height/2,
                                                   tempw,
                                                   tempw));
        }else{
            int tempw = MIN(m_obContentSize.width, m_obContentSize.height);
            m_pDecrementImageView->setScaleType(CAImageView::ScaleType::FitViewByVertical);
            m_pDecrementImageView->setCenter(DRect(m_obContentSize.width/2, m_obContentSize.height/4*3,
                                                   tempw,
                                                   tempw));
        }
        
        insertSubview(m_pDecrementImageView,10);
    }
}

void CAStepper::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
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
            m_value = MAX(m_value, m_minimumValue);
            m_value = MIN(m_value, m_maximumValue);
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
            m_value = MAX(m_value, m_minimumValue);
            m_value = MIN(m_value, m_maximumValue);
            break;
            
        case 0:
        default:
            bValueChange = false;
            break;
    }
    
    // send value change event
    if (bValueChange && m_function)
    {
        m_function(m_value);
    }
}

void CAStepper::repeat(float dt)
{
    action();
}

void CAStepper::click(CATouch* pTouch)
{
    DRect increseRect, decreaseRect;
    if (m_pOrientation==CAStepper::Orientation::Horizontal) {
        increseRect = getBounds();
        increseRect.origin.x = m_obContentSize.width/2;
        increseRect.size.width = m_obContentSize.width/2;
        decreaseRect = increseRect;
        decreaseRect.origin.x = 0;
    }else{
        decreaseRect = getBounds();
        decreaseRect.origin.y = m_obContentSize.height/2;
        decreaseRect.size.height = m_obContentSize.height/2;
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

void CAStepper::setContentSize(const DSize & var)
{
    DSize size = var;
    if (m_bRecSpe)
    {
        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAStepper");
        int w = atoi(map.at("width").c_str());
        int h = atoi(map.at("height").c_str());
        
        size.width = (w == 0) ? size.width : w;
        size.height = (h == 0) ? size.height : h;
        
        if (m_pOrientation == Orientation::Vertical)
        {
            size.width = size.width + size.height;
            size.height = size.width - size.height;
            size.width = size.width - size.height;
        }
    }
    CAControl::setContentSize(size);
    if (m_pBackgroundImageView)
    {
        m_pBackgroundImageView->setFrame(this->getBounds());
    }
    
    if (m_pDividerImageView) {

        if (m_pOrientation==CAStepper::Orientation::Horizontal)
        {
            m_pDividerImageView->setCenter(DRect(m_obContentSize.width/2,
                                                  m_obContentSize.height/2,
                                                  LINE_WIDTH,
                                                  m_obContentSize.height));
        }else{
            m_pDividerImageView->setCenter(DRect(m_obContentSize.width/2,
                                                  m_obContentSize.height/2,
                                                  m_obContentSize.width,
                                                   LINE_WIDTH));
        }
    }
    
    //float div = (m_pDividerImageView) ? m_pDividerImageView->m_obContentSize.width/2 : 0;
    
    // init increment
    if (m_pIncrementImageView) {
        if (m_pOrientation==CAStepper::Orientation::Horizontal) {
            int tempw = MIN(m_obContentSize.width, m_obContentSize.height);
            m_pIncrementImageView->setCenter(DRect(m_obContentSize.width/4*3, m_obContentSize.height/2,
                                                    tempw,
                                                    tempw));
        }else{
            int tempw = MIN(m_obContentSize.width, m_obContentSize.height);
            m_pIncrementImageView->setCenter(DRect(m_obContentSize.width/2, m_obContentSize.height/4,
                                                    tempw,
                                                    tempw));
        }
    }
    
    // init decrement
    if (m_pDecrementImageView) {
        if (m_pOrientation==CAStepper::Orientation::Horizontal) {
            int tempw = MIN(m_obContentSize.width, m_obContentSize.height);
            m_pDecrementImageView->setCenter(DRect(m_obContentSize.width/4, m_obContentSize.height/2,
                                                    tempw,
                                                    tempw));
        }else{
            int tempw = MIN(m_obContentSize.width, m_obContentSize.height);
            m_pDecrementImageView->setCenter(DRect(m_obContentSize.width/2, m_obContentSize.height/4*3,
                                                    tempw,
                                                    tempw));
        }
    }
}

void CAStepper::setTarget(const std::function<void(float)>& function)
{
    if (function) m_function = function;
}

NS_CC_END
