

#include "CAKeypadDispatcher.h"
#include "support/data_support/ccCArray.h"

NS_CC_BEGIN

CAKeypadDispatcher::CAKeypadDispatcher()
: m_bLocked(false)
, m_bToAdd(false)
, m_bToRemove(false)
{
    m_pDelegates = CCArray::create();
    m_pDelegates->retain();

    m_pHandlersToAdd    = ccCArrayNew(8);
    m_pHandlersToRemove = ccCArrayNew(8);
}

CAKeypadDispatcher::~CAKeypadDispatcher()
{
    CC_SAFE_RELEASE(m_pDelegates);
    if (m_pHandlersToAdd)
    {
        ccCArrayFree(m_pHandlersToAdd);
    }
    
    if (m_pHandlersToRemove)
    {
        ccCArrayFree(m_pHandlersToRemove);
    }    
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
        ccCArrayAppendValue(m_pHandlersToRemove, pDelegate);
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
        ccCArrayAppendValue(m_pHandlersToAdd, pDelegate);
        m_bToAdd = true;
    }
}

void CAKeypadDispatcher::forceAddDelegate(CAKeypadDelegate* pDelegate)
{
    CAKeypadHandler* pHandler = CAKeypadHandler::handlerWithDelegate(pDelegate);

    if (pHandler)
    {
        m_pDelegates->addObject(pHandler);
    }
}

void CAKeypadDispatcher::forceRemoveDelegate(CAKeypadDelegate* pDelegate)
{
    CAKeypadHandler* pHandler = NULL;
    CAObject* pObj = NULL;
    CCARRAY_FOREACH(m_pDelegates, pObj)
    {
        pHandler = (CAKeypadHandler*)pObj;
        if (pHandler && pHandler->getDelegate() == pDelegate)
        {
            m_pDelegates->removeObject(pHandler);
            break;
        }
    }
}

bool CAKeypadDispatcher::dispatchKeypadMSG(ccKeypadMSGType nMsgType)
{
    CAKeypadHandler*  pHandler = NULL;
    CAKeypadDelegate* pDelegate = NULL;

    m_bLocked = true;

    if (m_pDelegates->count() > 0)
    {
        CAObject* pObj = NULL;
        CCARRAY_FOREACH(m_pDelegates, pObj)
        {
            CC_BREAK_IF(!pObj);

            pHandler = (CAKeypadHandler*)pObj;
            pDelegate = pHandler->getDelegate();

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
    }

    m_bLocked = false;
    if (m_bToRemove)
    {
        m_bToRemove = false;
        for (unsigned int i = 0; i < m_pHandlersToRemove->num; ++i)
        {
            forceRemoveDelegate((CAKeypadDelegate*)m_pHandlersToRemove->arr[i]);
        }
        ccCArrayRemoveAllValues(m_pHandlersToRemove);
    }

    if (m_bToAdd)
    {
        m_bToAdd = false;
        for (unsigned int i = 0; i < m_pHandlersToAdd->num; ++i)
        {
            forceAddDelegate((CAKeypadDelegate*)m_pHandlersToAdd->arr[i]);
        }
        ccCArrayRemoveAllValues(m_pHandlersToAdd);
    }

    return true;
}

NS_CC_END
