

#ifndef __CANotificationCenter_H__
#define __CANotificationCenter_H__

#include "basics/CAObject.h"
#include "basics/CASTLContainer.h"
#include <functional>
NS_CC_BEGIN

class CC_DLL CANotificationCenter : public CAObject
{
public:

    typedef std::function<void(CAObject*)> Callback;
    
    struct CC_DLL Observer : public CAObject
    {
        Observer(const CANotificationCenter::Callback& callback, CAObject *target, const std::string& name);

        void performSelector(CAObject *obj);
        
        std::string name;
        CAObject *target;
        CANotificationCenter::Callback callback;
        int handler{0};
    };
    
    
public:
    
    CANotificationCenter();

    ~CANotificationCenter();
    
    static CANotificationCenter *getInstance();

    void addObserver(const CANotificationCenter::Callback& callback, CAObject *target, const std::string& name);

    void removeObserver(CAObject *target, const std::string& name);

    int removeAllObservers(CAObject *target);

    void registerScriptObserver(CAObject *target, const std::string& name, int handler);

    void unregisterScriptObserver(CAObject *target, const std::string& name);

    void postNotification(const std::string& name);

    void postNotification(const std::string& name, CAObject *object);

    void postNotificationWithIntValue(const std::string& name, int value);
    void postNotificationWithFloatValue(const std::string& name, float value);
    void postNotificationWithDoubleValue(const std::string& name, double value);
    void postNotificationWithStringValue(const std::string& name, const std::string& value);
    void postNotificationWithJsonStringValue(const std::string& name, const std::string& value);
    
    inline int getScriptHandler() { return m_scriptHandler; };

    int getObserverHandlerByName(const std::string& name);
    
private:

    bool observerExisted(CAObject *target, const std::string& name);

    void insertList(CAObject *target);
    
    std::map<CAObject*, CAList<Observer*>> m_observers;

    int m_scriptHandler;
};



NS_CC_END

#endif//__CANotificationCenter_H__
