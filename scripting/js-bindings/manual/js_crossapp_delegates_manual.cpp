//
//  js_crossapp_delegates_manual.cpp
//  HelloJavacript
//
//  Created by zhanglei on 16/7/6.
//
//


#include "js_crossapp_delegates_manual.hpp"
#include "crossapp_specifics.hpp"
#include "jsb_crossapp_auto.hpp"

#include "CrossApp.h"
typedef CAMap<std::string,CAObject*> CADelegateMap;

#define KEY_DOWNLOAD_MANAGER_DELEGATE  "DownloadManagerDelegate"

class JSB_DownloadManagerDelegate : public CAObject,public CADownloadManagerDelegate
{
public:
    JSB_DownloadManagerDelegate()
    {
        _JSDelegate = nullptr;
    }
    virtual ~JSB_DownloadManagerDelegate()
    {
        _JSDelegate = nullptr;
    }
    void setJSDataSource(JS::HandleObject pJSDelegate)
    {
        _JSDelegate = pJSDelegate;
    }
    virtual void onError(unsigned long download_id, CADownloadManager::ErrorCode errorCode){
        
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS::RootedValue retval(cx);
        
        jsval args[2];
        
        args[0] = long_to_jsval(cx, download_id);
        args[1] = uint32_to_jsval(cx, errorCode);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate), "onError", 2, args);
    };
    
    virtual void onProgress(unsigned long download_id, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded){
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS::RootedValue retval(cx);
        
        jsval args[4];
        
        args[0] = long_to_jsval(cx, download_id);
        args[1] = uint32_to_jsval(cx, percent);
        args[2] = long_to_jsval(cx, nowDownloaded);
        args[3] = long_to_jsval(cx, totalToDownloaded);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate), "onProgress", 4, args);
    };
    
    virtual void onPauseDownload(unsigned long download_id){
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS::RootedValue retval(cx);
        
        jsval args[1];
        args[0] = long_to_jsval(cx, download_id);

        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate), "onPauseDownload", 1, args);
    };
    
    virtual void onResumeDownload(unsigned long download_id){
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS::RootedValue retval(cx);
        
        jsval args[1];
        args[0] = long_to_jsval(cx, download_id);
        
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate), "onResumeDownload", 1, args);
    };
    
    virtual void onSuccess(unsigned long download_id){
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS::RootedValue retval(cx);
        
        jsval args[1];
        args[0] = long_to_jsval(cx, download_id);
        
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate), "onSuccess", 1, args);
    };
private:
    JS::Heap<JSObject*> _JSDelegate;
};

static bool js_crossapp_CADownloadManager_setDownloadManagerDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CrossApp::CADownloadManager* cobj = (CrossApp::CADownloadManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_crossapp_CADownloadManager_setDownloadManagerDelegate : Invalid Native Object");
    if (argc == 1) {
        JSB_DownloadManagerDelegate* arg0 =  new (std::nothrow) JSB_DownloadManagerDelegate();;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = args.get(0).toObjectOrNull();
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (JSB_DownloadManagerDelegate*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_crossapp_CADownloadManager_setDownloadManagerDelegate : Error processing arguments");
        cobj->setDownloadManagerDelegate(arg0);
        args.rval().setUndefined();
        return true;
    }
    
    JS_ReportError(cx, "js_crossapp_CADownloadManager_setDownloadManagerDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}


void register_all_crossapp_delegates_manual(JSContext* cx, JS::HandleObject global)
{
    JS::RootedObject ccObj(cx);
    JS::RootedObject jsbObj(cx);
    JS::RootedValue tmpVal(cx);
    JS::RootedObject tmpObj(cx);
    get_or_create_js_obj(cx, global, "ca", &ccObj);
    get_or_create_js_obj(cx, global, "jsb", &jsbObj);

    tmpObj.set(jsb_CrossApp_CADownloadManager_prototype);
    JS_DefineFunction(cx, tmpObj, "setDownloadManagerDelegate", js_crossapp_CADownloadManager_setDownloadManagerDelegate, 1, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    
}
