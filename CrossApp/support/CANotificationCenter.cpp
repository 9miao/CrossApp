

#include "CANotificationCenter.h"

using namespace std;

NS_CC_BEGIN

static CANotificationCenter *s_sharedNotifCenter = NULL;

CANotificationCenter::CANotificationCenter()
: m_scriptHandler(0)
{

}

CANotificationCenter::~CANotificationCenter()
{
    m_observers.clear();
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
    CADeque<CANotificationObserver*>::iterator itr;
    for (itr=m_observers.begin(); itr!=m_observers.end(); ++itr)
    {
        CANotificationObserver* observer = *itr;
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
    
    CANotificationObserver *observer = new CANotificationObserver(target, selector, name, obj);
    if (!observer)
        return;
    
    observer->autorelease();
    m_observers.pushBack(observer);
}

void CANotificationCenter::removeObserver(CAObject *target, const char *name)
{
    CADeque<CANotificationObserver*>::iterator itr;
    for (itr=m_observers.begin(); itr!=m_observers.end(); ++itr)
    {
        CANotificationObserver* observer = *itr;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_observers.erase(itr);
            return;
        }
    }
}

int CANotificationCenter::removeAllObservers(CAObject *target)
{
    CADeque<CANotificationObserver*> toRemove;

    CADeque<CANotificationObserver*>::iterator itr;
    for (itr=m_observers.begin(); itr!=m_observers.end(); ++itr)
    {
        CANotificationObserver* observer = *itr;
        if (!observer)
            continue;

        if (observer->getTarget() == target)
        {
            toRemove.pushBack(observer);
        }
    }

    m_observers.erase(toRemove.begin(), toRemove.end());
    return (int)toRemove.size();
}

void CANotificationCenter::registerScriptObserver( CAObject *target, int handler,const char* name)
{
    
    if (this->observerExisted(target, name))
        return;
    
    CANotificationObserver *observer = new CANotificationObserver(target, NULL, name, NULL);
    if (!observer)
        return;
    
    observer->setHandler(handler);
    observer->autorelease();
    m_observers.pushBack(observer);
}

void CANotificationCenter::unregisterScriptObserver(CAObject *target,const char* name)
{        
    CADeque<CANotificationObserver*>::iterator itr;
    for (itr=m_observers.begin(); itr!=m_observers.end(); ++itr)
    {
        CANotificationObserver* observer = *itr;
        if (!observer)
            continue;
            
        if ( !strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_observers.erase(itr);
        }
    }
}

void CANotificationCenter::postNotification(const char *name, CAObject *object)
{
    CADeque<CANotificationObserver*> ObserversCopy = m_observers;
    
    CADeque<CANotificationObserver*>::iterator itr;
    for (itr=ObserversCopy.begin(); itr!=ObserversCopy.end(); ++itr)
    {
        CANotificationObserver* observer = *itr;
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
    
    CADeque<CANotificationObserver*>::iterator itr;
    for (itr=m_observers.begin(); itr!=m_observers.end(); ++itr)
    {
        CANotificationObserver* observer = *itr;
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
/// CANotificationObserver
///
////////////////////////////////////////////////////////////////////////////////
CANotificationObserver::CANotificationObserver(CAObject *target, SEL_CallFuncO selector, const char *name, CAObject *obj)
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

CANotificationObserver::~CANotificationObserver()
{
    CC_SAFE_DELETE_ARRAY(m_name);
}

void CANotificationObserver::performSelector(CAObject *obj)
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

CAObject *CANotificationObserver::getTarget()
{
    return m_target;
}

SEL_CallFuncO CANotificationObserver::getSelector()
{
    return m_selector;
}

char *CANotificationObserver::getName()
{
    return m_name;
}

CAObject *CANotificationObserver::getObject()
{
    return m_object;
}

int CANotificationObserver::getHandler()
{
    return m_nHandler;
}

void CANotificationObserver::setHandler(int var)
{
    m_nHandler = var;
}

NS_CC_END
