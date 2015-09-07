//
//  CAPageView.cpp
//  HelloCpp
//
//  Created by 栗元峰 on 14-7-29.
//
//

#include "CAPageView.h"
#include "basics/CAScheduler.h"

#define s_fSpacing _px(0)

CAPageView::CAPageView(const CAPageViewDirection& type)
:m_ePageViewDirection(type)
,m_ePageViewState(None)
,m_nCurrPage(0)
,m_pPageViewDelegate(NULL)
,m_bListener(false)
,m_fSpacing(_px(40))
{

}

CAPageView::~CAPageView()
{
    m_pViews.clear();
}

CAPageView* CAPageView::createWithFrame(const CCRect& rect, const CAPageViewDirection& type)
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

CAPageView* CAPageView::createWithCenter(const CCRect& rect, const CAPageViewDirection& type)
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

bool CAPageView::init()
{
    if (!CAScrollView::init())
    {
        return false;
    }
    
    this->setShowsScrollIndicators(false);

    if (m_ePageViewDirection == CAPageViewDirectionHorizontal)
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
    
    if (m_ePageViewDirection == CAPageViewDirectionHorizontal)
    {
        this->setViewSize(CCSize(this->getBounds().size.width * m_pViews.size() + m_fSpacing * (m_pViews.size() - 1), m_obViewSize.height));
    }
    else
    {
        this->setViewSize(CCSize(m_obViewSize.width, this->getBounds().size.height * m_pViews.size() + m_fSpacing * (m_pViews.size() - 1)));
    }
    
    for (size_t i=0; i<m_pViews.size(); i++)
    {
        CCRect rect = this->getBounds();
        if (m_ePageViewDirection == CAPageViewDirectionHorizontal)
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
    bool bVertScroll = m_ePageViewDirection == CAPageViewDirectionVertical;
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

    if (m_pPageViewDelegate && m_nCurrPage != var && m_bListener)
    {
        m_pPageViewDelegate->pageViewDidBeginTurning(this);
    }
    
    m_nCurrPage = var;
    
    if (m_ePageViewDirection == CAPageViewDirectionHorizontal)
    {
        this->setContentOffset(CCPoint(m_nCurrPage * (this->getBounds().size.width + m_fSpacing), 0), animated);
    }
    else
    {
        this->setContentOffset(CCPoint(0, m_nCurrPage * (this->getBounds().size.height + m_fSpacing)), animated);
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

void CAPageView::contentOffsetFinish(float dt)
{
    CAScrollView::contentOffsetFinish(dt);
    if (m_pPageViewDelegate && m_bListener)
    {
        m_pPageViewDelegate->pageViewDidEndTurning(this);
        m_bListener = false;
    }
}

bool CAPageView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (!m_vTouches.empty())
    {
        m_vTouches.replace(0, pTouch);
        return true;
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
    if (m_ePageViewDirection == CAPageViewDirectionHorizontal)
    {
        float off_x = -m_tInertia.x;
        
        if (off_x > 0)
        {
            m_ePageViewState = Next;
        }
        else if (off_x < 0)
        {
            m_ePageViewState = Last;
        }
        else
        {
            m_ePageViewState = None;
        }
    }
    else
    {
        float off_y = -m_tInertia.y;
        
        if (off_y > 0)
        {
            m_ePageViewState = Next;
        }
        else if (off_y < 0)
        {
            m_ePageViewState = Last;
        }
        else
        {
            m_ePageViewState = None;
        }
    }
    
    int page = this->getCurrPage();
    if (m_ePageViewState == Next)
    {
        page++;
    }
    else if (m_ePageViewState == Last)
    {
        page--;
    }
    else
    {
        if (m_ePageViewDirection == CAPageViewDirectionHorizontal)
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
        if (m_pPageViewDelegate)
        {
            CCPoint point = this->convertTouchToNodeSpace(pTouch);
            m_pPageViewDelegate->pageViewDidSelectPageAtIndex(this, this->getCurrPage(), point);
        }
    }
    else
    {
        page = MIN(page, (int)this->getPageCount() - 1);
        page = MAX(page, 0);
        
        this->setCurrPage(page, true, true);
    }
}



