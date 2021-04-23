//
//  CAPageView.cpp
//  HelloCpp
//
//  Created by 栗元峰 on 14-7-29.
//
//

#include "CAPageView.h"
#include "basics/CAScheduler.h"
#include "animation/CACustomAnimation.h"

CAPageView::CAPageView(CAPageView::Orientation type)
:m_eOrientation(type)
,m_ePageViewState(CAPageView::State::None)
,m_nCurrPage(0)
,m_pPageViewDelegate(nullptr)
,m_bListener(false)
,m_fSpacing(0)
{

}

CAPageView::~CAPageView()
{
    m_pViews.clear();
}

CAPageView* CAPageView::createWithFrame(const DRect& rect, const Orientation& type)
{
    CAPageView* view = new CAPageView(type);
    if (view && view->initWithFrame(rect))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

CAPageView* CAPageView::createWithCenter(const DRect& rect, const Orientation& type)
{
    CAPageView* view = new CAPageView(type);
    if (view && view->initWithCenter(rect))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

CAPageView* CAPageView::createWithLayout(const CrossApp::DLayout &layout, const Orientation &type)
{
    CAPageView* view = new CAPageView(type);
    if (view && view->initWithLayout(layout))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

bool CAPageView::init()
{
    if (!CAScrollView::init())
    {
        return false;
    }
    
    this->setShowsScrollIndicators(false);

    if (m_eOrientation == CAPageView::Orientation::Horizontal)
    {
        this->setVerticalScrollEnabled(false);
    }
    else
    {
        this->setHorizontalScrollEnabled(false);
    }
    
    this->setBounces(false);
    
    return true;
}

void CAPageView::setViews(const CAVector<CAView*>& vec)
{
    if (!m_pViews.empty())
    {
        m_pContainer->removeAllSubviews();
        m_pViews.clear();
    }
    
    this->setViews(CADeque<CAView*>(vec));
}

void CAPageView::setViews(const CADeque<CAView*>& vec)
{
    if (!m_pViews.empty())
    {
        m_pContainer->removeAllSubviews();
        m_pViews.clear();
    }
    
    m_pViews = CADeque<CAView*>(vec);
    
    if (m_eOrientation == CAPageView::Orientation::Horizontal)
    {
        this->setViewSize(DSize(m_obContentSize.width * m_pViews.size() + m_fSpacing * (m_pViews.size() - 1), 0));
    }
    else
    {
        this->setViewSize(DSize(0, m_obContentSize.height * m_pViews.size() + m_fSpacing * (m_pViews.size() - 1)));
    }
    
    for (size_t i=0; i<m_pViews.size(); i++)
    {
        DRect rect = this->getBounds();
        if (m_eOrientation == CAPageView::Orientation::Horizontal)
        {
            rect.origin.x = (rect.size.width + m_fSpacing) * i;
        }
        else
        {
            rect.origin.y = (rect.size.height + m_fSpacing) * i;
        }
        m_pContainer->addSubview(m_pViews.at(i));
        m_pViews.at(i)->setFrame(rect);
    }
}

void CAPageView::setContentSize(const DSize& contentSize)
{
    CAScrollView::setContentSize(contentSize);
    
    if (!m_pViews.empty())
    {
        if (m_eOrientation == CAPageView::Orientation::Horizontal)
        {
            this->setViewSize(DSize(m_obContentSize.width * m_pViews.size() + m_fSpacing * (m_pViews.size() - 1), 0));
        }
        else
        {
            this->setViewSize(DSize(0, m_obContentSize.height * m_pViews.size() + m_fSpacing * (m_pViews.size() - 1)));
        }
        
        for (size_t i=0; i<m_pViews.size(); i++)
        {
            DRect rect = this->getBounds();
            if (m_eOrientation == CAPageView::Orientation::Horizontal)
            {
                rect.origin.x = (rect.size.width + m_fSpacing) * i;
            }
            else
            {
                rect.origin.y = (rect.size.height + m_fSpacing) * i;
            }
            m_pViews.at(i)->setFrame(rect);
        }
        
        this->contentOffsetFinish();
    }
}

CAView* CAPageView::getSubViewAtIndex(int index)
{
    CAView* view = NULL;
    
    do
    {
        CC_BREAK_IF(index < 0);
        CC_BREAK_IF((size_t)index >= m_pViews.size());
        view = m_pViews.at(index);
    }
    while (0);
    
    return view;
}

void CAPageView::setShowsScrollIndicators(bool var)
{
    bool bVertScroll = m_eOrientation == CAPageView::Orientation::Vertical;
    this->setShowsHorizontalScrollIndicator(var && !bVertScroll);
    this->setShowsVerticalScrollIndicator(var && bVertScroll);
    m_bShowsScrollIndicators = var;
}

unsigned int CAPageView::getPageCount()
{
    return (unsigned int)m_pViews.size();
}

void CAPageView::setCurrPage(int var, bool animated, bool listener)
{
    CC_RETURN_IF(m_pViews.empty());
    m_bListener = listener;
    var = MIN(var, this->getPageCount() - 1);
    var = MAX(var, 0);

    if (m_nCurrPage != var && m_bListener)
    {
        if (m_obBeginTurning)
        {
            m_obBeginTurning();
        }
        else if (m_pPageViewDelegate)
        {
            m_pPageViewDelegate->pageViewDidBeginTurning(this);
        }
    }
    
    m_nCurrPage = var;
    
    if (animated)
    {
        if (m_eOrientation == CAPageView::Orientation::Horizontal)
        {
            this->setContentOffset(DPoint(m_nCurrPage * (m_obContentSize.width + m_fSpacing), 0), animated);
        }
        else
        {
            this->setContentOffset(DPoint(0, m_nCurrPage * (m_obContentSize.height + m_fSpacing)), animated);
        }
    }
    else
    {
        this->contentOffsetFinish();
    }
    
}

int CAPageView::getCurrPage()
{
    return m_nCurrPage;
}

void CAPageView::setSpacing(int var)
{
    m_fSpacing = var;
    
    if (!m_pViews.empty())
    {
        CAVector<CAView*> vec = m_pViews;
        int currPath = m_nCurrPage;
        this->setViews(vec);
        this->setCurrPage(currPath, false, false);
    }
}

int CAPageView::getSpacing()
{
    return m_fSpacing;
}

void CAPageView::contentOffsetFinish()
{
    if (m_bListener && m_vTouches.empty())
    {
        if (m_obEndTurning)
        {
            m_obEndTurning();
        }
        else if (m_pPageViewDelegate)
        {
            m_pPageViewDelegate->pageViewDidEndTurning(this);
        }
        m_bListener = false;
    }
}

bool CAPageView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (CACustomAnimation::isSchedule("contentOffset:" + m_s__StrID))
    {
        return false;
    }
    
    return CAScrollView::ccTouchBegan(pTouch, pEvent);
}

void CAPageView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_pViews.empty());
    CAScrollView::ccTouchMoved(pTouch, pEvent);
}

