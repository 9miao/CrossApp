#ifndef __SCRIPT_SUPPORT_H__
#define __SCRIPT_SUPPORT_H__

#include "platform/CACommon.h"
#include "cocoa/CCSet.h"
#include <map>
#include <string>
#include <list>
#include "controller/CAViewController.h"
typedef struct lua_State lua_State;

NS_CC_BEGIN

class CANotificationCenter;

enum ccScriptType {
    kScriptTypeNone = 0,
    kScriptTypeLua,
    kScriptTypeJavascript
};

class CC_DLL CAScriptHandlerEntry : public CAObject
{
public:
    static CAScriptHandlerEntry* create(int nHandler);
    ~CAScriptHandlerEntry(void);
    
    int getHandler(void) {
        return m_nHandler;
    }
    
    int getEntryId(void) {
        return m_nEntryId;
    }
    
protected:
    CAScriptHandlerEntry(int nHandler)
    : m_nHandler(nHandler)
    {
        static int newEntryId = 0;
        newEntryId++;
        m_nEntryId = newEntryId;
    }
    
    int m_nHandler;
    int m_nEntryId;
};

class CATouchScriptHandlerEntry : public CAScriptHandlerEntry
{
public:
    static CATouchScriptHandlerEntry* create(int nHandler, bool bIsMultiTouches, int nPriority, bool bSwallowsTouches);
    ~CATouchScriptHandlerEntry(void);
    
    bool isMultiTouches(void) {
        return m_bIsMultiTouches;
    }
    
    int getPriority(void) {
        return m_nPriority;
    }
    
    bool getSwallowsTouches(void) {
        return m_bSwallowsTouches;
    }
    
private:
    CATouchScriptHandlerEntry(int nHandler)
    : CAScriptHandlerEntry(nHandler)
    , m_bIsMultiTouches(false)
    , m_nPriority(0)
    , m_bSwallowsTouches(false)
    {
    }
    bool init(bool bIsMultiTouches, int nPriority, bool bSwallowsTouches);
    
    bool    m_bIsMultiTouches;
    int     m_nPriority;
    bool    m_bSwallowsTouches;
};

enum ScriptEventType
{
    kRestartGame,
    kViewControllerEvent
};

struct BasicScriptData
{

    void* nativeObject;

    void* value;

    BasicScriptData(void* inObject,void* inValue = nullptr)
    : nativeObject(inObject),value(inValue)
    {
    }
};

struct ScriptEvent
{

    ScriptEventType type;

    void* data;

    ScriptEvent(ScriptEventType inType,void* inData)
    : type(inType),
    data(inData)
    {
    }
};

class CC_DLL CAScriptEngineProtocol
{
public:
    virtual ~CAScriptEngineProtocol() {};
    
    virtual ccScriptType getScriptType() { return kScriptTypeNone; };
    
    virtual void removeScriptObjectByObject(CAObject* pObj) = 0;
    
    virtual void removeScriptHandler(int nHandler) {};
    
    virtual int reallocateScriptHandler(int nHandler) { return -1;}

    virtual int executeString(const char* codes) = 0;

    virtual int executeScriptFile(const char* filename) = 0;

    virtual int executeGlobalFunction(const char* functionName) = 0;

    virtual int sendEvent(ScriptEvent* evt) = 0;

    virtual bool handleAssert(const char *msg) = 0;

    virtual void setCalledFromScript(bool callFromScript) { CC_UNUSED_PARAM(callFromScript); };

    virtual bool isCalledFromScript() { return false; };

    virtual void releaseScriptObject(CrossApp::CAObject* owner, CrossApp::CAObject* target) = 0;

    virtual void releaseAllSubviewsRecursive(CrossApp::CAView* view) = 0;
};

class CC_DLL CAScriptEngineManager
{
public:
    ~CAScriptEngineManager(void);
    
    static CAScriptEngineManager* getScriptEngineManager(void);
    
    static CAScriptEngineManager* createScriptEngineManager(void);
    
    static void purgeSharedManager(void);

    static bool sendViewControllerEventToJS(CAViewController* node, int action);
    
    CAScriptEngineProtocol* getScriptEngine(void)
    {
        return m_pScriptEngine;
    }
    void setScriptEngine(CAScriptEngineProtocol *pScriptEngine);
    void removeScriptEngine(void);
    
    
private:
    CAScriptEngineManager(void)
    : m_pScriptEngine(nullptr)
    {
    }
    
    CAScriptEngineProtocol *m_pScriptEngine;
};

NS_CC_END

#endif // __SCRIPT_SUPPORT_H__
