
#ifndef __JS_BINDINGS_CORE_H__
#define __JS_BINDINGS_CORE_H__

#include "jsapi.h"
#include "jsfriendapi.h"

#ifdef __cplusplus
extern "C" {
#endif

    enum {
        JSB_C_FLAG_CALL_FREE = 0,
        JSB_C_FLAG_DO_NOT_CALL_FREE =1,
    };

    // structure used by "Object Oriented Functions".
    // handle is a pointer to the native object
    // flags: flags for the object
    struct jsb_c_proxy_s {
        unsigned long flags;    // Should it be removed at "destructor" time, or not ?
        void *handle;           // native object, like cpSpace, cpBody, etc.
        JS::Heap<JSObject*> jsobj;        // JS Object. Needed for rooting / unrooting
    };
    
    // Functions for setting / removing / getting the proxy used by the "C" Object Oriented API. Think of Chipmunk classes
    struct jsb_c_proxy_s* jsb_get_c_proxy_for_jsobject( JSObject *jsobj );
    void jsb_del_c_proxy_for_jsobject( JSObject *jsobj );
    void jsb_set_c_proxy_for_jsobject( JSObject *jsobj, void *handle, unsigned long flags);

    // JSObject -> proxy
    /** gets a proxy for a given JSObject */
    void* jsb_get_proxy_for_jsobject(JSObject *jsobj);
    /** sets a proxy for a given JSObject */
    void jsb_set_proxy_for_jsobject(void* proxy, JSObject *jsobj);
    /** dels a proxy for a given JSObject */
    void jsb_del_proxy_for_jsobject(JSObject *jsobj);

    // reverse: proxy -> JSObject
    /** gets a JSObject for a given proxy */
    JSObject* jsb_get_jsobject_for_proxy(void *proxy);
    /** sets a JSObject for a given proxy */
    void jsb_set_jsobject_for_proxy(JSObject *jsobj, void* proxy);
    /** delts a JSObject for a given proxy */
    void jsb_del_jsobject_for_proxy(void* proxy);
    
    
    // needed for callbacks. It does nothing.
    bool JSB_do_nothing(JSContext *cx, uint32_t argc, jsval *vp);

#ifdef __cplusplus
}
#endif

#endif /* __JS_BINDINGS_CORE_H__ */
