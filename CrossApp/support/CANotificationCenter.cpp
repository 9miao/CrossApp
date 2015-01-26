

#include "CANotificationCenter.h"
#include "cocoa/CCArray.h"
#include <string>

using namespace std;

NS_CC_BEGIN

static CANotificationCenter *s_sharedNotifCenter = NULL;

CANotificationCenter::CANotificationCenter()
: m_scriptHandler(0)
{
    m_observers = CCArray::createWithCapacity(3);
    m_observers->retain();
}

CANotificationCenter::~CANotificationCenter()
{
    m_observers->release();
}

CANotificationCenter *CANotificationCenter::sharedNotificationCenter(void)
{
    if (!s_sharedNotifCenter)
    {
        s_sharedNotifCenter = new CANotificationCenter;
    }
    return s_sharedNotifCenter;
}

void CANotificationCenter::purgeNotificationCenter(void)
{
    CC_SAFE_RELEASE_NULL(s_sharedNotifCenter);
}

//
// internal functions
//
bool CANotificationCenter::observerExisted(CAObject *target,const char *name)
{
    CAObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
            return true;
    }
    return false;
}

//
// observer functions
//
void CANotificationCenter::addObserver(CAObject *target, SEL_CallFuncO selector, const char *name, CAObject *obj)
{
    if (this->observerExisted(target, name))
        return;
    
    CCNotificationObserver *observer = new CCNotificationObserver(target, selector, name, obj);
    if (!observer)
        return;
    
    observer->autorelease();
    m_observers->addObject(observer);
}

void CANotificationCenter::removeObserver(CAObject *target, const char *name)
{
    CAObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_observers->removeObject(observer);
            return;
        }
    }
}

int CANotificationCenter::removeAllObservers(CAObject *target)
{
    CAObject *obj = NULL;
    CCArray *toRemove = CCArray::create();

    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver *observer = (CCNotificationObserver *)obj;
        if (!observer)
            continue;

        if (observer->getTarget() == target)
        {
            toRemove->addObject(observer);
        }
    }

    m_observers->removeObjectsInArray(toRemove);
    return toRemove->count();
}

void CANotificationCenter::registerScriptObserver( CAObject *target, int handler,const char* name)
{
    
    if (this->observerExisted(target, name))
        return;
    
    CCNotificationObserver *observer = new CCNotificationObserver(target, NULL, name, NULL);
    if (!observer)
        return;
    
    observer->setHandler(handler);
    observer->autorelease();
    m_observers->addObject(observer);
}

void CANotificationCenter::unregisterScriptObserver(CAObject *target,const char* name)
{        
    CAObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (!observer)
            continue;
            
        if ( !strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_observers->removeObject(observer);
        }
    }
}

void CANotificationCenter::postNotification(const char *name, CAObject *object)
{
    CCArray* ObserversCopy = CCArray::createWithCapacity(m_observers->count());
    ObserversCopy->addObjectsFromArray(m_observers);
    CAObject* obj = NULL;
    CCARRAY_FOREACH(ObserversCopy, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(name,observer->getName()) && (observer->getObject() == object || observer->getObject() == NULL || object == NULL))
        {
            if (0 == observer->getHandler())
            {
                observer->performSelector(object);
            }
        }
    }
}

void CANotificationCenter::postNotification(const char *name)
{
    this->postNotification(name,NULL);
}

int CANotificationCenter::getObserverHandlerByName(const char* name)
{
    if (NULL == name || strlen(name) == 0)
    {
        return -1;
    }
    
    CAObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (NULL == observer)
            continue;
        
        if ( 0 == strcmp(observer->getName(),name) )
        {
            return observer->getHandler();
            break;
        }
    }
    
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
///
/// CCNotificationObserver
///
////////////////////////////////////////////////////////////////////////////////
CCNotificationObserver::CCNotificationObserver(CAObject *target, SEL_CallFuncO selector, const char *name, CAObject *obj)
{
    m_target = target;
    m_selector = selector;
    m_object = obj;
    
    m_name = new char[strlen(name)+1];
    memset(m_name,0,strlen(name)+1);
    
    string orig (name);
    orig.copy(m_name,strlen(name),0);
    m_nHandler = 0;
}

CCNotificationObserver::~CCNotificationObserver()
{
    CC_SAFE_DELETE_ARRAY(m_name);
}

void CCNotificationObserver::performSelector(CAObject *obj)
{
    if (m_target)
    {
		if (obj)
        {
			(m_target->*m_selector)(obj);
		}
        else
        {
			(m_target->*m_selector)(m_object);
		}
    }
}

CAObject *CCNotificationObserver::getTarget()
{
    return m_target;
}

SEL_CallFuncO CCNotificationObserver::getSelector()
{
    return m_selector;
}

char *CCNotificationObserver::getName()
{
    return m_name;
}

CAObject *CCNotificationObserver::getObject()
{
    return m_object;
}

int CCNotificationObserver::getHandler()
{
    return m_nHandler;
}

void CCNotificationObserver::setHandler(int var)
{
    m_nHandler = var;
}

NS_CC_END
