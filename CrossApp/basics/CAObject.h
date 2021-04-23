

#ifndef __CAObject_H__
#define __CAObject_H__

#include "CASyncQueue.h"
#include "platform/CCPlatformMacros.h"
#include "float.h"
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>

NS_CC_BEGIN

class CAObject;
class CAEvent;
class CAView;
class CAObject;

typedef void (CAObject::*SEL_CallFunc)();
typedef void (CAObject::*SEL_CallFuncO)(CAObject*);
typedef void (CAObject::*SEL_Schedule)(float);

class CC_DLL CAClonable
{
public:
    virtual CAClonable* clone() const = 0;
};

class CC_DLL CAObject
{
public:

    unsigned int        m_u__ID;

    std::string         m_s__StrID;
    
protected:

    unsigned int        m_uReference;

    unsigned int        m_uAutoReleaseCount;

public:
    
    CAObject(void);

    virtual ~CAObject(void);
    
    bool init() { return true; }
    
    virtual void release(void);
    
	virtual CAObject* retain(void);
    
	virtual CAObject* autorelease(void);
    
    bool isSingleReference(void) const;
    
    unsigned int retainCount(void) const;
    
    virtual bool isEqual(const CAObject* pObject);

    virtual void update(float dt) {CC_UNUSED_PARAM(dt);};
    
    void performSelector(SEL_CallFunc callFunc, float afterDelay);
    
    void performSelector(SEL_CallFuncO callFunc, CAObject* objParam, float afterDelay);
    
    void cancelPreviousPerformRequests(SEL_CallFunc callFunc);
    
    void cancelPreviousPerformRequests(SEL_CallFuncO callFunc);
    
    CC_SYNTHESIZE(void*, m_pUserData, UserData);
    
    CC_SYNTHESIZE_RETAIN(CAObject*, m_pUserObject, UserObject);
    
    CC_SYNTHESIZE(int, m_nTag, Tag);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sTextTag, TextTag);
    
    unsigned int getUID(){ return m_u__ID; }
    
    const std::string& getStrID(){ return m_s__StrID; }

    friend class CAAutoreleasePool;
};

#define callfunc_selector(_SELECTOR) static_cast<CrossApp::SEL_CallFunc>(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) static_cast<CrossApp::SEL_CallFuncO>(&_SELECTOR)
#define schedule_selector(_SELECTOR) (CrossApp::SEL_Schedule)(&_SELECTOR)


const float FLOAT_NONE = 0XFFFFFFFF;
const int INT_NONE = 0x8FFFFFFF;
const unsigned int UINT_NONE = 0XFFFFFFFF;

enum {
    FLAGS_TRANSFORM_DIRTY = (1 << 0),
    FLAGS_CONTENT_SIZE_DIRTY = (1 << 1),
    FLAGS_RENDER_AS_3D = (1 << 3),
    
    FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
};


NS_CC_END

#endif // __CAObject_H__
