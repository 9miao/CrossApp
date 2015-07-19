//
//  CAPageControl.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-7.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
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
, m_currentPageIndicatorTintColor(ccc4(255, 255, 255, 255))
, m_pPageImage(NULL)
, m_pSelectPageImage(NULL)
, m_style(CAPageControlStyleDot)
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
    setPageIndicatorImage(CAImage::create("source_material/page_n.png"));
    setCurrIndicatorImage(CAImage::create("source_material/page_h.png"));

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

//        m_currentPage++;
//        if (m_currentPage == m_numberOfPages) {
//            m_currentPage = 0;
//        }
//        if (!m_bDefersCurrentPageDisplay) {
//            updateCurrentPageDisplay();
//        }
//        
//        if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged]) {
//            (m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, CCPointZero);
//        }

        // find touched dot
        float width = getBounds().size.width/m_numberOfPages;
        CCRect rect = getBounds();
        for (int i=0; i<m_numberOfPages; i++) {
            rect.size.width = width * i + width;
            if (rect.containsPoint(convertToNodeSpace(pTouch->getLocation()))) {
                if (m_currentPage != i) {
                    m_currentPage = i;

                    if (!m_bDefersCurrentPageDisplay) {
                        updateCurrentPageDisplay();
                    }
                    
                    if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged]) {
                        (m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, CCPointZero);
                    }
                }
                break;
            }
        }
    }
}

void CAPageControl::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CAPageControl::setTouchEnabled(bool enable)
{
    CAControl::setTouchEnabled(enable);
    
    
}

void CAPageControl::setStyle(const CAPageControlStyle &var)
{
    if (m_style != var) {
        m_style = var;
        
        switch (m_style) {
            case CAPageControlStyleDot:
                setPageIndicatorImage(CAImage::create("source_material/page_n.png"));
                setCurrIndicatorImage(CAImage::create("source_material/page_h.png"));
                break;
            case CAPageControlStyleRound:
                setPageIndicatorImage(CAImage::create("source_material/page_round_n.png"));
                setCurrIndicatorImage(CAImage::create("source_material/page_round_h.png"));
                break;
            case CAPageControlStyleRectangle:
                setPageIndicatorImage(CAImage::create("source_material/page_rect_n.png"));
                setCurrIndicatorImage(CAImage::create("source_material/page_rect_h.png"));
                break;
                
            default:
                break;
        }
    }
}

const CAPageControlStyle& CAPageControl::getStyle()
{
    return m_style;
}

void CAPageControl::addTarget(CAObject* target, SEL_CAControl selector)
{
    CAControl::addTarget(target, selector, CAControlEventTouchValueChanged);
}

void CAPageControl::removeTarget(CAObject* target, SEL_CAControl selector)
{
    CAControl::removeTarget(target, selector, CAControlEventTouchValueChanged);
}
NS_CC_END