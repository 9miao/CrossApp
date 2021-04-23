//
//  ScriptingCore.cpp
//  MyTest
//
//  Created by Lei.zhang on 16/1/19.
//  Copyright © 2016 Lei.zhang All rights reserved.
//

#include "ScriptingCore.h"

// Removed in Firefox v27, use 'js/OldDebugAPI.h' instead
#include "js/OldDebugAPI.h"

// for debug socket
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#include <io.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/JniHelper.h"
#include <jni.h>
#endif

#include <thread>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <mutex>
#include "crossapp_specifics.hpp"

#ifdef ANDROID
#define  LOG_TAG    "ScriptingCore.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...) js_log(__VA_ARGS__)
#endif

#if CrossApp_DEBUG
#define TRACE_DEBUGGER_SERVER(...) CCLOG(__VA_ARGS__)
#else
#define TRACE_DEBUGGER_SERVER(...)
#endif // #if DEBUG

#define BYTE_CODE_FILE_EXT ".cross"
static std::string inData;
static std::string outData;
static std::vector<std::string> g_queue;
static std::mutex g_qMutex;
static std::mutex g_rwMutex;
static int clientSocket = -1;
static uint32_t s_nestedLoopLevel = 0;

js_proxy_t *_native_js_global_ht = NULL;
js_proxy_t *_js_native_global_ht = NULL;

std::unordered_map<std::string, js_type_class_t*> _js_global_type_map;
static std::unordered_map<void*, js_proxy_t*> _native_js_global_map;
static std::unordered_map<JSObject*, js_proxy_t*> _js_native_global_map;
static std::unordered_map<JSObject*, JSObject*> _js_hook_owner_map;

static char *_js_log_buf = NULL;

static std::vector<sc_register_sth> registrationList;
// name ~> JSScript map
static std::unordered_map<std::string, JSScript*> filename_script;

/*
static void cc_closesocket(int fd)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    closesocket(fd);
#else
    close(fd);
#endif
}
*/
static ScriptingCore* instance = nullptr; 
typedef void (*sc_register_sth)(JSContext* cx, JS::HandleObject global);



/// The max length of CCLog message.
static const int MAX_LOG_LENGTH = 16*1024; 
void js_log(const char *format, ...) {
    if (_js_log_buf == NULL)
    {
        _js_log_buf = (char *)calloc(sizeof(char), MAX_LOG_LENGTH+1);
        _js_log_buf[MAX_LOG_LENGTH] = '\0';
    }
    va_list vl;
    va_start(vl, format);
    int len = vsnprintf(_js_log_buf, MAX_LOG_LENGTH, format, vl);
    va_end(vl);
    if (len > 0)
    {
        CCLog("JS: %s", _js_log_buf);
    }
}
static void sc_finalize(JSFreeOp *freeOp, JSObject *obj) {
//    CCLOGINFO("jsbindings: finalizing JS object %p (global class)", obj);
}

static void ReportException(JSContext *cx)
{
    if (JS_IsExceptionPending(cx)) {
        if (!JS_ReportPendingException(cx)) {
            JS_ClearPendingException(cx);
        }
    }
}


static const JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub,
    JS_DeletePropertyStub,
    JS_PropertyStub,
    JS_StrictPropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    sc_finalize,
    nullptr, nullptr, nullptr,
    JS_GlobalObjectTraceHook
};
// Just a wrapper around JSPrincipals that allows static construction.
class CCJSPrincipals : public JSPrincipals
{
public:
    explicit CCJSPrincipals(int rc = 0)
    : JSPrincipals()
    {
        refcount = rc;
    }
};
static CCJSPrincipals shellTrustedPrincipals(1);


ScriptingCore::ScriptingCore()
:_rt(nullptr)
,_cx(nullptr)
,_jsInited(false)
,_needCleanup(false)
,_global(nullptr)
,_debugGlobal(nullptr)
,_callFromScript(false)
,_enabledReportError(false)
{
    initRegister();
}
void ScriptingCore::initRegister(){
    this->addRegisterCallback(registerDefaultClasses);
    this->_runLoop = new (std::nothrow) SimpleRunLoop();
}

void ScriptingCore::restartVM()
{
    cleanup();
    initRegister();
    CCApplication::sharedApplication()->applicationDidFinishLaunching();
}

ScriptingCore::~ScriptingCore()
{
    cleanup();
}

bool JSBCore_os(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc!=0)
    {
        JS_ReportError(cx, "Invalid number of arguments in __getOS");
        return false;
    }
    
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    
    JSString * os;
    
    // osx, ios, android, windows, linux, etc..
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    os = JS_InternString(cx, "iOS");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    os = JS_InternString(cx, "Android");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    os = JS_InternString(cx, "Windows");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
    os = JS_InternString(cx, "Marmalade");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    os = JS_InternString(cx, "Linux");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_BADA)
    os = JS_InternString(cx, "Bada");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    os = JS_InternString(cx, "Blackberry");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    os = JS_InternString(cx, "OS X");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    os = JS_InternString(cx, "WINRT");
#else
    os = JS_InternString(cx, "Unknown");
#endif
    
    args.rval().set(STRING_TO_JSVAL(os));
    
    return true;
};
bool JSBCore_version(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc!=0)
    {
        JS_ReportError(cx, "Invalid number of arguments in __getVersion");
        return false;
    }
    
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    char version[256];
    snprintf(version, sizeof(version)-1, "%s", CrossAppVersion());
    JSString * js_version = JS_InternString(cx, version);
    
    args.rval().set(STRING_TO_JSVAL(js_version));
    
    return true;
};
bool JSB_core_restartVM(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==0, cx, false, "Invalid number of arguments in executeScript");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    ScriptingCore::getInstance()->reset();
    args.rval().setUndefined();
    return true;
};
bool JSB_cleanScript(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc != 1)
    {
        JS_ReportError(cx, "Invalid number of arguments in JSB_cleanScript");
        return false;
    }
    
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JSString *jsPath = args.get(0).toString();
    JSB_PRECONDITION2(jsPath, cx, false, "Error js file in clean script");
    JSStringWrapper wrapper(jsPath);
    ScriptingCore::getInstance()->cleanScript(wrapper.get());
    
    args.rval().setUndefined();
    
    return true;
};

