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
#include "basics/CAApplication.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

CAPageControl::CAPageControl()
: m_numberOfPages(0)
, m_currentPage(0)
, m_bSinglePage(false)
, m_bDefersCurrentPageDisplay(false)
, m_pageIndicatorTintColor(CAColor4B(255, 255, 255, 255))
, m_currentPageIndicatorTintColor(CAColor4B(255, 255, 255, 255))
, m_pPageImage(nullptr)
, m_pSelectPageImage(nullptr)
, m_style(CAPageControl::Style::Dot)
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

CAPageControl* CAPageControl::createWithFrame(const DRect& rect)
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

CAPageControl* CAPageControl::createWithCenter(const DRect& rect)
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

CAPageControl* CAPageControl::createWithLayout(const CrossApp::DLayout &layout)
{
    CAPageControl* page = new CAPageControl();
    
    if (page && page->initWithLayout(layout))
    {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return NULL;
}

bool CAPageControl::init()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAPageControl");
    setPageIndicatorImage(CAImage::create(map.at("backgroundView_normal_custom")));
    setCurrIndicatorImage(CAImage::create(map.at("backgroundView_selected_custom")));

    return true;
}

bool CAPageControl::initWithFrame(const DRect& rect)
{
    if (!CAControl::initWithFrame(rect))
    {
        return false;
    }
    
    return true;
}

bool CAPageControl::initWithCenter(const DRect& rect)
{
    if (!CAControl::initWithCenter(rect))
    {
        return false;
    }
    
    return true;
}

void CAPageControl::updateCurrentPageDisplay()
{
    for (int i=0; i<m_numberOfPages; i++)
    {
        CAImageView* view = m_pIndicators.at(i);
        if (i == m_currentPage)
        {
            view->setImage(m_pSelectPageImage);
            view->setColor(m_currentPageIndicatorTintColor);
        }
        else
        {
            view->setImage(m_pPageImage);
            view->setColor(m_pageIndicatorTintColor);
        }
    }
    
    updateDraw();
}

void CAPageControl::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    if (m_pIndicators.size() != m_numberOfPages)
    {
        m_pIndicators.clear();
        
        for (int i=0; i<m_numberOfPages; i++)
        {
            CAImageView* view;
            float w = getFrame().size.width/m_numberOfPages;
            w = (m_style == CAPageControl::Style::Dot) ? 14 : (w-10);
            if (i == m_currentPage)
            {
                view = CAImageView::createWithImage(m_pSelectPageImage);
                view->setColor(m_currentPageIndicatorTintColor);
                view->setFrame(DRect(0,0,w,14));
            }
            else
            {
                view = CAImageView::createWithImage(m_pPageImage);        
                view->setColor(m_pageIndicatorTintColor);
                view->setFrame(DRect(0,0,w,14));
            }
            DRect rect;
            float width = getFrame().size.width/m_numberOfPages;
            rect.origin.x = i * width + width/2;
            rect.origin.y = getFrame().size.height/2;
            view->setCenterOrigin(rect.origin);
            
            addSubview(view);
            
            m_pIndicators.pushBack(view);
        }
    }
}

void CAPageControl::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

bool CAPageControl::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return true;
}

void CAPageControl::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CAPageControl::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    float width = m_obContentSize.width/m_numberOfPages;
    DRect rect = this->getBounds();
    for (int i=0; i<m_numberOfPages; i++)
    {
        rect.size.width = width * i + width;
        if (rect.containsPoint(point))
        {
            if (m_currentPage != i)
            {
                m_currentPage = i;
                
                if (!m_bDefersCurrentPageDisplay)
                    updateCurrentPageDisplay();
                
                if (m_function)
                    m_function(m_currentPage);
            }
            break;
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

void CAPageControl::setStyle(CAPageControl::Style var)
{
    if (m_style != var)
    {
        m_style = var;
        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAPageControl");
        switch (m_style) {
            case CAPageControl::Style::Dot:
                setPageIndicatorImage(CAImage::create(map.at("backgroundView_normal_custom")));
                setCurrIndicatorImage(CAImage::create(map.at("backgroundView_selected_custom")));
                break;
            case CAPageControl::Style::Round:
                setPageIndicatorImage(CAImage::create(map.at("backgroundView_normal_round")));
                setCurrIndicatorImage(CAImage::create(map.at("backgroundView_selected_round")));
                break;
            case CAPageControl::Style::Rectangle:
                setPageIndicatorImage(CAImage::create(map.at("backgroundView_normal_rect")));
                setCurrIndicatorImage(CAImage::create(map.at("backgroundView_selected_rect")));
                break;
                
            default:
                break;
        }
    }
}

CAPageControl::Style CAPageControl::getStyle()
{
    return m_style;
}

void CAPageControl::setTarget(const std::function<void(int)>& function)
{
    if (function) m_function = function;
}

NS_CC_END
