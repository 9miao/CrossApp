//
//  CASplitViewController.cpp
//  HelloCpp
//
//  Created by 栗元峰 on 15/7/23.
//
//

#include "CASplitViewController.h"
#include "animation/CAAnimation.h"
#include "basics/CAApplication.h"
#include "dispatcher/CATouchDispatcher.h"

CASplitViewController::CASplitViewController()
:m_pBackgroundView(NULL)
{
    this->getView()->setColor(CAColor_clear);
}

CASplitViewController::~CASplitViewController()
{

}

bool CASplitViewController::initWithController(const CAVector<CAViewController*>& vec, const std::vector<unsigned int>& widths)
{
    m_pViewControllers = vec;
    m_iWidths = widths;
    
    return true;
}

CAViewController* CASplitViewController::getViewControllerAtIndex(int index)
{
    return m_pViewControllers.at(index);
}

void CASplitViewController::resetViewControllerAtIndex(const vector<unsigned int>& widths)
{
    CCSize size = this->getView()->getBounds().size;
    m_iWidths.clear();
    m_iWidths = widths;
    unsigned int x = 0;
    for (size_t i=0; i<m_pViewControllers.size(); i++)
    {
        CAView* view = m_pViewControllers.at(i)->getView()->getSuperview();
        
        CCRect rect;
        rect.origin.x = x;
        rect.origin.y = 0;
        rect.size.width = widths.at(i);
        rect.size.height = size.height;
        view->setFrame(rect);
        m_pViewControllers.at(i)->getView()->setFrame(view->getBounds());
        x += widths.at(i);
    }
}

void CASplitViewController::viewDidLoad()
{
    CCSize size = this->getView()->getBounds().size;
    
    unsigned int x = 0;
    for (size_t i=0; i<m_pViewControllers.size(); i++)
    {
        CAView* view = new CAView();
        
        CCRect rect;
        rect.origin.x = x;
        rect.origin.y = 0;
        rect.size.width = m_iWidths.at(i);
        rect.size.height = size.height;
        view->setFrame(rect);
        this->getView()->addSubview(view);
        view->release();
        m_pViewControllers.at(i)->addViewFromSuperview(view);
        m_pViewControllers.at(i)->viewDidAppear();
        
        x += m_iWidths.at(i);
    }
    
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getView()->getBounds());
    }
}

void CASplitViewController::viewDidUnload()
{
    
}

void CASplitViewController::viewDidAppear()
{
    for (size_t i=0; i<m_pViewControllers.size(); i++)
    {
        m_pViewControllers.at(i)->viewDidAppear();
    }
}

void CASplitViewController::viewDidDisappear()
{
    for (size_t i=0; i<m_pViewControllers.size(); i++)
    {
        m_pViewControllers.at(i)->viewDidDisappear();
    }
}

void CASplitViewController::reshapeViewRectDidFinish()
{
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getView()->getBounds());
    }

}

void CASplitViewController::setBackgroundView(CrossApp::CAView *var)
{
    this->getView()->removeSubview(m_pBackgroundView);
    m_pBackgroundView = var;
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getView()->getBounds());
        this->getView()->insertSubview(m_pBackgroundView, -1);
    }
}

CAView* CASplitViewController::getBackgroundView()
{
    return m_pBackgroundView;
}