void registerDefaultClasses(JSContext* cx, JS::HandleObject global) {
    // first, try to get the ns
    JS::RootedValue nsval(cx);
    JS::RootedObject ns(cx);
    JS_GetProperty(cx, global, "ca", &nsval);
    // Not exist, create it
    if (nsval == JSVAL_VOID)
    {
        ns.set(JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));
        nsval = OBJECT_TO_JSVAL(ns);
        JS_SetProperty(cx, global, "ca", nsval);
    }
    else
    {
        ns.set(nsval.toObjectOrNull());
    }
    
    //
    // Javascript controller (__jsc__)
    //
    JS::RootedObject proto(cx);
    JS::RootedObject parent(cx);
    JS::RootedObject jsc(cx, JS_NewObject(cx, NULL, proto, parent));
    JS::RootedValue jscVal(cx);
    jscVal = OBJECT_TO_JSVAL(jsc);
    JS_SetProperty(cx, global, "__jsc__", jscVal);
    
    
    //在引擎中注册基础函数，实现这些就可以同javascript调用这些函数进行测试了
    
    JS_DefineFunction(cx, jsc, "garbageCollect", ScriptingCore::forceGC, 0, JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_ENUMERATE );
    JS_DefineFunction(cx, jsc, "dumpRoot", ScriptingCore::dumpRoot, 0, JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_ENUMERATE );
    JS_DefineFunction(cx, jsc, "addGCRootObject", ScriptingCore::addRootJS, 1, JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_ENUMERATE );
    JS_DefineFunction(cx, jsc, "removeGCRootObject", ScriptingCore::removeRootJS, 1, JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_ENUMERATE );
    JS_DefineFunction(cx, jsc, "executeScript", ScriptingCore::executeScript, 1, JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_ENUMERATE );
    
    // register some global functions
    JS_DefineFunction(cx, global, "require", ScriptingCore::executeScript, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, global, "log", ScriptingCore::log, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, global, "executeScript", ScriptingCore::executeScript, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, global, "forceGC", ScriptingCore::forceGC, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    
    JS_DefineFunction(cx, global, "__getOS", JSBCore_os, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, global, "__getVersion", JSBCore_version, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, global, "__restartVM", JSB_core_restartVM, 0, JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_ENUMERATE );
    JS_DefineFunction(cx, global, "__cleanScript", JSB_cleanScript, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, global, "__isObjectValid", ScriptingCore::isObjectValid, 1, JSPROP_READONLY | JSPROP_PERMANENT);
}


ScriptingCore* ScriptingCore::getInstance()
{
    if (!instance) {
        instance = new ScriptingCore();
    }
    return instance;
}

void ScriptingCore::addRegisterCallback(sc_register_sth callback) {
    registrationList.push_back(callback);
}
static bool
CheckObjectAccess(JSContext *cx)
{
    return true;
}
static JSSecurityCallbacks securityCallbacks = {
    CheckObjectAccess,
    NULL
};

void ScriptingCore::createGlobalContext()
{
    if (_cx && _rt)
    {
        ScriptingCore::removeAllRoots(_cx);
        JS_DestroyContext(_cx);
        JS_DestroyRuntime(_rt);
        _cx = nullptr;
        _rt = nullptr;
    }
    
    if (!_jsInited && !JS_Init())
    {
        return;
    }
    else
    {
        _jsInited = true;
    }
    
    _rt = JS_NewRuntime(8L * 1024L * 1024L);
    JS_SetGCParameter(_rt, JSGC_MAX_BYTES, 0xffffffff);
    
    JS_SetTrustedPrincipals(_rt, &shellTrustedPrincipals);
    JS_SetSecurityCallbacks(_rt, &securityCallbacks);
    JS_SetNativeStackQuota(_rt, JSB_MAX_STACK_QUOTA);
    
    
    _cx = JS_NewContext(_rt, 8192);
    
    JS::RuntimeOptionsRef(_rt).setIon(true);
    JS::RuntimeOptionsRef(_rt).setBaseline(true);
    
    JS_SetErrorReporter(_cx, ScriptingCore::reportError);
    
    _global = new (std::nothrow) JS::PersistentRootedObject(_rt, NewGlobalObject(_cx));
    JS::RootedObject global(_cx, _global->get());
    
    // Removed in Firefox v34
    js::SetDefaultObjectForContext(_cx, global);
    
    JSAutoCompartment ac(_cx, _global->get());
    
    runScript("script/jsb_prepare.js");
    
    for (std::vector<sc_register_sth>::iterator it = registrationList.begin(); it != registrationList.end(); it++) {
        sc_register_sth callback = *it;
        callback(_cx, *_global);
    }
    _needCleanup = true;
    
    runScript("script/jsb_boot.js");
    runScript("script/jsb_crossapp.js");
}

bool ScriptingCore::evalString(const char *string)
{
    JS::RootedValue retVal(_cx);
    return evalString(string, &retVal);
}

