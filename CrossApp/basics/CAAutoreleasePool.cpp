
#include "CAAutoreleasePool.h"
#include "ccMacros.h"

NS_CC_BEGIN

static CAPoolManager* s_pPoolManager = NULL;

CAAutoreleasePool::CAAutoreleasePool(void)
{
    m_pManagedObjectArray = new CCArray();
    m_pManagedObjectArray->init();
}

CAAutoreleasePool::~CAAutoreleasePool(void)
{
    CC_SAFE_DELETE(m_pManagedObjectArray);
}

void CAAutoreleasePool::addObject(CAObject* pObject)
{
    m_pManagedObjectArray->addObject(pObject);

    CCAssert(pObject->m_uReference > 1, "reference count should be greater than 1");
    ++(pObject->m_uAutoReleaseCount);
    pObject->release(); // no ref count, in this case autorelease pool added.
}

void CAAutoreleasePool::removeObject(CAObject* pObject)
{
    for (unsigned int i = 0; i < pObject->m_uAutoReleaseCount; ++i)
    {
        m_pManagedObjectArray->removeObject(pObject, false);
    }
}

void CAAutoreleasePool::clear()
{
    if(m_pManagedObjectArray->count() > 0)
    {
        //CAAutoreleasePool* pReleasePool;
#ifdef _DEBUG
        int nIndex = m_pManagedObjectArray->count() - 1;
#endif

        CAObject* pObj = NULL;
        CCARRAY_FOREACH_REVERSE(m_pManagedObjectArray, pObj)
        {
            if(!pObj)
                break;

            --(pObj->m_uAutoReleaseCount);
            //(*it)->release();
            //delete (*it);
#ifdef _DEBUG
            nIndex--;
#endif
        }

        m_pManagedObjectArray->removeAllObjects();
    }
}


//--------------------------------------------------------------------
//
// CAPoolManager
//
//--------------------------------------------------------------------

CAPoolManager* CAPoolManager::sharedPoolManager()
{
    if (s_pPoolManager == NULL)
    {
        s_pPoolManager = new CAPoolManager();
    }
    return s_pPoolManager;
}

void CAPoolManager::purgePoolManager()
{
    CC_SAFE_DELETE(s_pPoolManager);
}

CAPoolManager::CAPoolManager()
{
    m_pReleasePoolStack = new CCArray();    
    m_pReleasePoolStack->init();
    m_pCurReleasePool = 0;
}

CAPoolManager::~CAPoolManager()
{
    
     finalize();
 
     // we only release the last autorelease pool here 
    m_pCurReleasePool = 0;
     m_pReleasePoolStack->removeObjectAtIndex(0);
 
     CC_SAFE_DELETE(m_pReleasePoolStack);
}

void CAPoolManager::finalize()
{
    if(m_pReleasePoolStack->count() > 0)
    {
        //CAAutoreleasePool* pReleasePool;
        CAObject* pObj = NULL;
        CCARRAY_FOREACH(m_pReleasePoolStack, pObj)
        {
            if(!pObj)
                break;
            CAAutoreleasePool* pPool = (CAAutoreleasePool*)pObj;
            pPool->clear();
        }
    }
}

void CAPoolManager::push()
{
    CAAutoreleasePool* pPool = new CAAutoreleasePool();       //ref = 1
    m_pCurReleasePool = pPool;

    m_pReleasePoolStack->addObject(pPool);                   //ref = 2

    pPool->release();                                       //ref = 1
}

void CAPoolManager::pop()
{
    CC_RETURN_IF(! m_pCurReleasePool);
    
    int nCount = m_pReleasePoolStack->count();

    m_pCurReleasePool->clear();
 
    if(nCount > 1)
    {
        m_pReleasePoolStack->removeObjectAtIndex(nCount-1);
        m_pCurReleasePool = (CAAutoreleasePool*)m_pReleasePoolStack->objectAtIndex(nCount - 2);
    }

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
