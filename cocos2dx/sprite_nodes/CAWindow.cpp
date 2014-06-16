
#include "CAWindow.h"
#include "support/CCPointExtension.h"
#include "CCDirector.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionEase.h"
#include "touch_dispatcher/CATouchDispatcher.h"
enum
{
    CAWindowZoderBottom = 0,
    CAWindowZoderCenter = 1,
    CAWindowZoderTop = 0xffff
};

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
    bool bRet = false;
     do 
     {
         CCDirector * pDirector;
         CC_BREAK_IF( ! (pDirector = CCDirector::sharedDirector()) );
         CCRect rect = CCRectZero;
         rect.size = pDirector->getWinSize();
         this->setFrame(rect);
         
         bRet = true;
     } while (0);
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

void CAWindow::setRootViewController(cocos2d::CAViewController *var)
{
    if (m_pRootViewController)
    {
        m_pRootViewController->removeViewFromSuperview();
        m_pRootViewController->release();
        m_pRootViewController = NULL;
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
        
        CCCallFunc* start = CCCallFunc::create(this, callfunc_selector(CAWindow::presentStart));
        CCCallFunc* end = CCCallFunc::create(this, callfunc_selector(CAWindow::presentEnd));
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
        
        CCCallFunc* start = CCCallFunc::create(this, callfunc_selector(CAWindow::dismissStart));
        CCCallFunc* end = CCCallFunc::create(this, callfunc_selector(CAWindow::dismissEnd));
        CCFrameTo* frameTo = CCFrameTo::create(0.3f, endFrame);
        CCEaseSineIn* easeBack = CCEaseSineIn::create(frameTo);
        CCSequence* allActions = CCSequence::create(start, easeBack, end, NULL);
        view->runAction(allActions);
    }
    else
    {
        this->dismissEnd();
    }
    
}

void CAWindow::presentStart()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(false);
}

void CAWindow::presentEnd()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(true);
}

void CAWindow::dismissStart()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(false);
}

void CAWindow::dismissEnd()
{
    m_pModalViewController->removeViewFromSuperview();
    CC_SAFE_RELEASE_NULL(m_pModalViewController);
    CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(true);
}

NS_CC_END
