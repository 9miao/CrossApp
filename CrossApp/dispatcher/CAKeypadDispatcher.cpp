

#include "CAKeypadDispatcher.h"

NS_CC_BEGIN

CAKeypadDispatcher::CAKeypadDispatcher()
: m_bLocked(false)
, m_bToAdd(false)
, m_bToRemove(false)
{

}

CAKeypadDispatcher::~CAKeypadDispatcher()
{
    m_vDelegates.clear();
    m_vHandlersToAdd.clear();
    m_vHandlersToRemove.clear();
}

void CAKeypadDispatcher::removeDelegate(CAKeypadDelegate* pDelegate)
{
    if (!pDelegate)
    {
        return;
    }
    if (! m_bLocked)
    {
        forceRemoveDelegate(pDelegate);
    }
    else
    {
        m_vHandlersToRemove.push_back(pDelegate);
        m_bToRemove = true;
    }
}

void CAKeypadDispatcher::addDelegate(CAKeypadDelegate* pDelegate)
{
    if (!pDelegate)
    {
        return;
    }

    if (! m_bLocked)
    {
        forceAddDelegate(pDelegate);
    }
    else
    {
        m_vHandlersToAdd.push_back(pDelegate);
        m_bToAdd = true;
    }
}

void CAKeypadDispatcher::forceAddDelegate(CAKeypadDelegate* pDelegate)
{
    CAKeypadHandler* pHandler = CAKeypadHandler::handlerWithDelegate(pDelegate);

    if (pHandler)
    {
        m_vDelegates.pushBack(dynamic_cast<CAObject*>(pHandler));
    }
}

void CAKeypadDispatcher::forceRemoveDelegate(CAKeypadDelegate* pDelegate)
{
    CAKeypadHandler* pHandler = NULL;
    std::vector<CAObject*>::iterator itr;
    for (itr=m_vDelegates.begin(); itr!=m_vDelegates.end(); itr++)
    {
        pHandler = dynamic_cast<CAKeypadHandler*>(*itr);
        if (pHandler && pHandler->getDelegate() == pDelegate)
        {
            m_vDelegates.erase(itr);
            break;
        }
    }
    
}

bool CAKeypadDispatcher::dispatchKeypadMSG(ccKeypadMSGType nMsgType)
{
    CAKeypadHandler*  pHandler = NULL;
    CAKeypadDelegate* pDelegate = NULL;

    m_bLocked = true;

    std::vector<CAObject*>::iterator itr;
    for (itr=m_vDelegates.begin(); itr!=m_vDelegates.end(); itr++)
    {
        pHandler = dynamic_cast<CAKeypadHandler*>(*itr);
        pDelegate = pHandler->getDelegate();
        CC_BREAK_IF(!pHandler);
        switch (nMsgType)
        {
            case kTypeBackClicked:
                pDelegate->keyBackClicked();
                break;
            case kTypeMenuClicked:
                pDelegate->keyMenuClicked();
                break;
            default:
                break;
        }
    }
    
    m_bLocked = false;
    if (m_bToRemove)
    {
        m_bToRemove = false;
        
        for (unsigned int i = 0; i < m_vHandlersToRemove.size(); ++i)
        {
            forceRemoveDelegate(m_vHandlersToRemove.at(i));
        }
        m_vHandlersToRemove.clear();
    }

    if (m_bToAdd)
    {
        m_bToAdd = false;
        for (unsigned int i = 0; i < m_vHandlersToAdd.size(); ++i)
        {
            forceAddDelegate((CAKeypadDelegate*)m_vHandlersToAdd.at(i));
        }
        m_vHandlersToAdd.clear();
    }

    return true;
}

NS_CC_END