bool ScriptingCore::evalString(const char *string, JS::RootedValue *outVal, const char *filename, JSContext *cx, JSObject* global )
{
    if (cx == NULL) {
        cx = _cx;
    }
    
    if (global == NULL) {
        global = _global->get();
    }
    JSAutoCompartment ac(cx,global);
    JS::RootedObject cc(cx, global);
    JS::RootedValue rval(cx);
    bool ok =  JS_EvaluateScript(cx, cc, string, strlen(string), "ScriptingCore::evalString", 1, &rval);
    /*
    if (ok) {
        JSString *str = rval.toString();
        printf("javascript:-->%s",JS_EncodeString(cx, str));
    }
    */
    if (!ok) {
        CCLog("ScriptingCore::evalString faild-->   %s",string);
    }
    return ok;
}

static std::string RemoveFileExt(const std::string& filePath) {
    size_t pos = filePath.rfind('.');
    if (0 < pos) {
        return filePath.substr(0, pos);
    }
    else {
        return filePath;
    }
}

JSScript* ScriptingCore::getScript(const char *path){
    
    // a) check jsc file first
    std::string byteCodePath = RemoveFileExt(std::string(path)) + BYTE_CODE_FILE_EXT;
    if (filename_script.find(byteCodePath) != filename_script.end())
        return filename_script[byteCodePath];
    
    // b) no jsc file, check js file
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
    if (filename_script.find(fullPath) != filename_script.end())
        return filename_script[fullPath];
    
    return NULL;
}
void ScriptingCore::compileScript(const char *path, JSObject* global, JSContext* cx ){
    if (!path) {
        return;
    }
    
    if (getScript(path)) {
        return;
    }
    
    FileUtils * futil = FileUtils::getInstance();
    
    if (global == NULL) {
        global = *_global;
    }
    if (cx == NULL) {
        cx = _cx;
    }
    
    JSAutoCompartment ac(cx,global);
    
    JS::RootedScript script(cx);
    JS::RootedObject obj(cx,global);
    
    // a) check jsc file first
    std::string byteCodePath = RemoveFileExt(std::string(path)) + BYTE_CODE_FILE_EXT ;
    
    // Check whether '.jsc' files exist to avoid outputing log which says 'couldn't find .jsc file'.
    if (futil->isFileExist(byteCodePath)) {
        
        
        unsigned long size = 0;
        unsigned char* data = futil->getFileData(byteCodePath.c_str(), "rb", &size);
        
        if (size > 0)
        {
            script = JS_DecodeScript(cx, data, static_cast<uint32_t>(size), nullptr);
        }
    }
    
     // b) no jsc file, check js file
    if (!script) {
        /* Clear any pending exception from previous failed decoding.  */
        ReportException(cx);
        
        std::string fullPath = futil->fullPathForFilename(path);
        
        JS::CompileOptions op(cx);
        op.setUTF8(true);
        op.setFileAndLine(fullPath.c_str(), 1);
        
        bool ok = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string jsFileContent = futil->getFileString(fullPath);

        if (!jsFileContent.empty())
        {
            ok = JS::Compile(cx, obj, op, jsFileContent.c_str(), jsFileContent.size(), &script);
        }
#else
         ok = JS::Compile(cx, obj, op, fullPath.c_str(), &script);
#endif
        if (ok) {
            filename_script[fullPath] = script;
        }
    }
    else {
        filename_script[byteCodePath] = script;
    }
}

bool ScriptingCore::runScript(const char *path){
    return runScript(path, *_global, _cx);
}
bool ScriptingCore::runScript(const char *path, JS::HandleObject global, JSContext* cx)
{
    if (cx == NULL) {
        cx = _cx;
    }
    compileScript(path,global,cx);
    JS::RootedScript script(cx,getScript(path));
    bool evaluatedOK = false;
    if (script) {
        JS::RootedValue rval(cx);
        JSAutoCompartment ac(cx, global);
        evaluatedOK = JS_ExecuteScript(cx, global, script, &rval);
        if (false == evaluatedOK)
        {
            CCLog("Evaluating %s failed (evaluatedOK == JS_FALSE)", path);
            JS_ReportPendingException(cx);
        }
    }
    
    return evaluatedOK;
}

bool ScriptingCore::requireScript(const char *path, JS::MutableHandleValue jsvalRet)
{
    return requireScript(path, *_global, _cx, jsvalRet);
}

bool ScriptingCore::requireScript(const char *path, JS::HandleObject global, JSContext* cx, JS::MutableHandleValue jsvalRet)
{
    if(cx == NULL)
    {
        cx = _cx;
    }
    
    this->compileScript(path, global, cx);
    JS::RootedScript script(cx, this->getScript(path));
    bool evaluateOK = false;
    if (script) {
        JSAutoCompartment ac(cx,global);
        evaluateOK = JS_ExecuteScript(cx, global, script, jsvalRet);
        if (false == evaluateOK) {
            CCLog("(evaluatedOK == JS_FALSE)");
            JS_ReportPendingException(cx);
        }
    }
    return evaluateOK;
}

void ScriptingCore::cleanScript(const char *path)
{
    std::string byteCodePath = RemoveFileExt(std::string(path)) + BYTE_CODE_FILE_EXT ;
    auto it = filename_script.find(byteCodePath);
    if (it != filename_script.end()) {
        filename_script.erase(it);
    }
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
    it = filename_script.find(fullPath);
    if (it != filename_script.end()) {
        filename_script.erase(it);
    }
}

std::unordered_map<std::string, JSScript*>  &ScriptingCore::getFileScript()
{
    return filename_script;
}

void ScriptingCore::cleanAllScript()
{
    filename_script.clear();
}

void ScriptingCore::start()
{
    // for now just this
    createGlobalContext();
}

