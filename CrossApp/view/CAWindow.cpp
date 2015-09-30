
#include "CAWindow.h"
#include "support/CAPointExtension.h"
#include "basics/CAApplication.h"
#include "animation/CAViewAnimation.h"
#include "dispatcher/CATouchDispatcher.h"

NS_CC_BEGIN

CAWindow::CAWindow()
:m_pRootViewController(NULL)
,m_pModalViewController(NULL)
{
    this->setDisplayRange(false);
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
        DRect rect = DRectZero;
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
        m_pRootViewController->getView()->setZOrder(CAWindowZOderBottom);
    }
}

CAViewController* CAWindow::getRootViewController()
{
    return m_pRootViewController;
}

void CAWindow::presentModalViewController(CAViewController* controller, bool animated)
{
    CC_RETURN_IF(controller == NULL);
    CC_RETURN_IF(m_pModalViewController);
    CC_SAFE_RETAIN(controller);
    m_pModalViewController = controller;
    
    m_pModalViewController->addViewFromSuperview(this);
    m_pModalViewController->getView()->setZOrder(CAWindowZOderCenter);
    m_pModalViewController->viewDidAppear();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();
        DRect frame = view->getFrame();
        frame.origin.y = frame.size.height;
        view->setFrame(frame);
        
        DRect endFrame = DRectZero;
        endFrame.size = view->getFrame().size;
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.1f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveLinear);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CAWindow::presentEnd));
        view->setFrame(endFrame);
        CAViewAnimation::commitAnimations();
    }
    else
    {
        this->presentEnd();
    }
}

void CAWindow::presentEnd()
{
    if (m_pRootViewController)
    {
        m_pRootViewController->viewDidDisappear();
    }
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

void CAWindow::dismissModalViewController(bool animated)
{
    CC_RETURN_IF(m_pModalViewController == NULL);
    
    if (m_pRootViewController)
    {
        m_pRootViewController->viewDidAppear();
    }
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();
        
        DRect endFrame = view->getFrame();
        endFrame.origin.y = endFrame.size.height;
        
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
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

NS_CC_END
