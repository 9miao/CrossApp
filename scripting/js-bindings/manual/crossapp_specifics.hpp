
#ifndef __JS_SPECIFICS_H__
#define __JS_SPECIFICS_H__

#include "ScriptingCore.h"
#include "platform/CASAXParser.h"
#include <assert.h>

/**
 * You don't need to manage the returned pointer. They live for the whole life of
 * the app.
 */
template <class T>
inline js_type_class_t *js_get_type_from_native(T* native_obj) {
    bool found = false;
    std::string typeName = typeid(*native_obj).name();
    auto typeProxyIter = _js_global_type_map.find(typeName);
    if (typeProxyIter == _js_global_type_map.end())
    {
        typeName = typeid(T).name();
        typeProxyIter = _js_global_type_map.find(typeName);
        if (typeProxyIter != _js_global_type_map.end())
        {
            found = true;
        }
    }
    else
    {
        found = true;
    }
    return found ? typeProxyIter->second : nullptr;
}

/**
 * The returned pointer should be deleted using jsb_remove_proxy. Most of the
 * time you do that in the C++ destructor.
 */
template<class T>
inline js_proxy_t *js_get_or_create_proxy(JSContext *cx, T *native_obj) {
    js_proxy_t *proxy;
    HASH_FIND_PTR(_native_js_global_ht, &native_obj, proxy);
    if (!proxy) {
        js_type_class_t *typeProxy = js_get_type_from_native<T>(native_obj);
        // Return NULL if can't find its type rather than making an assert.
        assert(typeProxy);
        if (!typeProxy) {
            return NULL;
        }
        
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

        JS::RootedObject proto(cx, const_cast<JSObject*>(typeProxy->proto.get()));
        JS::RootedObject parent(cx, const_cast<JSObject*>(typeProxy->parentProto.get()));
        JS::RootedObject js_obj(cx, JS_NewObject(cx, typeProxy->jsclass, proto, parent));
        proxy = jsb_new_proxy(native_obj, js_obj);
        AddObjectRoot(cx, &proxy->obj);
        return proxy;
    } else {
        return proxy;
    }
    return NULL;
}

JS::Value anonEvaluate(JSContext *cx, JS::HandleObject thisObj, const char* string);
void get_or_create_js_obj(JSContext* cx, JS::HandleObject obj, const std::string &name, JS::MutableHandleObject jsObj);

void register_crossapp_js_core(JSContext* cx, JS::HandleObject obj);

bool js_crossapp_ca_localStorageInit(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_ca_localStorageFree(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_ca_localStorageSetItem(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_ca_localStorageGetItem(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_ca_localStorageRemoveItem(JSContext *cx, uint32_t argc, jsval *vp);

bool js_crossapp_retain(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_release(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_CAMotionManager_startGyroscope(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_CADatePickerView_onSelectRow(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_CACustomAnimation_schedule(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_CANotificationCenter_addObserver(JSContext *cx, uint32_t argc, jsval *vp);
bool js_crossapp_CAAddressBook_getAddressBook(JSContext *cx, uint32_t argc, jsval *vp);
#endif
