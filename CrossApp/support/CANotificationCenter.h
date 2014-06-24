

#ifndef __CANotificationCenter_H__
#define __CANotificationCenter_H__

#include "basics/CAObject.h"
#include "cocoa/CCArray.h"

NS_CC_BEGIN
/**
 * @js NA
 */
class CC_DLL CANotificationCenter : public CAObject
{
public:
    /** CANotificationCenter constructor */
    CANotificationCenter();

    /** CANotificationCenter destructor */
    ~CANotificationCenter();
    
    /** Gets the single instance of CANotificationCenter. */
    static CANotificationCenter *sharedNotificationCenter(void);

    /** Destroys the single instance of CANotificationCenter. */
    static void purgeNotificationCenter(void);

    /** @brief Adds an observer for the specified target.
     *  @param target The target which wants to observe notification events.
     *  @param selector The callback function which will be invoked when the specified notification event was posted.
     *  @param name The name of this notification.
     *  @param obj The extra parameter which will be passed to the callback function.
     */
    void addObserver(CAObject *target, 
                     SEL_CallFuncO selector,
                     const char *name,
                     CAObject *obj);

    /** @brief Removes the observer by the specified target and name.
     *  @param target The target of this notification.
     *  @param name The name of this notification. 
     */
    void removeObserver(CAObject *target,const char *name);
    
    /** @brief Removes all notifications registered by this target
     *  @param target The target of this notification.
     *  @returns the number of observers removed
     */
    int removeAllObservers(CAObject *target);

    /** @brief Registers one hander for script binding.
     *  @note Only supports Lua Binding now.
     *  @param handler The lua handler.
     */
    void registerScriptObserver(CAObject *target,int handler,const char* name);

    /** Unregisters script observer */
    void unregisterScriptObserver(CAObject *target,const char* name);
    
    /** @brief Posts one notification event by name.
     *  @param name The name of this notification.
     */
    void postNotification(const char *name);

    /** @brief Posts one notification event by name.
     *  @param name The name of this notification.
     *  @param object The extra parameter.
     */
    void postNotification(const char *name, CAObject *object);
    
    /** @brief Gets script handler.
     *  @note Only supports Lua Binding now.
     *  @return The script handle.
     */
    inline int getScriptHandler() { return m_scriptHandler; };
    
    /** @brief Gets observer script handler.
     *  @param name The name of this notification.
     *  @return The observer script handle.
     */
    int getObserverHandlerByName(const char* name);
private:
    // internal functions

    // Check whether the observer exists by the specified target and name.
    bool observerExisted(CAObject *target,const char *name);
    
    // variables
    //
    CCArray *m_observers;
    int     m_scriptHandler;
};

/**
 * @js NA
 * @lua NA
 */
class CC_DLL CCNotificationObserver : public CAObject
{
public:
    /** @brief CCNotificationObserver constructor
     *  @param target The target which wants to observer notification events.
     *  @param selector The callback function which will be invoked when the specified notification event was posted.
     *  @param name The name of this notification.
     *  @param obj The extra parameter which will be passed to the callback function.
     */
    CCNotificationObserver(CAObject *target, 
                           SEL_CallFuncO selector,
                           const char *name,
                           CAObject *obj);

    /** CCNotificationObserver destructor function */
    ~CCNotificationObserver();      
    
    /** Invokes the callback function of this observer */
    void performSelector(CAObject *obj);
private:
    CC_PROPERTY_READONLY(CAObject *, m_target, Target);
    CC_PROPERTY_READONLY(SEL_CallFuncO, m_selector, Selector);
    CC_PROPERTY_READONLY(char *, m_name, Name);
    CC_PROPERTY_READONLY(CAObject *, m_object, Object);
    CC_PROPERTY(int, m_nHandler,Handler);
};

NS_CC_END

#endif//__CANotificationCenter_H__
