
#include "CAWindow.h"
#include "support/CCPointExtension.h"
#include "basics/CAApplication.h"
#include "animation/CAViewAnimation.h"
#include "dispatcher/CATouchDispatcher.h"

NS_CC_BEGIN

CAWindow::CAWindow()
:m_pRootViewController(NULL)
,m_pModalViewController(NULL)
{

}

CAWindow::~CAWindow()
{
    CC_SAFE_RELEASE_NULL(m_pRootViewController);
    CC_SAFE_RELEASE_NULL(m_pModalViewController);
}

bool CAWindow::init()
{
    CAView::init();
    bool bRet = false;
    if (CAApplication* pApplication = CAApplication::getApplication())
    {
        CCRect rect = CCRectZero;
        rect.size = pApplication->getWinSize();
        this->setFrame(rect);
        bRet = true;
    }
    
     return bRet;
}

CAWindow *CAWindow::create()
{
    CAWindow *pRet = new CAWindow();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void CAWindow::setRootViewController(CrossApp::CAViewController *var)
{
    if (m_pRootViewController)
    {
        m_pRootViewController->removeViewFromSuperview();
        CC_SAFE_RELEASE_NULL(m_pRootViewController);
    }
    
    if (var)
    {
        var->retain();
        m_pRootViewController = var;
        m_pRootViewController->addViewFromSuperview(this);
        m_pRootViewController->getView()->setZOrder(CAWindowZoderBottom);
    }
}

CAViewController* CAWindow::getRootViewController()
{
    return m_pRootViewController;
}

void setDispatchEvents(CAWindow* window, bool var)
{
    if (var == false)
    {
        CAView* view = CAView::createWithFrame(window->getBounds(), CAColor_clear);
        view->setHaveNextResponder(false);
        view->setTextTag("DispatchEvents");
        window->insertSubview(view, 0xffffffff);
    }
    else
    {
        window->removeSubviewByTextTag("DispatchEvents");
    }
}

void CAWindow::presentModalViewController(CAViewController* controller, bool animated)
{
    CC_RETURN_IF(controller == NULL);
    CC_RETURN_IF(m_pModalViewController);
    CC_SAFE_RETAIN(controller);
    m_pModalViewController = controller;
    
    m_pModalViewController->addViewFromSuperview(this);
    m_pModalViewController->getView()->setZOrder(CAWindowZoderCenter);
    m_pModalViewController->viewDidAppear();
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();
        CCRect frame = view->getFrame();
        frame.origin.y = frame.size.height;
        view->setFrame(frame);
        
        CCRect endFrame = CCRectZero;
        endFrame.size = view->getFrame().size;
        setDispatchEvents(this, false);
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.1f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveLinear);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CAWindow::presentEnd));
        view->setFrame(endFrame);
        CAViewAnimation::commitAnimations();
    }
}

void CAWindow::presentEnd()
{
    if (m_pRootViewController)
    {
        m_pRootViewController->viewDidDisappear();
    }
    setDispatchEvents(this, true);
}

void CAWindow::dismissModalViewController(bool animated)
{
    CC_RETURN_IF(m_pModalViewController == NULL);
    
    if (m_pRootViewController)
    {
        m_pRootViewController->viewDidAppear();
    }
    
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();
        
        CCRect endFrame = view->getFrame();
        endFrame.origin.y = endFrame.size.height;
        
        setDispatchEvents(this, false);
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.1f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveLinear);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CAWindow::dismissEnd));
        view->setFrame(endFrame);
        CAViewAnimation::commitAnimations();
    }
    else
    {
        this->dismissEnd();
    }
    
}

void CAWindow::dismissEnd()
{
    m_pModalViewController->viewDidDisappear();
    m_pModalViewController->removeViewFromSuperview();
    CC_SAFE_RELEASE_NULL(m_pModalViewController);
    setDispatchEvents(this, true);
}

NS_CC_END
