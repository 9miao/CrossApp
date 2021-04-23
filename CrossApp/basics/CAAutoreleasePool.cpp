
#include "CAAutoreleasePool.h"
#include "ccMacros.h"

NS_CC_BEGIN

static CAPoolManager* s_pPoolManager = NULL;

CAAutoreleasePool::CAAutoreleasePool(void)
{
    m_pManagedObjectArray = new std::vector<CAObject*>();
}

CAAutoreleasePool::~CAAutoreleasePool(void)
{
    CC_SAFE_DELETE(m_pManagedObjectArray);
}

void CAAutoreleasePool::addObject(CAObject* pObject)
{
    m_pManagedObjectArray->push_back(pObject);
    
    CCAssert(pObject->m_uReference > 0, "reference count should be greater than 0");
    ++(pObject->m_uAutoReleaseCount);
}

void CAAutoreleasePool::removeObject(CAObject* pObject)
{
    for (unsigned int i = 0; i < pObject->m_uAutoReleaseCount; ++i)
    {
        std::vector<CAObject*>::iterator itr = std::find(m_pManagedObjectArray->begin(),
                                                         m_pManagedObjectArray->end(),
                                                         pObject);
        if (itr != m_pManagedObjectArray->end())
        {
            m_pManagedObjectArray->erase(itr);
        }
    }
}

void CAAutoreleasePool::clear()
{
    if(!m_pManagedObjectArray->empty())
    {
#ifdef _DEBUG
        size_t nIndex = m_pManagedObjectArray->size() - 1;
#endif
        while (m_pManagedObjectArray->size())
        {
            CAObject* obj = m_pManagedObjectArray->back();
            --obj->m_uAutoReleaseCount;
            obj->release();
            m_pManagedObjectArray->pop_back();
#ifdef _DEBUG
            nIndex--;
#endif
        }
    }
}


//--------------------------------------------------------------------
//
// CAPoolManager
//
//--------------------------------------------------------------------

CAPoolManager* CAPoolManager::getInstance()
{
    if (s_pPoolManager == NULL)
    {
        s_pPoolManager = new CAPoolManager();
    }
    return s_pPoolManager;
}

void CAPoolManager::destroyInstance()
{
    CC_SAFE_DELETE(s_pPoolManager);
}

CAPoolManager::CAPoolManager()
{
    m_pReleasePoolStack = new CADeque<CAAutoreleasePool*>();
    m_pCurReleasePool = 0;
}

CAPoolManager::~CAPoolManager()
{
    
    finalize();
    
    // we only release the last autorelease pool here
    m_pCurReleasePool = 0;
    m_pReleasePoolStack->clear();
    
    CC_SAFE_DELETE(m_pReleasePoolStack);
}

void CAPoolManager::finalize()
{
    if(!m_pReleasePoolStack->empty())
    {
        for (CADeque<CAAutoreleasePool*>::iterator itr=m_pReleasePoolStack->begin();
             itr!=m_pReleasePoolStack->end();
             itr++)
        {
            (*itr)->clear();
        }
    }
}

void CAPoolManager::push()
{
    CAAutoreleasePool* pPool = new CAAutoreleasePool();       //ref = 1
    m_pCurReleasePool = pPool;
    
    m_pReleasePoolStack->pushBack(pPool);                   //ref = 2
    
    pPool->release();                                       //ref = 1
}

void CAPoolManager::pop()
{
    CC_RETURN_IF(! m_pCurReleasePool);
    
    m_pCurReleasePool->clear();
    m_pReleasePoolStack->popBack();
    
    m_pCurReleasePool = m_pReleasePoolStack->back();
    
    /*m_pCurReleasePool = NULL;*/
}

void CAPoolManager::removeObject(CAObject* pObject)
{
    CCAssert(m_pCurReleasePool, "current auto release pool should not be null");
    
    m_pCurReleasePool->removeObject(pObject);
}

void CAPoolManager::addObject(CAObject* pObject)
{
    getCurReleasePool()->addObject(pObject);
}


CAAutoreleasePool* CAPoolManager::getCurReleasePool()
{
    if(!m_pCurReleasePool)
    {
        push();
    }
    
    CCAssert(m_pCurReleasePool, "current auto release pool should not be null");
    
    return m_pCurReleasePool;
}

NS_CC_END
