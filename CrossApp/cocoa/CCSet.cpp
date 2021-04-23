

#include "CCSet.h"



NS_CC_BEGIN

CCSet::CCSet(void)
{
    m_pSet = new std::set<CAObject *>;
}

CCSet::CCSet(const CCSet &rSetObject)
{
    m_pSet = new std::set<CAObject *>(*rSetObject.m_pSet);

    // call retain of members
    CCSetIterator iter;
    for (iter = m_pSet->begin(); iter != m_pSet->end(); ++iter)
    {
        if (! (*iter))
        {
            break;
        }

        (*iter)->retain();
    }
}

CCSet::~CCSet(void)
{
    removeAllObjects();
    CC_SAFE_DELETE(m_pSet);
}

CCSet * CCSet::create()
{
    CCSet * pRet = new CCSet();
    
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    
    return pRet;
}

CCSet* CCSet::copy(void)
{
    CCSet *pSet = new CCSet(*this);

    return pSet;
}

CCSet* CCSet::mutableCopy(void)
{
    return copy();
}

int CCSet::count(void)
{
    return (int)m_pSet->size();
}

void CCSet::addObject(CAObject *pObject)
{
    if (m_pSet->count(pObject) == 0)
    {
        CC_SAFE_RETAIN(pObject);
        m_pSet->insert(pObject);
    }
}

void CCSet::removeObject(CAObject *pObject)
{
    if (m_pSet->erase(pObject) > 0)
    {
        CC_SAFE_RELEASE(pObject);
    }
}

void CCSet::removeAllObjects()
{
    CCSetIterator it;
    for (it = m_pSet->begin(); it != m_pSet->end(); ++it)
    {
        CC_SAFE_RELEASE((*it));
    }
    m_pSet->clear();
}

bool CCSet::containsObject(CAObject *pObject)
{
    return m_pSet->find(pObject) != m_pSet->end();
}

CCSetIterator CCSet::begin(void)
{
    return m_pSet->begin();
}

CCSetIterator CCSet::end(void)
{
    return m_pSet->end();
}

CAObject* CCSet::anyObject()
{
    if (!m_pSet || m_pSet->empty())
    {
        return 0;
    }
    
    CCSetIterator it;

    for( it = m_pSet->begin(); it != m_pSet->end(); ++it)
    {
        if (*it)
        {
            return (*it);
        }
    }

    return 0;
}

NS_CC_END