void CAPageView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CAScrollView::ccTouchEnded(pTouch, pEvent);
    CC_RETURN_IF(m_vTouches.size() > 1);
    if (m_eOrientation == CAPageView::Orientation::Horizontal)
    {
        float off_x = -m_tInertia.x;
        
        if (off_x > 0)
        {
            m_ePageViewState = CAPageView::State::Next;
        }
        else if (off_x < 0)
        {
            m_ePageViewState = CAPageView::State::Last;
        }
        else
        {
            m_ePageViewState = CAPageView::State::None;
        }
    }
    else
    {
        float off_y = -m_tInertia.y;
        
        if (off_y > 0)
        {
            m_ePageViewState = CAPageView::State::Next;
        }
        else if (off_y < 0)
        {
            m_ePageViewState = CAPageView::State::Last;
        }
        else
        {
            m_ePageViewState = CAPageView::State::None;
        }
    }
    
    int page = this->getCurrPage();
    if (m_ePageViewState == CAPageView::State::Next)
    {
        page++;
    }
    else if (m_ePageViewState == CAPageView::State::Last)
    {
        page--;
    }
    else
    {
        if (m_eOrientation == CAPageView::Orientation::Horizontal)
        {
            float width = this->getBounds().size.width;
            float off_x = this->getContentOffset().x - this->getCurrPage() * width;
            
            if (off_x > width/2)
            {
                page++;
            }
            else if (off_x < -width/2)
            {
                page--;
            }
        }
        else
        {
            float height = this->getBounds().size.height;
            float off_y = this->getContentOffset().y - this->getCurrPage() * height;
            
            if (off_y > height/2)
            {
                page++;
            }
            else if (off_y < -height/2)
            {
                page--;
            }
        }
    }
    
    if (!pTouch->isDelta())
    {
        if (m_obDidSelectedPageAtIndex)
        {
            m_obDidSelectedPageAtIndex(this->getCurrPage(), this->convertTouchToNodeSpace(pTouch));
        }
        else if (m_pPageViewDelegate)
        {
            m_pPageViewDelegate->pageViewDidSelectedPageAtIndex(this, this->getCurrPage(), this->convertTouchToNodeSpace(pTouch));
        }
        m_bListener = false;
        if (m_eOrientation == CAPageView::Orientation::Horizontal)
        {
            this->setContentOffset(DPoint(m_nCurrPage * (m_obContentSize.width + m_fSpacing), 0), true);
        }
        else
        {
            this->setContentOffset(DPoint(0, m_nCurrPage * (m_obContentSize.height + m_fSpacing)), true);
        }
    }
    else
    {
        page = MIN(page, (int)this->getPageCount() - 1);
        page = MAX(page, 0);
        
        this->setCurrPage(page, true, true);
    }
}