void ScriptingCore::cleanup()
{
    if (!_needCleanup) {
        return;
    }
    localStorageFree();
    removeAllRoots(_cx);
    garbageCollect();
    if (_cx)
    {
        JS_DestroyContext(_cx);
        _cx = nullptr;
    }
    if (_rt)
    {
        JS_DestroyRuntime(_rt);
        _rt = nullptr;
    }
    CC_SAFE_DELETE(_global);
    CC_SAFE_DELETE(_debugGlobal);
    _global = nullptr;
    _debugGlobal = nullptr;
    
    if (_js_log_buf) {
        free(_js_log_buf);
        _js_log_buf = NULL;
    }
    
    for (auto iter = _js_global_type_map.begin(); iter != _js_global_type_map.end(); ++iter)
    {
        free(iter->second->jsclass);
        free(iter->second);
    }
    
    _js_global_type_map.clear();
    filename_script.clear();
    registrationList.clear();
    
    _needCleanup = false;
}
void ScriptingCore::reset()
{
    CAApplication::getApplication()->restart();
}

void ScriptingCore::setEnabledReportError(bool var)
{
    _enabledReportError = var;
}

void ScriptingCore::reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
    std::string error_info = CrossApp::crossapp_format_string("%s:%u:%s\n",
                                                              report->filename ? report->filename : "<no filename=\"filename\">",
                                                              (unsigned int) report->lineno,
                                                              message);
    js_log(error_info.c_str());
    
    if (ScriptingCore::getInstance()->_enabledReportError)
    {
        CAAlertView::create("JavaScript报错信息", error_info)->show();
    }
};

void removeJSObject(JSContext* cx, void* nativeObj)
{
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    
    nproxy = jsb_get_native_proxy(nativeObj);
    if (nproxy) {
        jsproxy = jsb_get_js_proxy(nproxy->obj);
        RemoveObjectRoot(cx, &jsproxy->obj);
        jsb_remove_proxy(nproxy, jsproxy);
    }
}


bool ScriptingCore::log(JSContext* cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc > 0) {
        JSString *string = JS::ToString(cx, args.get(0));
        if (string) {
            
            JSStringWrapper wrapper(string);
            js_log("%s", wrapper.get());
        }
    }
    args.rval().setUndefined();
    return true;
}

void ScriptingCore::removeScriptObjectByObject(CAObject* pObj)
{
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    void *ptr = (void*)pObj;
    if (ptr)
    {
        nproxy = jsb_get_native_proxy(ptr);
        if (nproxy) {
            JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
            jsproxy = jsb_get_js_proxy(nproxy->obj);
            RemoveObjectRoot(cx, &jsproxy->obj);
            jsb_remove_proxy(nproxy, jsproxy);
        }
    }
}

bool ScriptingCore::setReservedSpot(uint32_t i, JSObject *obj, jsval value) {
    JS_SetReservedSlot(obj, i, value);
    return true;
}

bool ScriptingCore::executeScript(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc >= 1) {
        JSString* str = JS::ToString(cx, JS::RootedValue(cx, args.get(0)));
        JSStringWrapper path(str);
        bool res = false;
        if (argc == 2 && args.get(1).isString()) {
            JSString* globalName = args.get(1).toString();
            JSStringWrapper name(globalName);
            
            JS::RootedObject debugObj(cx, ScriptingCore::getInstance()->getDebugGlobal());
            if (debugObj) {
                res = ScriptingCore::getInstance()->runScript(path.get(), debugObj);
            } else {
                JS_ReportError(cx, "Invalid global object: %s", name.get());
                return false;
            }
        } else {
            JS::RootedObject glob(cx, JS::CurrentGlobalOrNull(cx));
            res = ScriptingCore::getInstance()->runScript(path.get(), glob);
        }
        return res;
    }
    args.rval().setUndefined();
    return true;
}
bool ScriptingCore::forceGC(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSRuntime *rt = JS_GetRuntime(cx);
    JS_GC(rt);
    return true;
}
bool ScriptingCore::dumpRoot(JSContext *cx, uint32_t argc, jsval *vp)
{
    // JS_DumpNamedRoots is only available on DEBUG versions of SpiderMonkey.
    // Mac and Simulator versions were compiled with DEBUG.
#if CrossApp_DEBUG
    //    JSContext *_cx = ScriptingCore::getInstance()->getGlobalContext();
    //    JSRuntime *rt = JS_GetRuntime(_cx);
    //    JS_DumpNamedRoots(rt, dumpNamedRoot, NULL);
    //    JS_DumpHeap(rt, stdout, NULL, JSTRACE_OBJECT, NULL, 2, NULL);
#endif
    return true;
}
bool ScriptingCore::addRootJS(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        JS::Heap<JSObject*> o(args.get(0).toObjectOrNull());
        if (AddNamedObjectRoot(cx, &o, "from-js") == false) {
            LOGD("something went wrong when setting an object to the root");
        }
        
        return true;
    }
    return false;
}
bool ScriptingCore::removeRootJS(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        JS::Heap<JSObject*> o(args.get(0).toObjectOrNull());
        if (o != nullptr) {
            RemoveObjectRoot(cx, &o);
        }
        return true;
    }
    return false;
}

// **
int ScriptingCore::sendEvent(ScriptEvent* evt)
{
    if (NULL == evt)
        return 0;
    
    // special type, can't use this code after JSAutoCompartment
    if (evt->type == kRestartGame)
    {
        restartVM();
        return 0;
    }
    
    JSAutoCompartment ac(_cx, _global->get());
    
    switch (evt->type)
    {
        case kViewControllerEvent:
        {
            return handleViewControllerEvent(evt->data);
        }
        break;
        default:
            CCAssert(false, "Invalid script event.");
            break;
    }
    
    return 0;
}

