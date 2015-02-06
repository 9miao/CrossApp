
#include "CAWindow.h"
#include "support/CCPointExtension.h"
#include "basics/CAApplication.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionEase.h"
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

void CAWindow::presentModalViewController(CAViewController* controller, bool animated)
{
    CC_RETURN_IF(controller == NULL);
    CC_RETURN_IF(m_pModalViewController);
    CC_SAFE_RETAIN(controller);
    m_pModalViewController = controller;
    
    m_pModalViewController->addViewFromSuperview(this);
    m_pModalViewController->getView()->setZOrder(CAWindowZoderCenter);
    
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();
        CCRect frame = view->getFrame();
        frame.origin.y = frame.size.height;
        view->setFrame(frame);
        
        CCRect endFrame = CCRectZero;
        endFrame.size = view->getFrame().size;
        
        CCCallFunc* start = CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                               callfunc_selector(CATouchDispatcher::setDispatchEventsFalse));
        CCCallFunc* end = CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                             callfunc_selector(CATouchDispatcher::setDispatchEventsTrue));
        CCDelayTime* delayTime = CCDelayTime::create(0.1f);
        CCFrameTo* frameTo = CCFrameTo::create(0.3f, endFrame);
        CCEaseSineOut* easeBack = CCEaseSineOut::create(frameTo);
        CCSequence* allActions = CCSequence::create(start, delayTime, easeBack, end, NULL);
        view->runAction(allActions);
    }
}

void CAWindow::dismissModalViewController(bool animated)
{
    CC_RETURN_IF(m_pModalViewController == NULL);
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();
        
        CCRect endFrame = view->getFrame();
        endFrame.origin.y = endFrame.size.height;
        
        CCCallFunc* start = CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                               callfunc_selector(CATouchDispatcher::setDispatchEventsFalse));
        CCCallFunc* end = CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                             callfunc_selector(CATouchDispatcher::setDispatchEventsTrue));
        CCCallFunc* dissEnd = CCCallFunc::create(this, callfunc_selector(CAWindow::dismissEnd));
        CCFrameTo* frameTo = CCFrameTo::create(0.3f, endFrame);
        CCEaseSineIn* easeBack = CCEaseSineIn::create(frameTo);
        CCSequence* allActions = CCSequence::create(start, easeBack, dissEnd, end, NULL);
        view->runAction(allActions);
    }
    else
    {
        this->dismissEnd();
    }
    
}

void CAWindow::dismissEnd()
{
    m_pModalViewController->removeViewFromSuperview();
    CC_SAFE_RELEASE_NULL(m_pModalViewController);
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEvents(true);
}

NS_CC_END
