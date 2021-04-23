//
//  ScriptingCore.hpp
//  MyTest
//
//  Created by Lei.zhang on 16/1/19.
//  Copyright © 2016年 Lei.zhang All rights reserved.
//

#ifndef ScriptingCore_hpp
#define ScriptingCore_hpp

#include "jsapi.h"
#include "jsfriendapi.h"
#include "CrossApp.h"
#include "js_bindings_config.h"
#include "js_bindings_core.h"
#include "spidermonkey_specifics.h"
#include "js_manual_conversions.h"
#include "mozilla/Maybe.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

void js_log(const char *format, ...);

typedef void (*sc_register_sth)(JSContext* cx, JS::HandleObject global);

void registerDefaultClasses(JSContext* cx, JS::HandleObject global);

class SimpleRunLoop : public CrossApp::CAObject
{
public:
    void update(float d);
};

class ScriptingCore : public CAScriptEngineProtocol
{
private:
    JSRuntime *_rt;
    JSContext *_cx;
    JS::PersistentRootedObject *_global;
    JS::PersistentRootedObject *_debugGlobal;
    SimpleRunLoop* _runLoop;
    bool _jsInited;
    bool _needCleanup;
    bool _callFromScript;
    ScriptingCore();
public:
    ~ScriptingCore();

    static ScriptingCore* getInstance();

    virtual CrossApp::ccScriptType getScriptType() override { return CrossApp::kScriptTypeJavascript; };

    virtual void removeScriptObjectByObject(CAObject* obj) override;

    virtual int executeString(const char* codes) override { return 0; };

    virtual  int executeScriptFile(const char* filename) override { return 0; }

    virtual int executeGlobalFunction(const char* functionName) override { return 0; } //
    
    virtual int sendEvent(ScriptEvent* message) override;

    virtual bool handleAssert(const char *msg) override  { return false; }
    
    virtual void setCalledFromScript(bool callFromScript) override { _callFromScript = callFromScript; }; 
    virtual bool isCalledFromScript() override { return _callFromScript; };

    bool executeFunctionWithOwner(jsval owner, const char *name, uint32_t argc, jsval *vp);

    bool executeFunctionWithOwner(jsval owner, const char *name, uint32_t argc, jsval *vp, JS::MutableHandleValue retVal);

    bool executeFunctionWithOwner(jsval owner, const char *name, const JS::HandleValueArray& args);

    bool executeFunctionWithOwner(jsval owner, const char *name, const JS::HandleValueArray& args, JS::MutableHandleValue retVal);
    
    bool evalString(const char *string);
    
    bool evalString(const char *string, JS::RootedValue *outVal, const char *filename = NULL, JSContext *cx = NULL, JSObject* global = NULL);

    JSScript* getScript(const char *path);

    void compileScript(const char *path, JSObject* global = NULL, JSContext* cx = NULL);

    bool runScript(const char *path);

    bool runScript(const char *path, JS::HandleObject global, JSContext* cx = NULL);

    bool requireScript(const char *path, JS::MutableHandleValue jsvalRet);

    bool requireScript(const char *path, JS::HandleObject global, JSContext* cx, JS::MutableHandleValue jsvalRet);

    void cleanScript(const char *path);

    std::unordered_map<std::string, JSScript*> &getFileScript();

    void cleanAllScript();

    void start();

    void cleanup();

    void reset();

    void addRegisterCallback(sc_register_sth callback);

    void createGlobalContext();

    static void removeAllRoots(JSContext *cx);

    int executeCustomTouchEvent(int eventType,CATouch *pTouch, JSObject *obj, JS::MutableHandleValue retval);
 
    int executeCustomTouchEvent(int eventType,CATouch *pTouch, JSObject *obj);

    int executeCustomTouchesEvent(int eventType,const std::vector<CATouch*>& touches, JSObject *obj);
    
    JSContext* getGlobalContext() {
        return _cx;
    };

    void setEnabledReportError(bool var);
    
    static void reportError(JSContext *cx, const char *message, JSErrorReport *report);

    static bool log(JSContext *cx, uint32_t argc, jsval *vp);

    bool setReservedSpot(uint32_t i, JSObject *obj, jsval value);

    static bool executeScript(JSContext *cx, uint32_t argc, jsval *vp);

    static bool forceGC(JSContext *cx, uint32_t argc, jsval *vp);

    static bool dumpRoot(JSContext *cx, uint32_t argc, jsval *vp);

    static bool addRootJS(JSContext *cx, uint32_t argc, jsval *vp);

    static bool removeRootJS(JSContext *cx, uint32_t argc, jsval *vp);

    static bool isObjectValid(JSContext *cx, uint32_t argc, jsval *vp);

    void debugProcessInput(const std::string& str);

    void enableDebugger(unsigned int port = 5086);

    JSObject* getDebugGlobal() { return _debugGlobal->get(); }

    JSObject* getGlobalObject() { return _global->get(); }

    bool isFunctionOverridedInJS(JS::HandleObject obj, const std::string& name, JSNative native);

    virtual void releaseScriptObject(CrossApp::CAObject* owner, CrossApp::CAObject* target) override;

    virtual void releaseAllSubviewsRecursive(CrossApp::CAView* view) override;

    virtual void garbageCollect();
    
    void restartVM();
    
private:
    void initRegister();
public:

    int handleViewControllerEvent(void* data);
    
    bool _enabledReportError;
};


JSObject* NewGlobalObject(JSContext* cx, bool debug = false);

js_proxy_t* jsb_new_proxy(void* nativeObj, JSObject* jsObj);
js_proxy_t* jsb_get_native_proxy(void* nativeObj);
js_proxy_t* jsb_get_js_proxy(JSObject* jsObj);
void jsb_remove_proxy(js_proxy_t* nativeProxy, js_proxy_t* jsProxy);
void jsb_remove_proxy(js_proxy_t* nativeProxy);
template <class T>
jsval getJSObject(JSContext* cx, T* nativeObj)
{
    js_proxy_t *proxy = js_get_or_create_proxy<T>(cx, nativeObj);
    return proxy ? OBJECT_TO_JSVAL(proxy->obj) : JSVAL_NULL;
}

void removeJSObject(JSContext* cx, void* nativeObj);


#endif /* ScriptingCore_hpp */