bool ScriptingCore::isObjectValid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 1) {
        JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
        js_proxy_t *proxy = jsb_get_js_proxy(tmpObj);
        if (proxy && proxy->ptr) {
            args.rval().set(JSVAL_TRUE);
        }
        else {
            args.rval().set(JSVAL_FALSE);
        }
        return true;
    }
    else {
        JS_ReportError(cx, "Invalid number of arguments: %d. Expecting: 1", argc);
        return false;
    }
}
void ScriptingCore::debugProcessInput(const std::string& str)
{
    JSAutoCompartment ac(_cx, *_debugGlobal);
    
    JSString* jsstr = JS_NewStringCopyZ(_cx, str.c_str());
    jsval argv = STRING_TO_JSVAL(jsstr);
    JS::RootedValue outval(_cx);
    
    JS_CallFunctionName(_cx, JS::RootedObject(_cx, *_debugGlobal), "processInput", JS::HandleValueArray::fromMarkedLocation(1, &argv), &outval);
}
//#pragma mark - Debugger
/*
static void _clientSocketWriteAndClearString(std::string& s)
{
    ::send(clientSocket, s.c_str(), s.length(), 0);
    s.clear();
}
*/

bool JSBDebug_BufferWrite(JSContext* cx, unsigned argc, jsval* vp)
{
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        JSStringWrapper strWrapper(args.get(0));
        // this is safe because we're already inside a lock (from clearBuffers)
        outData.append(strWrapper.get());
        //_clientSocketWriteAndClearString(outData);
    }
    return true;
}
static bool NS_ProcessNextEvent()
{
    g_qMutex.lock();
    size_t size = g_queue.size();
    g_qMutex.unlock();
    
    while (size > 0)
    {
        g_qMutex.lock();
        auto first = g_queue.begin();
        std::string str = *first;
        g_queue.erase(first);
        size = g_queue.size();
        g_qMutex.unlock();
        
        ScriptingCore::getInstance()->debugProcessInput(str);
    }
    //    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    return true;
}
bool JSBDebug_enterNestedEventLoop(JSContext* cx, unsigned argc, jsval* vp)
{
    enum {
        NS_OK = 0,
        NS_ERROR_UNEXPECTED
    };
    
#define NS_SUCCEEDED(v) ((v) == NS_OK)
    
    int rv = NS_OK;
    
    uint32_t nestLevel = ++s_nestedLoopLevel;
    
    while (NS_SUCCEEDED(rv) && s_nestedLoopLevel >= nestLevel) {
        if (!NS_ProcessNextEvent())
            rv = NS_ERROR_UNEXPECTED;
    }
    
    CCAssert(s_nestedLoopLevel <= nestLevel,
             "nested event didn't unwind properly");
    
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().set(UINT_TO_JSVAL(s_nestedLoopLevel));
    //    JS_SET_RVAL(cx, vp, UINT_TO_JSVAL(s_nestedLoopLevel));
    return true;
}
bool JSBDebug_exitNestedEventLoop(JSContext* cx, unsigned argc, jsval* vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (s_nestedLoopLevel > 0) {
        --s_nestedLoopLevel;
    } else {
        args.rval().set(UINT_TO_JSVAL(0));
        //        JS_SET_RVAL(cx, vp, UINT_TO_JSVAL(0));
        return true;
    }
    args.rval().setUndefined();
    //    JS_SET_RVAL(cx, vp, UINT_TO_JSVAL(s_nestedLoopLevel));
    return true;
}
bool JSBDebug_getEventLoopNestLevel(JSContext* cx, unsigned argc, jsval* vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().set(UINT_TO_JSVAL(s_nestedLoopLevel));
    //    JS_SET_RVAL(cx, vp, UINT_TO_JSVAL(s_nestedLoopLevel));
    return true;
}
static void processInput(const std::string& data) {
    std::lock_guard<std::mutex> lk(g_qMutex);
    g_queue.push_back(data);
}
static void clearBuffers() {
    std::lock_guard<std::mutex> lk(g_rwMutex);
    // only process input if there's something and we're not locked
    if (inData.length() > 0) {
        processInput(inData);
        inData.clear();
    }
    if (outData.length() > 0) {
       // _clientSocketWriteAndClearString(outData);
    }
}

