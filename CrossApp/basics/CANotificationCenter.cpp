

#include "CANotificationCenter.h"
#include "CAValue.h"
#include "basics/CAApplication.h"
#include "support/Json/lib_json/json_lib.h"
NS_CC_BEGIN

CANotificationCenter::CANotificationCenter()
: m_scriptHandler(0)
{

}

CANotificationCenter::~CANotificationCenter()
{
    m_observers.clear();
}

CANotificationCenter *CANotificationCenter::getInstance(void)
{
    return CAApplication::getApplication()->getNotificationCenter();
}

bool CANotificationCenter::observerExisted(CAObject *target, const std::string& name)
{
    if (m_observers.find(target) != m_observers.end())
    {
        for (auto & observer : m_observers.at(target))
        {
            CC_CONTINUE_IF(!observer);
            
            if (observer->name == name)
                return true;
        }
    }

    return false;
}

void CANotificationCenter::insertList(CAObject *target)
{
    if (m_observers.find(target) == m_observers.end())
    {
        m_observers.insert(std::make_pair(target, CAList<Observer*>()));
    }
}

void CANotificationCenter::addObserver(const CANotificationCenter::Callback& callback, CAObject *target, const std::string& name)
{
    CC_RETURN_IF(this->observerExisted(target, name));

    Observer *observer = new Observer(callback, target, name);
    if (observer)
    {
        this->insertList(target);
        m_observers.at(target).pushBack(observer);
        observer->release();
    }
}

void CANotificationCenter::removeObserver(CAObject *target, const std::string& name)
{
    if (m_observers.find(target) != m_observers.end())
    {
        CAList<Observer*>& observers = m_observers.at(target);
        for (auto itr = observers.begin(); itr != observers.end();)
        {
            Observer* observer = *itr;
            CC_CONTINUE_IF(!observer);
            
            if (observer->name == name)
            {
                itr = observers.erase(itr);
                break;
            }
            else
            {
                ++itr;
            }
        }
    }
}

int CANotificationCenter::removeAllObservers(CAObject *target)
{
	int size = 0;
    
    if (m_observers.find(target) != m_observers.end())
    {
        size = (int)m_observers.size();
        m_observers.erase(target);
    }

	return size;
}

void CANotificationCenter::registerScriptObserver(CAObject *target, const std::string& name, int handler)
{
    
    CC_RETURN_IF(this->observerExisted(target, name));
    
    Observer *observer = new Observer(nullptr, target, name);
    if (observer)
    {
        this->insertList(target);
        observer->handler = handler;
        m_observers.at(target).pushBack(observer);
        observer->release();
    }
}

void CANotificationCenter::unregisterScriptObserver(CAObject *target, const std::string& name)
{        
    if (m_observers.find(target) != m_observers.end())
    {
        CAList<Observer*>& observers = m_observers.at(target);
        for (auto itr = observers.begin(); itr != observers.end();)
        {
            Observer* observer = *itr;
            CC_CONTINUE_IF(!observer);
            
            if (observer->name == name)
            {
                itr = observers.erase(itr);
                break;
            }
            else
            {
                ++itr;
            }
        }
    }
}

void CANotificationCenter::postNotification(const std::string& name, CAObject *object)
{
    std::map<CAObject*, CAList<Observer*>> observersCopy = std::map<CAObject*, CAList<Observer*>>(m_observers);
    
    for (auto pair : observersCopy)
    {
        const CAList<Observer*>& observers = pair.second;
        for (auto observer : observers)
        {
            if (observer->name == name && observer->handler == 0)
            {
                observer->performSelector(object);
                break;
            }
        }
    }
}

void CANotificationCenter::postNotification(const std::string& name)
{
    this->postNotification(name, nullptr);
}

void CANotificationCenter::postNotificationWithIntValue(const std::string& name, int value)
{
    CAValue v = CAValue(value);
    this->postNotification(name, &v);
}

void CANotificationCenter::postNotificationWithFloatValue(const std::string& name, float value)
{
    CAValue v = CAValue(value);
    this->postNotification(name, &v);
}

void CANotificationCenter::postNotificationWithDoubleValue(const std::string& name, double value)
{
    CAValue v = CAValue(value);
    this->postNotification(name, &v);
}

void CANotificationCenter::postNotificationWithStringValue(const std::string& name, const std::string& value)
{
    CAValue v = CAValue(value);
    this->postNotification(name, &v);
}
enum ValueType2
{
    nullValue = 0, ///< 'null' value
    intValue,      ///< signed integer value
    uintValue,     ///< unsigned integer value
    realValue,     ///< double value
    stringValue,   ///< UTF-8 string value
    booleanValue,  ///< bool value
    arrayValue,    ///< array value (ordered list)
    objectValue    ///< object value (collection of name/value pairs).
};

CAValue jsonvalue_to_cavalue(const CSJson::Value& json_value)
{
    CAValue value;
    
    switch (json_value.type())
    {
        case CSJson::ValueType::intValue:
        case CSJson::ValueType::uintValue:
            value = CAValue(json_value.asInt());
            break;
        case CSJson::ValueType::realValue:
            value = CAValue(json_value.asDouble());
            break;
        case CSJson::ValueType::stringValue:
            value = CAValue(json_value.asString());
            break;
        case CSJson::ValueType::booleanValue:
            value = CAValue(json_value.asBool());
            break;
        case CSJson::ValueType::arrayValue:
        {
            CrossApp::CAValueVector value_vector;
            unsigned int length = json_value.size();
            for(unsigned int i=0; i<length; i++)
            {
                value_vector.push_back(jsonvalue_to_cavalue(json_value[i]));
            }
            value = CAValue(value_vector);
        }
            break;
        case CSJson::ValueType::objectValue:
        {
            CrossApp::CAValueMap value_map;
            CSJson::Value::Members members = json_value.getMemberNames();
            for(auto& key : members)
            {
                value_map[key] = jsonvalue_to_cavalue(json_value[key]);
            }
            value = CAValue(value_map);
        }
            break;
        case CSJson::ValueType::nullValue:
            break;
        default:
            break;
    }
    
    return value;
}

void CANotificationCenter::postNotificationWithJsonStringValue(const std::string& name, const std::string& value)
{
    CSJson::Reader read;
    CSJson::Value root;
    bool succ = read.parse(value, root);
    if (succ == false)
    {
        CCLog("GetParseError \n");
    }
    
    CAValue v = jsonvalue_to_cavalue(root);
    this->postNotification(name, &v);
}

int CANotificationCenter::getObserverHandlerByName(const std::string& name)
{
    if (name.empty())
    {
        return -1;
    }
    
    for (auto pair : m_observers)
    {
        const CAList<Observer*>& observers = pair.second;
        for (auto observer : observers)
        {
            if (observer->name == name)
            {
                return observer->handler;
                break;
            }
        }
    }

    return -1;
}


CANotificationCenter::Observer::Observer(const CANotificationCenter::Callback& callback, CAObject *target, const std::string& name)
:name(name)
,target(target)
,callback(callback)
{
    
}

void CANotificationCenter::Observer::performSelector(CAObject *obj)
{
    if (callback)
    {
        callback(obj);
    }
}

NS_CC_END
