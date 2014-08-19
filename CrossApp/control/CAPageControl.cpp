//
//  CAPageControl.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-7.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CAPageControl.h"
#include "view/CAImageView.h"
#include "images/CAImage.h"

NS_CC_BEGIN

CAPageControl::CAPageControl()
: m_numberOfPages(0)
, m_currentPage(0)
, m_bSinglePage(false)
, m_bDefersCurrentPageDisplay(false)
, m_pageIndicatorTintColor(ccc4(127, 127, 127, 255))
, m_currentPageIndicatorTintColor(ccc4(51, 204, 255, 255))
, m_pPageImage(NULL)
, m_pSelectPageImage(NULL)
{
    
}

CAPageControl::~CAPageControl()
{
    m_pIndicators.clear();
    CC_SAFE_RELEASE(m_pPageImage);
    CC_SAFE_RELEASE(m_pSelectPageImage);
}

CAPageControl* CAPageControl::create()
{
    CAPageControl* page = new CAPageControl();
    
    if (page && page->init())
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

CAPageControl* CAPageControl::createWithFrame(const CCRect& rect)
{
    CAPageControl* page = new CAPageControl();
    
    if (page && page->initWithFrame(rect))
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

CAPageControl* CAPageControl::createWithCenter(const CCRect& rect)
{
    CAPageControl* page = new CAPageControl();
    
    if (page && page->initWithCenter(rect))
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

bool CAPageControl::init()
{
    if (!CAControl::init())
    {
        return false;
    }
    
    m_pPageImage = CAImage::create("source_material/WhiteDots.png");
    m_pPageImage->retain();
    
    m_pSelectPageImage = CAImage::create("source_material/WhiteDots.png");
    m_pSelectPageImage->retain();
    
    setTouchEnabled(true);
    
    return true;
}

bool CAPageControl::initWithFrame(const CCRect& rect)
{
    if (!CAControl::initWithFrame(rect))
    {
        return false;
    }
    
    return true;
}

bool CAPageControl::initWithCenter(const CCRect& rect)
{
    if (!CAControl::initWithCenter(rect))
    {
        return false;
    }
    
    return true;
}

void CAPageControl::updateCurrentPageDisplay()
{
    for (int i=0; i<m_numberOfPages; i++) {
        CAImageView* view = m_pIndicators.at(i);
        if (i == m_currentPage) {
            view->setImage(m_pSelectPageImage);
            view->setColor(m_currentPageIndicatorTintColor);
        } else {
            view->setImage(m_pPageImage);
            view->setColor(m_pageIndicatorTintColor);
        }
    }
    
    updateDraw();
}

void CAPageControl::onEnter()
{
    CAControl::onEnter();
    
    if (m_pIndicators.size() != m_numberOfPages) {
        m_pIndicators.clear();
        
        for (int i=0; i<m_numberOfPages; i++) {
            CAImageView* view;
            if (i == m_currentPage) {
                view = CAImageView::createWithImage(m_pSelectPageImage);
                view->setColor(m_currentPageIndicatorTintColor);
            } else {
                view = CAImageView::createWithImage(m_pPageImage);        
                view->setColor(m_pageIndicatorTintColor);
            }
            
            CCRect rect;
            float width = getFrame().size.width/m_numberOfPages;
            rect.origin.x = i * width + width/2;
            rect.origin.y = getFrame().size.height/2;
            view->setCenterOrigin(rect.origin);
            
            addSubview(view);
            
            m_pIndicators.pushBack(view);
        }
    }
}

void CAPageControl::onExit()
{
    CAControl::onExit();
}

void CAPageControl::visit()
{
    CAControl::visit();
}

bool CAPageControl::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (getBounds().containsPoint(convertToNodeSpace(pTouch->getLocation()))) {
        return true;
    }
    return false;
}

void CAPageControl::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CAPageControl::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    if (getBounds().containsPoint(convertToNodeSpace(pTouch->getLocation()))) {
        
        m_currentPage++;
        if (m_currentPage == m_numberOfPages) {
            m_currentPage = 0;
        }

        if (!m_bDefersCurrentPageDisplay) {
            updateCurrentPageDisplay();
        }
        
        if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged]) {
            (m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, CCPointZero);
        }
    }
}

void CAPageControl::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    
}


NS_CC_END