/*
static void serverEntryPoint(unsigned int port)
{
    // start a server, accept the connection and keep reading data from it
    struct addrinfo hints, *result = nullptr, *rp = nullptr;
    int s = 0;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    
    std::stringstream portstr;
    portstr << port;
    
    int err = 0;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    WSADATA wsaData;
    err = WSAStartup(MAKEWORD(2, 2),&wsaData);
#endif
    
    if ((err = getaddrinfo(NULL, portstr.str().c_str(), &hints, &result)) != 0) {
        LOGD("getaddrinfo error : %s\n", gai_strerror(err));
    }
    
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if ((s = socket(rp->ai_family, rp->ai_socktype, 0)) < 0) {
            continue;
        }
        int optval = 1;
        if ((setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval))) < 0) {
            cc_closesocket(s);
            TRACE_DEBUGGER_SERVER("debug server : error setting socket option SO_REUSEADDR");
            return;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if ((setsockopt(s, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval))) < 0) {
            close(s);
            TRACE_DEBUGGER_SERVER("debug server : error setting socket option SO_NOSIGPIPE");
            return;
        }
#endif //(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        if ((::bind(s, rp->ai_addr, rp->ai_addrlen)) == 0) {
            break;
        }
        cc_closesocket(s);
        s = -1;
    }
    if (s < 0 || rp == NULL) {
        TRACE_DEBUGGER_SERVER("debug server : error creating/binding socket");
        return;
    }
    
    freeaddrinfo(result);
    
    listen(s, 1);
    
    while (true) {
        clientSocket = accept(s, NULL, NULL);
        
        if (clientSocket < 0)
        {
            TRACE_DEBUGGER_SERVER("debug server : error on accept");
            return;
        }
        else
        {
            // read/write data
            TRACE_DEBUGGER_SERVER("debug server : client connected");
            
            inData = "connected";
            // process any input, send any output
            clearBuffers();
            
            char buf[1024] = {0};
            int readBytes = 0;
            while ((readBytes = (int)::recv(clientSocket, buf, sizeof(buf), 0)) > 0)
            {
                buf[readBytes] = '\0';
                // TRACE_DEBUGGER_SERVER("debug server : received command >%s", buf);
                
                // no other thread is using this
                inData.append(buf);
                // process any input, send any output
                clearBuffers();
            } // while(read)
            
            cc_closesocket(clientSocket);
        }
    } // while(true)
}
*/
void ScriptingCore::enableDebugger(unsigned int port)
{
    if (!_debugGlobal)
    {
        JSAutoCompartment ac0(_cx, _global->get());
        
        JS_SetDebugMode(_cx, true);
        
        _debugGlobal = new (std::nothrow) JS::PersistentRootedObject(_cx, NewGlobalObject(_cx, true));
        // Adds the debugger object to root, otherwise it may be collected by GC.
        JS::RootedObject rootedDebugObj(_cx, _debugGlobal->get());
        JSAutoCompartment ac(_cx, rootedDebugObj);
        // these are used in the debug program
        JS_DefineFunction(_cx, rootedDebugObj, "_bufferWrite", JSBDebug_BufferWrite, 1, JSPROP_READONLY | JSPROP_PERMANENT);
        JS_DefineFunction(_cx, rootedDebugObj, "_enterNestedEventLoop", JSBDebug_enterNestedEventLoop, 0, JSPROP_READONLY | JSPROP_PERMANENT);
        JS_DefineFunction(_cx, rootedDebugObj, "_exitNestedEventLoop", JSBDebug_exitNestedEventLoop, 0, JSPROP_READONLY | JSPROP_PERMANENT);
        JS_DefineFunction(_cx, rootedDebugObj, "_getEventLoopNestLevel", JSBDebug_getEventLoopNestLevel, 0, JSPROP_READONLY | JSPROP_PERMANENT);
        
        runScript("script/jsb_debugger.js", rootedDebugObj);
        
        JS::RootedObject globalObj(_cx, _global->get());
        JS_WrapObject(_cx, &globalObj);
        // prepare the debugger
        jsval argv = OBJECT_TO_JSVAL(globalObj);
        JS::RootedValue outval(_cx);
        bool ok = JS_CallFunctionName(_cx, rootedDebugObj, "_prepareDebugger", JS::HandleValueArray::fromMarkedLocation(1, &argv), &outval);
        if (!ok) {
            JS_ReportPendingException(_cx);
        }

        CAScheduler::getScheduler()->schedule(schedule_selector(SimpleRunLoop::update), this->_runLoop, 0);
    }
}

void ScriptingCore::removeAllRoots(JSContext *cx) {
    // Native -> JS. No need to free "second"
    _native_js_global_map.clear();
    
    // JS -> Native: free "second" and "unroot" it.
    auto it_js = _js_native_global_map.begin();
    while (it_js != _js_native_global_map.end())
    {
        JS::RemoveObjectRoot(cx, &it_js->second->obj);
        free(it_js->second);
        it_js = _js_native_global_map.erase(it_js);
    }
    _js_native_global_map.clear();
}

JSObject* NewGlobalObject(JSContext* cx, bool debug)
{
    JS::CompartmentOptions options;
    options.setVersion(JSVERSION_LATEST);
    
    JS::RootedObject glob(cx, JS_NewGlobalObject(cx, &global_class, &shellTrustedPrincipals, JS::DontFireOnNewGlobalHook, options));
    if (!glob) {
        return nullptr;
    }
    JSAutoCompartment ac(cx, glob);
    bool ok = true;
    ok = JS_InitStandardClasses(cx, glob);
    if (ok)
        JS_InitReflect(cx, glob);
    if (ok && debug)
        ok = JS_DefineDebuggerObject(cx, glob);
    if (!ok)
        return nullptr;
    
    JS_FireOnNewGlobalObject(cx, glob);
    
    return glob;
}

bool ScriptingCore::executeFunctionWithOwner(jsval owner, const char *name, uint32_t argc, jsval *vp)
{
    JS::HandleValueArray args = JS::HandleValueArray::fromMarkedLocation(argc, vp);
    JS::RootedValue rval(_cx);
    return executeFunctionWithOwner(owner, name, args, &rval);
}

bool ScriptingCore::executeFunctionWithOwner(jsval owner, const char *name, uint32_t argc, jsval *vp, JS::MutableHandleValue retVal)
{
    //should not use CallArgs here, use HandleValueArray instead !!
    //because the "jsval* vp" is not the standard format from JSNative, the array doesn't contain this and retval
    //JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
//    CCLog("FunctionName:%s",name);
    JS::HandleValueArray args = JS::HandleValueArray::fromMarkedLocation(argc, vp);
    return executeFunctionWithOwner(owner, name, args, retVal);
}

bool ScriptingCore::executeFunctionWithOwner(jsval owner, const char *name, const JS::HandleValueArray& args)
{
    JS::RootedValue ret(_cx);
    return executeFunctionWithOwner(owner, name, args, &ret);
}

bool ScriptingCore::executeFunctionWithOwner(jsval owner, const char *name, const JS::HandleValueArray& args, JS::MutableHandleValue retVal)
{
    bool bRet = false;
    bool hasAction;
    JSContext* cx = this->_cx;
    JS::RootedValue temp_retval(cx);
    JS::RootedObject obj(cx, JS::RootedValue(cx, owner).toObjectOrNull());
    
    do
    {
        JSAutoCompartment ac(cx, obj);
        
        if (JS_HasProperty(cx, obj, name, &hasAction) && hasAction) {
            if (!JS_GetProperty(cx, obj, name, &temp_retval)) {
                break;
            }
            if (temp_retval == JSVAL_VOID) {
                break;
            }
            
            bRet = JS_CallFunctionName(cx, obj, name, args, retVal);
            
        }
    }while(0);
    return bRet;
}

