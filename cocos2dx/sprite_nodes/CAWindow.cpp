
#include "CAWindow.h"
#include "support/CCPointExtension.h"
#include "CCDirector.h"

NS_CC_BEGIN

CAWindow::CAWindow()
:m_pRootViewController(NULL)
{
    m_bIgnoreAnchorPointForPosition = true;
    setAnchorPoint(ccp(0.5f, 0.5f));
}

CAWindow::~CAWindow()
{
    CC_SAFE_RELEASE_NULL(m_pRootViewController);
}

bool CAWindow::init()
{
    bool bRet = false;
     do 
     {
         CCDirector * pDirector;
         CC_BREAK_IF( ! (pDirector = CCDirector::sharedDirector()) );
         this->setContentSize(pDirector->getWinSize());
         // success
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
    }
}

CAViewController* CAWindow::getRootViewController()
{
    return m_pRootViewController;
}

NS_CC_END
