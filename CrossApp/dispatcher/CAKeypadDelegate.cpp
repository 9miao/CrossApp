

#include "CAKeypadDelegate.h"
#include "ccMacros.h"

NS_CC_BEGIN

CAKeypadDelegate* CAKeypadHandler::getDelegate()
{
    return m_pDelegate;
}

CAKeypadHandler::~CAKeypadHandler()
{
    if (m_pDelegate)
    {
        //dynamic_cast<CAObject*>(m_pDelegate)->release();
    }  
}

void CAKeypadHandler::setDelegate(CAKeypadDelegate *pDelegate)
{
    if (pDelegate)
    {
        dynamic_cast<CAObject*>(pDelegate)->retain();
    }

    if (m_pDelegate)
    {
        dynamic_cast<CAObject*>(m_pDelegate)->release();
    }
    m_pDelegate = pDelegate;
}

bool CAKeypadHandler::initWithDelegate(CAKeypadDelegate *pDelegate)
{
    CCAssert(pDelegate != NULL, "It's a wrong delegate!");

    m_pDelegate = pDelegate;
    //dynamic_cast<CAObject*>(pDelegate)->retain();

    return true;
}

CAKeypadHandler* CAKeypadHandler::handlerWithDelegate(CAKeypadDelegate *pDelegate)
{
    CAKeypadHandler* pHandler = new CAKeypadHandler;

    if (pHandler)
    {
        if (pHandler->initWithDelegate(pDelegate))
        {
            pHandler->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pHandler);
        }
    }

    return pHandler;
}

NS_CC_END