static std::string getTouchFuncName(int eventCode)
{
    std::string funcName;
    switch(eventCode) {
        case CCTOUCHBEGAN:
            funcName = "onTouchBegan";
            break;
        case CCTOUCHENDED:
            funcName = "onTouchEnded";
            break;
        case CCTOUCHMOVED:
            funcName = "onTouchMoved";
            break;
        case CCTOUCHCANCELLED:
            funcName = "onTouchCancelled";
            break;
        default:
            CCAssert(false, "Invalid event code!");
    }
    
    return funcName;
}

static std::string getTouchesFuncName(int eventCode)
{
    std::string funcName;
    switch(eventCode)
    {
        case CCTOUCHBEGAN:
            funcName = "onTouchesBegan";
            break;
        case CCTOUCHENDED:
            funcName = "onTouchesEnded";
            break;
        case CCTOUCHMOVED:
            funcName = "onTouchesMoved";
            break;
        case CCTOUCHCANCELLED:
            funcName = "onTouchesCancelled";
            break;
        default:
            CCAssert(false, "Invalid event code!");
            break;
    }
    return funcName;
}

int ScriptingCore::executeCustomTouchesEvent(int eventType,const std::vector<CATouch*>& touches, JSObject *obj)
{
    std::string funcName = getTouchesFuncName(eventType);
    
    JS::RootedObject jsretArr(_cx, JS_NewArrayObject(this->_cx, 0));
    //    JS_AddNamedObjectRoot(this->_cx, &jsretArr, "touchArray");
    int count = 0;
    for (auto& touch : touches)
    {
        jsval jsret = getJSObject(this->_cx, touch);
        JS::RootedValue jsval(_cx, jsret);
        if (!JS_SetElement(this->_cx, jsretArr, count, jsval)) {
            break;
        }
        ++count;
    }
    
    jsval jsretArrVal = OBJECT_TO_JSVAL(jsretArr);
    executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), funcName.c_str(), 1, &jsretArrVal);
    //    JS_RemoveObjectRoot(this->_cx, &jsretArr);
    
    for (auto& touch : touches)
    {
        removeJSObject(this->_cx, touch);
    }
    
    return 1;
}

int ScriptingCore::executeCustomTouchEvent(int eventType,CATouch *pTouch, JSObject *obj)
{
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

    JS::RootedValue retval(_cx);
    std::string funcName = getTouchFuncName(eventType);

    jsval jsTouch = getJSObject(this->_cx, pTouch);

    executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), funcName.c_str(), 1, &jsTouch, &retval);
    
    // Remove touch object from global hash table and unroot it.
    removeJSObject(this->_cx, pTouch);
    
    return 1;
    
}

int ScriptingCore::executeCustomTouchEvent(int eventType,CATouch *pTouch, JSObject *obj,JS::MutableHandleValue retval)
{
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
    
    std::string funcName = getTouchFuncName(eventType);
    
    jsval jsTouch = getJSObject(this->_cx, pTouch);
    
    executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), funcName.c_str(), 1, &jsTouch, retval);
    
    // Remove touch object from global hash table and unroot it.
    removeJSObject(this->_cx, pTouch);
    
    return 1;
    
}

bool ScriptingCore::isFunctionOverridedInJS(JS::HandleObject obj, const std::string& name, JSNative native)
{
    JS::RootedObject jsobj(_cx, obj);
    JSAutoCompartment ac(_cx, jsobj);
    JS::RootedValue value(_cx);
    bool ok = JS_GetProperty(_cx, jsobj, name.c_str(), &value);
    if (ok && !value.isNullOrUndefined() && !JS_IsNativeFunction(value.toObjectOrNull(), native))
    {
        return true;
    }
    
    return false;
}

int ScriptingCore::handleViewControllerEvent(void* data)
{
    if (NULL == data)
    return 0;
    
    BasicScriptData* basicScriptData = static_cast<BasicScriptData*>(data);
    if (NULL == basicScriptData->nativeObject || NULL == basicScriptData->value)
    return 0;
    
    CAViewController* viewController = static_cast<CAViewController*>(basicScriptData->nativeObject);
    int action = *((int*)(basicScriptData->value));
    js_proxy_t * p = jsb_get_native_proxy(viewController);
    if (!p) return 0;
    
    JSAutoCompartment ac(_cx, *_global);
    
    int ret = 0;
    JS::RootedValue retval(_cx);
    jsval dataVal = INT_TO_JSVAL(1);
    
    JS::RootedObject jstarget(_cx, p->obj);
    
    if (action == script::viewDidUnload)
    {
        ret = executeFunctionWithOwner(OBJECT_TO_JSVAL(p->obj), "viewDidUnload", 0, &dataVal, &retval);
    }
    else if (action == script::viewDidLoad)
    {
        ret = executeFunctionWithOwner(OBJECT_TO_JSVAL(p->obj), "viewDidLoad", 0, &dataVal, &retval);
    }
    else if (action == script::viewSizeDidChanged)
    {
        ret = executeFunctionWithOwner(OBJECT_TO_JSVAL(p->obj), "viewSizeDidChanged", 0, &dataVal, &retval);
    }
    else if (action == script::viewDidAppear)
    {
        ret = executeFunctionWithOwner(OBJECT_TO_JSVAL(p->obj), "viewDidAppear", 0, &dataVal, &retval);
    }
    else if (action == script::viewDidDisappear)
    {
        ret = executeFunctionWithOwner(OBJECT_TO_JSVAL(p->obj), "viewDidDisappear", 0, &dataVal, &retval);
    }
    return ret;
}

