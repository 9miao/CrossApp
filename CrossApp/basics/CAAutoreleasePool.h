
#ifndef __AUTORELEASEPOOL_H__
#define __AUTORELEASEPOOL_H__

#include "CAObject.h"
#include "basics/CASTLContainer.h"

NS_CC_BEGIN

class CC_DLL CAAutoreleasePool : public CAObject
{
    std::vector<CAObject*>*    m_pManagedObjectArray;
public:
    CAAutoreleasePool(void);
    ~CAAutoreleasePool(void);
    
    void addObject(CAObject *pObject);
    void removeObject(CAObject *pObject);
    
    void clear();
};


class CC_DLL CAPoolManager
{
    CADeque<CAAutoreleasePool*>*     m_pReleasePoolStack;
    CAAutoreleasePool*                m_pCurReleasePool;
    
    CAAutoreleasePool* getCurReleasePool();
public:
    CAPoolManager();
    ~CAPoolManager();
    void finalize();
    void push();
    void pop();
    
    void removeObject(CAObject* pObject);
    void addObject(CAObject* pObject);
    
    static CAPoolManager* getInstance();
    static void destroyInstance();
    
    friend class CAAutoreleasePool;
};


NS_CC_END

#endif //__AUTORELEASEPOOL_H__
