

#include "script_support/CAScriptSupport.h"
#include "basics/CAScheduler.h"

NS_CC_BEGIN

CAScriptHandlerEntry* CAScriptHandlerEntry::create(int nHandler)
{
    CAScriptHandlerEntry* entry = new CAScriptHandlerEntry(nHandler);
    entry->autorelease();
    return entry;
}

CAScriptHandlerEntry::~CAScriptHandlerEntry(void)
{
	if (m_nHandler != 0)
	{
        CAScriptEngineManager::getScriptEngineManager()->getScriptEngine()->removeScriptHandler(m_nHandler);
        m_nHandler = 0;
    }
}

CATouchScriptHandlerEntry* CATouchScriptHandlerEntry::create(int nHandler,
                                                             bool bIsMultiTouches,
                                                             int nPriority,
                                                             bool bSwallowsTouches)
{
    CATouchScriptHandlerEntry* pEntry = new CATouchScriptHandlerEntry(nHandler);
    pEntry->init(bIsMultiTouches, nPriority, bSwallowsTouches);
    pEntry->autorelease();
    return pEntry;
}

CATouchScriptHandlerEntry::~CATouchScriptHandlerEntry(void)
{
    if (m_nHandler != 0)
    {
        CAScriptEngineManager::getScriptEngineManager()->getScriptEngine()->removeScriptHandler(m_nHandler);
        m_nHandler = 0;
    }
}

bool CATouchScriptHandlerEntry::init(bool bIsMultiTouches, int nPriority, bool bSwallowsTouches)
{
    m_bIsMultiTouches = bIsMultiTouches;
    m_nPriority = nPriority;
    m_bSwallowsTouches = bSwallowsTouches;
    
    return true;
}

static CAScriptEngineManager* s_pSharedScriptEngineManager = nullptr;

CAScriptEngineManager* CAScriptEngineManager::createScriptEngineManager(void)
{
    if (!s_pSharedScriptEngineManager)
    {
        s_pSharedScriptEngineManager = new CAScriptEngineManager();
    }
    return s_pSharedScriptEngineManager;
}

CAScriptEngineManager::~CAScriptEngineManager(void)
{
    removeScriptEngine();
}

void CAScriptEngineManager::setScriptEngine(CAScriptEngineProtocol *pScriptEngine)
{
    removeScriptEngine();
    m_pScriptEngine = pScriptEngine;
}

void CAScriptEngineManager::removeScriptEngine(void)
{
    if (m_pScriptEngine)
    {
        m_pScriptEngine = NULL;
    }
}



CAScriptEngineManager* CAScriptEngineManager::getScriptEngineManager(void)
{
    return s_pSharedScriptEngineManager;
}

void CAScriptEngineManager::purgeSharedManager(void)
{
    if (s_pSharedScriptEngineManager)
    {
        delete s_pSharedScriptEngineManager;
        s_pSharedScriptEngineManager = NULL;
    }
}

bool CAScriptEngineManager::sendViewControllerEventToJS(CAViewController* node, int action)
{
    
    auto scriptEngine = getScriptEngineManager()->getScriptEngine();
    
    if (scriptEngine->isCalledFromScript())
    {
        // Should only be invoked at root class Node
        scriptEngine->setCalledFromScript(false);
    }
    else
    {
        BasicScriptData data(node,(void*)&action);
        ScriptEvent scriptEvent(kViewControllerEvent,(void*)&data);
        if (scriptEngine->sendEvent(&scriptEvent))
        return true;
    }
    
    return false;
     
    
//    auto scriptEngine = getScriptEngineManager()->getScriptEngine();
//    BasicScriptData data(node,(void*)&action);
//    ScriptEvent scriptEvent(kViewControllerEvent,(void*)&data);
//    if (scriptEngine->sendEvent(&scriptEvent))
//    return true;
}


NS_CC_END