void ScriptingCore::garbageCollect()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
    auto runtime = JS_GetRuntime(_cx);
    // twice: yep, call it twice since this is a generational GC
    // and we want to collect as much as possible when this is being called
    // from replaceScene().
    JS_GC(runtime);
    JS_GC(runtime);
#endif
}

#pragma mark - Debug

void SimpleRunLoop::update(float dt)
{
    g_qMutex.lock();
    size_t size = g_queue.size();
    g_qMutex.unlock();
    
    while (size > 0)
    {
        g_qMutex.lock();
        auto first = g_queue.begin();
        std::string str = *first;
        g_queue.erase(first);
        size = g_queue.size();
        g_qMutex.unlock();
        
        ScriptingCore::getInstance()->debugProcessInput(str);
    }
}

void ScriptingCore::releaseScriptObject(CrossApp::CAObject* owner, CrossApp::CAObject* target)
{
    JS::RootedObject global(_cx, _global->get());
    JS::RootedObject jsbObj(_cx);
    get_or_create_js_obj(_cx, global, "jsb", &jsbObj);
    JS::RootedValue jsbVal(_cx, OBJECT_TO_JSVAL(jsbObj));
    if (jsbVal.isNullOrUndefined())
    {
        return;
    }
    
    js_proxy_t *pOwner = jsb_get_native_proxy(owner);
    js_proxy_t *pTarget = jsb_get_native_proxy(target);
    if (!pOwner || !pTarget)
    {
        return;
    }
    JS::RootedValue valOwner(_cx, OBJECT_TO_JSVAL(pOwner->obj));
    JS::RootedValue valTarget(_cx, OBJECT_TO_JSVAL(pTarget->obj));
    if (valTarget.isPrimitive())
    {
        return;
    }
    
    JS::RootedValue retval(_cx);
    jsval valArr[2];
    valArr[0] = valOwner;
    valArr[1] = valTarget;
    
    JS::HandleValueArray args = JS::HandleValueArray::fromMarkedLocation(2, valArr);
    executeFunctionWithOwner(jsbVal, "unregisterNativeObject", args, &retval);
}

void ScriptingCore::releaseAllSubviewsRecursive(CrossApp::CAView* view)
{
    const CAVector<CAView*>& subviews = view->getSubviews();
    for (auto subview : subviews)
    {
        releaseScriptObject(view, subview);
        releaseAllSubviewsRecursive(subview);
    }
}

js_proxy_t* jsb_new_proxy(void* nativeObj, JSObject* jsObj)
{
    js_proxy_t* proxy = nullptr;
    
    if (nativeObj && jsObj)
    {
        // native to JS index
        proxy = (js_proxy_t *)malloc(sizeof(js_proxy_t));
        CC_ASSERT(proxy && "not enough memory");

        auto existJSProxy = _js_native_global_map.find(jsObj);
        if (existJSProxy != _js_native_global_map.end()) {
            
            jsb_remove_proxy(existJSProxy->second);
        }
        
        proxy->ptr = nativeObj;
        proxy->obj = jsObj;
        proxy->_jsobj = jsObj;
        
        
        // One Proxy in two entries
        _native_js_global_map[nativeObj] = proxy;
        _js_native_global_map[jsObj] = proxy;
    }
    else CCLOG("jsb_new_proxy: Invalid keys");
    
    return proxy;
}

js_proxy_t* jsb_get_native_proxy(void* nativeObj)
{
    auto search = _native_js_global_map.find(nativeObj);
    if(search != _native_js_global_map.end())
    return search->second;
    return nullptr;
}

js_proxy_t* jsb_get_js_proxy(JSObject* jsObj)
{
    auto search = _js_native_global_map.find(jsObj);
    if(search != _js_native_global_map.end())
    return search->second;
    return nullptr;
}

void jsb_remove_proxy(js_proxy_t* nativeProxy, js_proxy_t* jsProxy)
{
    js_proxy_t* proxy = nativeProxy ? nativeProxy : jsProxy;
    jsb_remove_proxy(proxy);
}

void jsb_remove_proxy(js_proxy_t* proxy)
{
    void* nativeKey = proxy->ptr;
    JSObject* jsKey = proxy->_jsobj;
//
    CC_ASSERT(nativeKey && "Invalid nativeKey");
    CC_ASSERT(jsKey && "Invalid JSKey");
//
    auto it_nat = _native_js_global_map.find(nativeKey);
    auto it_js = _js_native_global_map.find(jsKey);
//
#if 0
    // XXX FIXME: sanity check. Remove me once it is tested that it works Ok
    if (it_nat != _native_js_global_map.end() && it_js != _js_native_global_map.end())
    {
        CC_ASSERT(it_nat->second == it_js->second && "BUG. Different enties");
    }
#endif

    if (it_nat != _native_js_global_map.end())
    {
        _native_js_global_map.erase(it_nat);
    }
    else CCLOG("jsb_remove_proxy: failed. Native key not found");
    
    if (it_js != _js_native_global_map.end())
    {
        // Free it once, since we only have one proxy alloced entry
        free(it_js->second);
        _js_native_global_map.erase(it_js);
    }
    else CCLOG("jsb_remove_proxy: failed. JS key not found");
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppHelper_evalString(JNIEnv *env, jclass cls, jstring value) {
        
        const char* strValue = env->GetStringUTFChars(value, NULL);
        ScriptingCore::getInstance()->evalString(strValue);
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppHelper_evalStringOnGLThread(JNIEnv *env, jclass cls, jstring value) {
        
        const char* strValue = env->GetStringUTFChars(value, NULL);
        CrossApp::CAScheduler::getScheduler()->performFunctionInUIThread([=](){
            ScriptingCore::getInstance()->evalString(strValue);
        });
    }
}
#endif
