#include "CrossApp.h"
#include "CrossAppExt.h"
#include "CrossApp_specifics.hpp"
#include <typeinfo>
#include "js_bindings_config.h"
#include "generated/jsb_CrossApp_auto.hpp"
#include "js_manual_conversions.h"
USING_NS_CC_EXT;

schedFunc_proxy_t *_schedFunc_target_ht = NULL;
schedTarget_proxy_t *_schedObj_target_ht = NULL;
callfuncTarget_proxy_t *_callfuncTarget_native_ht = NULL;


static void addCallBackAndThis(JSObject *obj, jsval callback, jsval &thisObj) {
    if(callback != JSVAL_VOID) {
        ScriptingCore::getInstance()->setReservedSpot(0, obj, callback);
    }
    if(thisObj != JSVAL_VOID) {
        ScriptingCore::getInstance()->setReservedSpot(1, obj, thisObj);
    }
}
//TODO:JSB_ScrollViewDelegate
class JSB_ScrollViewDelegate
: public CAObject
, public CAScrollViewDelegate
{
public:
    JSB_ScrollViewDelegate()
    : m_pJSDelegate(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_ScrollViewDelegate()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSDelegate);
        }
    }
    
    virtual void scrollViewDidScroll(CAScrollView* view)
    {
        js_proxy_t * p = jsb_get_native_proxy(view);
        if (!p) return;
        
        jsval arg = OBJECT_TO_JSVAL(p->obj);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "scrollViewDidScroll", 1, &arg, NULL);
    }
    
    virtual void scrollViewDidZoom(CAScrollView* view)
    {
        js_proxy_t * p = jsb_get_native_proxy(view);
        if (!p) return;
        
        jsval arg = OBJECT_TO_JSVAL(p->obj);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "scrollViewDidZoom", 1, &arg, NULL);
    }
    
    void setJSDelegate(JSObject* pJSDelegate)
    {
        m_pJSDelegate = pJSDelegate;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSDelegate);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSDelegate, "TableViewDelegate");
        }
    }
private:
    JSObject* m_pJSDelegate;
    bool m_bNeedUnroot;
};
//TODO: js_CrossApp_CAScrollView_setDelegate
static JSBool js_CrossApp_CAScrollView_setDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CAScrollView* cobj = (CAScrollView *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        // save the delegate
        JSObject *jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSB_ScrollViewDelegate* nativeDelegate = new JSB_ScrollViewDelegate();
        nativeDelegate->setJSDelegate(jsDelegate);
        
        cobj->setUserObject(nativeDelegate);
        cobj->setScrollViewDelegate(nativeDelegate);
        
        nativeDelegate->release();
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}

//TODO: JSB_TextfieldDelegate
class JSB_TextfieldDelegate
: public CAObject
, public CATextFieldDelegate
{
public:
    JSB_TextfieldDelegate()
    : m_pJSDelegate(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_TextfieldDelegate()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSDelegate);
        }
    }
    virtual bool onTextFieldAttachWithIME(CATextField * sender)
    {
        js_proxy_t * p = jsb_get_native_proxy(sender);
        if (!p) return true;
        
        jsval arg = OBJECT_TO_JSVAL(p->obj);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "onTextFieldAttachWithIME", 1, &arg, NULL);

        return false;
    }
    
	//If the sender doesn't want to detach from the IME, return true;
    virtual bool onTextFieldDetachWithIME(CATextField * sender)
    {
        js_proxy_t * p = jsb_get_native_proxy(sender);
        if (!p) return true;
        
        jsval arg = OBJECT_TO_JSVAL(p->obj);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "onTextFieldDetachWithIME", 1, &arg, NULL);
        
        return false;
    }
    
	//If the sender doesn't want to insert the text, return true;
    virtual bool onTextFieldInsertText(CATextField * sender, const char * text, int nLen)
    {
        js_proxy_t * p = jsb_get_native_proxy(sender);
        if (!p) return true;
        
       
        //jsval arg = OBJECT_TO_JSVAL(p->obj);
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        jsval args[3];
        JSString *_text;
        _text = JS_NewStringCopyZ(cx, text);
        args[0] = OBJECT_TO_JSVAL(p->obj);
        args[1] = STRING_TO_JSVAL(_text);
        args[2] = INT_TO_JSVAL(nLen);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "onTextFieldInsertText", 3, args, NULL);
        
        return false;
    }
    
    //If the sender doesn't want to delete the delText, return true;
    virtual bool onTextFieldDeleteBackward(CATextField * sender, const char * delText, int nLen)
    {
        js_proxy_t * p = jsb_get_native_proxy(sender);
        if (!p) return true;
        
        
        //jsval arg = OBJECT_TO_JSVAL(p->obj);
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        jsval args[3];
        JSString *_text;
        _text = JS_NewStringCopyZ(cx, delText);
        args[0] = OBJECT_TO_JSVAL(p->obj);
        args[1] = STRING_TO_JSVAL(_text);
        args[2] = INT_TO_JSVAL(nLen);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "onTextFieldDeleteBackward", 3, args, NULL);
        return false;
    }
    virtual bool getKeyBoardHeight(CATextField *textfield ,int height)
    {
        js_proxy_t * p = jsb_get_native_proxy(textfield);
        if (!p) return true;
        
        
        //jsval arg = OBJECT_TO_JSVAL(p->obj);

        jsval args[2];
        args[0] = OBJECT_TO_JSVAL(p->obj);
        args[1] = INT_TO_JSVAL(height);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "getKeyBoardHeight", 2, args, NULL);
        return false;
    }
    void setJSDelegate(JSObject* pJSDelegate)
    {
        m_pJSDelegate = pJSDelegate;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSDelegate);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSDelegate, "TextFieldDelegate");
        }
    }
private:
    JSObject* m_pJSDelegate;
    bool m_bNeedUnroot;
};
//TODO: js_CrossApp_CAScrollView_setDelegate
static JSBool js_CrossApp_CATextField_setDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CATextField* cobj = (CATextField *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        // save the delegate
        JSObject *jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSB_TextfieldDelegate* nativeDelegate = new JSB_TextfieldDelegate();
        nativeDelegate->setJSDelegate(jsDelegate);
        
        cobj->setUserObject(nativeDelegate);
        cobj->setDelegate(nativeDelegate);
        
        nativeDelegate->release();
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}

//TODO: JSB_PickerViewDelegate
class JSB_PickerViewDelegate
: public CAObject
, public CAPickerViewDelegate
{
public:
    JSB_PickerViewDelegate()
    : m_pJSDelegate(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_PickerViewDelegate()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSDelegate);
        }
    }
    
    virtual void didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
    {
        js_proxy_t * p = jsb_get_native_proxy(pickerView);
        if (!p) return;
        
        jsval args[3];
        args[0] = OBJECT_TO_JSVAL(p->obj);
        args[1] = INT_TO_JSVAL(row);
        args[2] = INT_TO_JSVAL(component);
        
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "didSelectRow", 3, args, NULL);
    }
    
    void setJSDelegate(JSObject* pJSDelegate)
    {
        m_pJSDelegate = pJSDelegate;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSDelegate);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSDelegate, "PickerViewDelegate");
        }
    }
private:
    JSObject* m_pJSDelegate;
    bool m_bNeedUnroot;
};
//TODO: js_CrossApp_CAPickerView_setDelegate
static JSBool js_CrossApp_CAPickerView_setDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CAPickerView* cobj = (CAPickerView *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        // save the delegate
        
        JSObject *jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSB_PickerViewDelegate* nativeDelegate = new JSB_PickerViewDelegate();
        nativeDelegate->setJSDelegate(jsDelegate);
        
        CCDictionary* userDict = static_cast<CCDictionary*>(cobj->getUserObject());
        if (NULL == userDict)
        {
            userDict = new CCDictionary();
            cobj->setUserObject(userDict);
            userDict->release();
        }
        
        userDict->setObject(nativeDelegate, "PickerViewDelegate");
        
        cobj->setDelegate(nativeDelegate);
        
        nativeDelegate->release();
        
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}


#define KEY_CAVIEWCONTROLLER_VIEWDIDLOAD  "viewDidLoad"
#define KEY_CAVIEWCONTROLLER_VIEWDIDUNLOAD     "viewDidUnload"

#define KEY_TABLEVIEW_DATA_SOURCE  "TableViewDataSource"
#define KEY_TABLEVIEW_DELEGATE     "TableViewDelegate"
//TODO: JSB_PageViewDelegate
class JSB_PageViewDelegate
: public CAObject
, public CAPageViewDelegate
{
public:
    JSB_PageViewDelegate()
    : m_pJSDelegate(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_PageViewDelegate()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSDelegate);
        }
    }
    
    virtual void pageViewDidBeginTurning(CAPageView* pageView)
    {
        js_proxy_t * p = jsb_get_native_proxy(pageView);
        if (!p) return;
        
        jsval arg = OBJECT_TO_JSVAL(p->obj);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "pageViewDidBeginTurning", 1, &arg, NULL);
    };
    
    virtual void pageViewDidEndTurning(CAPageView* pageView)
    {
        js_proxy_t * p = jsb_get_native_proxy(pageView);
        if (!p) return;
        
        jsval arg = OBJECT_TO_JSVAL(p->obj);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "pageViewDidEndTurning", 1, &arg, NULL);
        
    };
    
    virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index)
    {
        js_proxy_t * p = jsb_get_native_proxy(pageView);
        if (!p) return;
        
        jsval args[2];
        args[0] = OBJECT_TO_JSVAL(p->obj);
        args[1] = INT_TO_JSVAL(index);
        
        
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "pageViewDidSelectPageAtIndex", 2, args, NULL);
        
    };
    
    
    
    void setJSDelegate(JSObject* pJSDelegate)
    {
        m_pJSDelegate = pJSDelegate;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSDelegate);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSDelegate, "PageViewDelegate");
        }
    }
    
    
private:
    
    
    void callJSDelegate(CATableView* table, unsigned int section, unsigned int row , std::string jsFunctionName)
    {
        
        
        
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return;
        
        jsval args[3];
        args[0] = OBJECT_TO_JSVAL(p->obj);
        args[1] = INT_TO_JSVAL(section);
        args[2] = INT_TO_JSVAL(row);
        
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), jsFunctionName.c_str(), 3, args, NULL);
    }
    
    JSObject* m_pJSDelegate;
    bool m_bNeedUnroot;
};
//TODO:  js_CrossApp_CAPageView_setDelegate
static JSBool js_CrossApp_CAPageView_setDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CAPageView* cobj = (CAPageView *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        // save the delegate
        JSObject *jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSB_PageViewDelegate* nativeDelegate = new JSB_PageViewDelegate();
        nativeDelegate->setJSDelegate(jsDelegate);
        
        CCDictionary* userDict = static_cast<CCDictionary*>(cobj->getUserObject());
        if (NULL == userDict)
        {
            userDict = new CCDictionary();
            cobj->setUserObject(userDict);
            userDict->release();
        }
        
        userDict->setObject(nativeDelegate, KEY_TABLEVIEW_DELEGATE);
        
        cobj->setPageViewDelegate(nativeDelegate);
        
        nativeDelegate->release();
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}


//TODO:  JSB_TableViewDelegate
class JSB_TableViewDelegate
: public CAObject
, public CATableViewDelegate
{
public:
    JSB_TableViewDelegate()
    : m_pJSDelegate(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_TableViewDelegate()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSDelegate);
        }
    }
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
    {
        callJSDelegate(table, section,row,"tableViewDidSelectRowAtIndexPath");
    };
    
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
    {
        callJSDelegate(table, section,row,"tableViewDidDeselectRowAtIndexPath");
    };
    
    
    void setJSDelegate(JSObject* pJSDelegate)
    {
        m_pJSDelegate = pJSDelegate;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSDelegate);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSDelegate, "TableViewDelegate");
        }
    }
    
    
private:
    
    
    void callJSDelegate(CATableView* table, unsigned int section, unsigned int row , std::string jsFunctionName)
    {
        
        
        
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p || !m_pJSDelegate) return;
        
        jsval args[3];
        args[0] = OBJECT_TO_JSVAL(p->obj);
        args[1] = INT_TO_JSVAL(section);
        args[2] = INT_TO_JSVAL(row);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), jsFunctionName.c_str(), 3, args, NULL);
    }
    
    JSObject* m_pJSDelegate;
    bool m_bNeedUnroot;
};
//TODO:  js_CrossApp_CATableView_setDelegate
static JSBool js_CrossApp_CATableView_setDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CATableView* cobj = (CATableView *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        // save the delegate
        JSObject *jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSB_TableViewDelegate* nativeDelegate = new JSB_TableViewDelegate();
        nativeDelegate->setJSDelegate(jsDelegate);
        
        CCDictionary* userDict = static_cast<CCDictionary*>(cobj->getUserObject());
        if (NULL == userDict)
        {
            userDict = new CCDictionary();
            cobj->setUserObject(userDict);
            userDict->release();
        }
        
        userDict->setObject(nativeDelegate, KEY_TABLEVIEW_DELEGATE);
        
        cobj->setTableViewDelegate(nativeDelegate);
        
        nativeDelegate->release();
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}

//TODO: JSB_PickerViewDataSource
class JSB_PickerViewDataSource: public CAObject
, public CAPickerViewDataSource
{
public:
    JSB_PickerViewDataSource()
    : m_pJSPickerViewDataSource(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_PickerViewDataSource()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSPickerViewDataSource);
        }
    }
    
    
    
    
    virtual unsigned int numberOfComponentsInPickerView(CAPickerView* pickerView)
    {
        jsval ret;
        bool ok = callJSDataSource(pickerView, "numberOfComponentsInPickerView", ret);
        if (ok) {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            uint32_t size = 0;
            JSBool isSucceed = jsval_to_uint32(cx, ret, &size);
            if (isSucceed) return size;
        }
        return 0;
        
    }
    
    virtual unsigned int numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component)
    {
        jsval ret;
        bool ok = callJSDataSource(pickerView,component ,"numberOfRowsInComponent", ret);
        if (ok) {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            uint32_t size = 0;
            JSBool isSucceed = jsval_to_uint32(cx, ret, &size);
            if (isSucceed) return size;
        }
        return 0;
        
    }
    
    virtual float widthForComponent(CAPickerView* pickerView, unsigned int component)
    {
        jsval ret;
        bool ok = callJSDataSource(pickerView , component, "widthForComponent", ret);
        if (ok) {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            uint32_t size = 0;
            JSBool isSucceed = jsval_to_uint32(cx, ret, &size);
            if (isSucceed) return size;
        }
        return 0;
    }
    
    virtual float rowHeightForComponent(CAPickerView* pickerView, unsigned int component)
    {
        jsval ret;
        bool ok = callJSDataSource(pickerView , component, "rowHeightForComponent", ret);
        if (ok) {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            uint32_t size = 0;
            JSBool isSucceed = jsval_to_uint32(cx, ret, &size);
            if (isSucceed) return size;
        }
        return 0;
    }
    
    virtual CCString* titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
    {
        jsval ret;
        bool ok = callJSDataSource(pickerView ,row, component, "titleForRow", ret);
        if (ok) {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            const char * size = 0;
            JSBool isSucceed = jsval_to_charptr(cx, ret, &size);
            if (isSucceed) return CCString::create(size);
        }
        return CCString::create("123");
    }
    
    virtual CAView* viewForRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
    {
        jsval ret;
        bool ok = callJSDataSource(pickerView, row,component ,"viewForRow", ret);
        if (ok)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            CAView* arg0;
            do {
                js_proxy_t *proxy;
                JSObject *tmpObj = JSVAL_TO_OBJECT(ret);
                proxy = jsb_get_js_proxy(tmpObj);
                arg0 = (CAView*)(proxy ? proxy->ptr : NULL);
                JSB_PRECONDITION2( arg0, cx, NULL, "Invalid Native Object");
            } while (0);
            return arg0;
        }
        return NULL;
        
    }
    
    virtual CAView* viewForSelect(CAPickerView* pickerView, unsigned int component, const CCSize& size)
    {
        jsval ret;
        bool ok = callJSDataSource(pickerView, component,size ,"viewForSelect", ret);
        if (ok)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            CAView* arg0;
            do {
                js_proxy_t *proxy;
                JSObject *tmpObj = JSVAL_TO_OBJECT(ret);
                proxy = jsb_get_js_proxy(tmpObj);
                arg0 = (CAView*)(proxy ? proxy->ptr : NULL);
                JSB_PRECONDITION2( arg0, cx, NULL, "Invalid Native Object");
            } while (0);
            return arg0;
        }
        return NULL;
    }
    
    void setPickerViewDataSource(JSObject* pJSSource)
    {
        m_pJSPickerViewDataSource = pJSSource;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSPickerViewDataSource);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSPickerViewDataSource, "PickerViewDataSource");
        }
    }
    
private:
    bool callJSDataSource(CAPickerView* table, std::string jsFunctionName, jsval& retVal)
    {
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return false;
        
        JSBool hasAction;
        jsval temp_retval;
        jsval dataVal = OBJECT_TO_JSVAL(p->obj);
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JSObject* obj = m_pJSPickerViewDataSource;
        
        if (JS_HasProperty(cx, obj, jsFunctionName.c_str(), &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, jsFunctionName.c_str(), &temp_retval)) {
                return false;
            }
            if(temp_retval == JSVAL_VOID) {
                return false;
            }
            JSAutoCompartment ac(cx, obj);
            JS_CallFunctionName(cx, obj, jsFunctionName.c_str(),
                                1, &dataVal, &retVal);
            return true;
        }
        return false;
    }
    
    bool callJSDataSource(CAPickerView* table,unsigned int component, std::string jsFunctionName, jsval& retVal)
    {
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return false;
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JSBool hasAction;
        jsval temp_retval;
        jsval dataVal[2];
        dataVal[0] = OBJECT_TO_JSVAL(p->obj);
        dataVal[1] = INT_TO_JSVAL(component);
        
        JSObject* obj = m_pJSPickerViewDataSource;
        
        if (JS_HasProperty(cx, obj, jsFunctionName.c_str(), &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, jsFunctionName.c_str(), &temp_retval)) {
                return false;
            }
            if(temp_retval == JSVAL_VOID) {
                return false;
            }
            JSAutoCompartment ac(cx, obj);
            JS_CallFunctionName(cx, obj, jsFunctionName.c_str(),
                                2, dataVal, &retVal);
            return true;
        }
        return false;
    }
    
    
    bool callJSDataSource(CAPickerView* table, int row , int component, std::string jsFunctionName, jsval& retVal)
    {
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return false;
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JSBool hasAction;
        jsval temp_retval;
        jsval dataVal[3];
        dataVal[0] = OBJECT_TO_JSVAL(p->obj);
        dataVal[1] = INT_TO_JSVAL(row);
        dataVal[2] = INT_TO_JSVAL(component);
        JSObject* obj = m_pJSPickerViewDataSource;
        
        if (JS_HasProperty(cx, obj, jsFunctionName.c_str(), &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, jsFunctionName.c_str(), &temp_retval)) {
                return false;
            }
            if(temp_retval == JSVAL_VOID) {
                return false;
            }
            JSAutoCompartment ac(cx, obj);
            JS_CallFunctionName(cx, obj, jsFunctionName.c_str(),
                                3, dataVal, &retVal);
            return true;
        }
        return false;
    }
    
    bool callJSDataSource(CAPickerView* table, int component , const CCSize& cellSize, std::string jsFunctionName, jsval& retVal)
    {
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return false;
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JSBool hasAction;
        jsval temp_retval;
        jsval dataVal[3];
        CADipSize size = cellSize;
        dataVal[0] = OBJECT_TO_JSVAL(p->obj);
        dataVal[1] = INT_TO_JSVAL(component);
        dataVal[2] = ccsize_to_jsval(cx, size);
        
        JSObject* obj = m_pJSPickerViewDataSource;
        
        if (JS_HasProperty(cx, obj, jsFunctionName.c_str(), &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, jsFunctionName.c_str(), &temp_retval)) {
                return false;
            }
            if(temp_retval == JSVAL_VOID) {
                return false;
            }
            JSAutoCompartment ac(cx, obj);
            JS_CallFunctionName(cx, obj, jsFunctionName.c_str(),
                                3, dataVal, &retVal);
            return true;
        }
        return false;
    }
    
private:
    JSObject* m_pJSPickerViewDataSource;
    bool m_bNeedUnroot;
};
//TODO: js_CrossApp_CAPickerView_setDataSource
static JSBool js_CrossApp_CAPickerView_setDataSource(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CAPickerView* cobj = (CAPickerView *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        // save the delegate
        JSB_PickerViewDataSource *nativeDataSource = new JSB_PickerViewDataSource();
        nativeDataSource->setPickerViewDataSource(JSVAL_TO_OBJECT(argv[0]));
        
        CCDictionary* userDict = static_cast<CCDictionary*>(cobj->getUserObject());
        if (NULL == userDict)
        {
            userDict = new CCDictionary();
            cobj->setUserObject(userDict);
            userDict->release();
        }
        
        userDict->setObject(nativeDataSource, "PickerViewDataSource");
        
        cobj->setDataSource(nativeDataSource);
        
        nativeDataSource->release();
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}





//TODO: JSB_TableViewDataSource

class JSB_TableViewDataSource
: public CAObject
, public CATableViewDataSource
{
public:
    JSB_TableViewDataSource()
    : m_pJSTableViewDataSource(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_TableViewDataSource()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSTableViewDataSource);
        }
    }
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
    {
        jsval ret;
        bool ok = callJSDelegate(table, "tableViewHeightForRowAtIndexPath", ret);
        if (ok) {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            uint32_t size = 0;
            JSBool isSucceed = jsval_to_uint32(cx, ret, &size);
            if (isSucceed) return size;
        }
        return 0;
        
    }
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
    {
        jsval ret;
        bool ok = callJSDelegate(table, row, cellSize,section ,"tableCellAtIndex", ret);
        if (ok)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            CATableViewCell* arg0;
            do {
                js_proxy_t *proxy;
                JSObject *tmpObj = JSVAL_TO_OBJECT(ret);
                proxy = jsb_get_js_proxy(tmpObj);
                arg0 = (CATableViewCell*)(proxy ? proxy->ptr : NULL);
                JSB_PRECONDITION2( arg0, cx, NULL, "Invalid Native Object");
            } while (0);
            return arg0;
        }
        return NULL;
    }
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section)
    {
        jsval ret;
        bool ok = callJSDelegate(table, "numberOfRowsInSection", ret);
        if (ok)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            uint32_t count = 0;
            JSBool isSucceed = jsval_to_uint32(cx, ret, &count);
            if (isSucceed) return count;
        }
        return 0;
    }
    
    
    void setTableViewDataSource(JSObject* pJSSource)
    {
        m_pJSTableViewDataSource = pJSSource;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSTableViewDataSource);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSTableViewDataSource, "TableViewDataSource");
        }
    }
    
private:
    bool callJSDelegate(CATableView* table, std::string jsFunctionName, jsval& retVal)
    {
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return false;
        
        JSBool hasAction;
        jsval temp_retval;
        jsval dataVal = OBJECT_TO_JSVAL(p->obj);
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JSObject* obj = m_pJSTableViewDataSource;
        
        if (JS_HasProperty(cx, obj, jsFunctionName.c_str(), &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, jsFunctionName.c_str(), &temp_retval)) {
                return false;
            }
            if(temp_retval == JSVAL_VOID) {
                return false;
            }
            JSAutoCompartment ac(cx, obj);
            JS_CallFunctionName(cx, obj, jsFunctionName.c_str(),
                                1, &dataVal, &retVal);
            return true;
        }
        return false;
    }
    
    bool callJSDelegate(CATableView* table, int idx,  const CCSize& cellSize, int section,std::string jsFunctionName, jsval& retVal)
    {
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return false;
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JSBool hasAction;
        jsval temp_retval;
        jsval dataVal[4];
        CADipSize size = cellSize;
        dataVal[0] = OBJECT_TO_JSVAL(p->obj);
        dataVal[2] = INT_TO_JSVAL(section);
        dataVal[1] = ccsize_to_jsval(cx, size);
        dataVal[3] = INT_TO_JSVAL(idx);
        
        JSObject* obj = m_pJSTableViewDataSource;
        
        if (JS_HasProperty(cx, obj, jsFunctionName.c_str(), &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, jsFunctionName.c_str(), &temp_retval)) {
                return false;
            }
            if(temp_retval == JSVAL_VOID) {
                return false;
            }
            JSAutoCompartment ac(cx, obj);
            JS_CallFunctionName(cx, obj, jsFunctionName.c_str(),
                                4, dataVal, &retVal);
            return true;
        }
        return false;
    }
    
private:
    JSObject* m_pJSTableViewDataSource;
    bool m_bNeedUnroot;
};
//TODO: js_CrossApp_CATableView_setDataSource
static JSBool js_CrossApp_CATableView_setDataSource(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CATableView* cobj = (CATableView *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    if (argc == 1)
    {
        JSB_TableViewDataSource* pNativeSource = new JSB_TableViewDataSource();
        pNativeSource->setTableViewDataSource(JSVAL_TO_OBJECT(argv[0]));
        
        CCDictionary* userDict = static_cast<CCDictionary*>(cobj->getUserObject());
        if (NULL == userDict)
        {
            userDict = new CCDictionary();
            cobj->setUserObject(userDict);
            userDict->release();
        }
        
        userDict->setObject(pNativeSource, KEY_TABLEVIEW_DATA_SOURCE);
        
        cobj->setTableViewDataSource(pNativeSource);
        
        pNativeSource->release();
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}









//TODO:  JSB_TableViewDelegate
class JSB_ListViewDelegate
: public CAObject
, public CAListViewDelegate
{
public:
    JSB_ListViewDelegate()
    : m_pJSDelegate(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_ListViewDelegate()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSDelegate);
        }
    }
//    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
//    {
//        callJSDelegate(table, section,row,"tableViewDidSelectRowAtIndexPath");
//    };
//    
//    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
//    {
//        callJSDelegate(table, section,row,"tableViewDidDeselectRowAtIndexPath");
//    };
//    
    
    void setJSDelegate(JSObject* pJSDelegate)
    {
        m_pJSDelegate = pJSDelegate;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSDelegate);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSDelegate, "TableViewDelegate");
        }
    }
    
    
private:
    
    
    void callJSDelegate(CATableView* table, unsigned int section, unsigned int row , std::string jsFunctionName)
    {
        
        
        
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p || !m_pJSDelegate) return;
        
        jsval args[3];
        args[0] = OBJECT_TO_JSVAL(p->obj);
        args[1] = INT_TO_JSVAL(section);
        args[2] = INT_TO_JSVAL(row);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), jsFunctionName.c_str(), 3, args, NULL);
    }
    
    JSObject* m_pJSDelegate;
    bool m_bNeedUnroot;
};
//TODO:  js_CrossApp_CAListView_setDelegate
static JSBool js_CrossApp_CAListView_setDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CAListView* cobj = (CAListView *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        // save the delegate
        JSObject *jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSB_ListViewDelegate* nativeDelegate = new JSB_ListViewDelegate();
        nativeDelegate->setJSDelegate(jsDelegate);
        
        CCDictionary* userDict = static_cast<CCDictionary*>(cobj->getUserObject());
        if (NULL == userDict)
        {
            userDict = new CCDictionary();
            cobj->setUserObject(userDict);
            userDict->release();
        }
        
        userDict->setObject(nativeDelegate, KEY_TABLEVIEW_DELEGATE);
        
        cobj->setListViewDelegate(nativeDelegate);
        
        nativeDelegate->release();
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}




//TODO: JSB_ListViewDataSource

class JSB_ListViewDataSource
: public CAObject
, public CAListViewDataSource
{
public:
    JSB_ListViewDataSource()
    : m_pJSTableViewDataSource(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_ListViewDataSource()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSTableViewDataSource);
        }
    }
    
    
    virtual unsigned int numberOfIndex(CAListView *listView)
    {
        jsval ret;
        bool ok = callJSDelegate(listView, "numberOfIndex", ret);
        if (ok)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            uint32_t count = 0;
            JSBool isSucceed = jsval_to_uint32(cx, ret, &count);
            if (isSucceed) return count;
        }

        return 0;
    }
    
	virtual unsigned int listViewHeightForIndex(CAListView *listView, unsigned int index)
    {
        jsval ret;
        bool ok = callJSDelegate(listView, "listViewHeightForIndex", ret);
        if (ok) {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            uint32_t size = 0;
            JSBool isSucceed = jsval_to_uint32(cx, ret, &size);
            if (isSucceed) return size;
        }

        return 0;
    }
    
	virtual CAListViewCell* listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index)
    {
        jsval ret;
        bool ok = callJSDelegate(listView, index, cellSize,"listViewCellAtIndex", ret);
        if (ok)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            CAListViewCell* arg0;
            do {
                js_proxy_t *proxy;
                JSObject *tmpObj = JSVAL_TO_OBJECT(ret);
                proxy = jsb_get_js_proxy(tmpObj);
                arg0 = (CAListViewCell*)(proxy ? proxy->ptr : NULL);
                JSB_PRECONDITION2( arg0, cx, NULL, "Invalid Native Object");
            } while (0);
            return arg0;
        }

        return NULL;
    }
    void setTableViewDataSource(JSObject* pJSSource)
    {
        m_pJSTableViewDataSource = pJSSource;
        
        // Check whether the js delegate is a pure js object.
        js_proxy_t* p = jsb_get_js_proxy(m_pJSTableViewDataSource);
        if (!p)
        {
            m_bNeedUnroot = true;
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &m_pJSTableViewDataSource, "TableViewDataSource");
        }
    }
    
private:
    bool callJSDelegate(CAListView* table, std::string jsFunctionName, jsval& retVal)
    {
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return false;
        
        JSBool hasAction;
        jsval temp_retval;
        jsval dataVal = OBJECT_TO_JSVAL(p->obj);
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JSObject* obj = m_pJSTableViewDataSource;
        
        if (JS_HasProperty(cx, obj, jsFunctionName.c_str(), &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, jsFunctionName.c_str(), &temp_retval)) {
                return false;
            }
            if(temp_retval == JSVAL_VOID) {
                return false;
            }
            JSAutoCompartment ac(cx, obj);
            JS_CallFunctionName(cx, obj, jsFunctionName.c_str(),
                                1, &dataVal, &retVal);
            return true;
        }
        return false;
    }
    
    bool callJSDelegate(CAListView* table, int idx,  const CCSize& cellSize,std::string jsFunctionName, jsval& retVal)
    {
        js_proxy_t * p = jsb_get_native_proxy(table);
        if (!p) return false;
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JSBool hasAction;
        jsval temp_retval;
        jsval dataVal[3];
        CADipSize size = cellSize;
        dataVal[0] = OBJECT_TO_JSVAL(p->obj);
        dataVal[1] = ccsize_to_jsval(cx, size);
        dataVal[2] = INT_TO_JSVAL(idx);
        
        JSObject* obj = m_pJSTableViewDataSource;
        
        if (JS_HasProperty(cx, obj, jsFunctionName.c_str(), &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, jsFunctionName.c_str(), &temp_retval)) {
                return false;
            }
            if(temp_retval == JSVAL_VOID) {
                return false;
            }
            JSAutoCompartment ac(cx, obj);
            JS_CallFunctionName(cx, obj, jsFunctionName.c_str(),
                                3, dataVal, &retVal);
            return true;
        }
        return false;
    }
    
private:
    JSObject* m_pJSTableViewDataSource;
    bool m_bNeedUnroot;
};
//TODO: js_CrossApp_CAListView_setDataSource
static JSBool js_CrossApp_CAListView_setDataSource(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CAListView* cobj = (CAListView *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    if (argc == 1)
    {
        JSB_ListViewDataSource* pNativeSource = new JSB_ListViewDataSource();
        pNativeSource->setTableViewDataSource(JSVAL_TO_OBJECT(argv[0]));
        
        CCDictionary* userDict = static_cast<CCDictionary*>(cobj->getUserObject());
        if (NULL == userDict)
        {
            userDict = new CCDictionary();
            cobj->setUserObject(userDict);
            userDict->release();
        }
        
        userDict->setObject(pNativeSource, KEY_TABLEVIEW_DATA_SOURCE);
        
        cobj->setListViewDataSource(pNativeSource);
        
        pNativeSource->release();
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}
//TODO: js_CrossApp_CATableView_create
static JSBool js_CrossApp_CATableView_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 3 || argc == 2) {
        
        JSB_TableViewDataSource* pNativeSource = new JSB_TableViewDataSource();
        pNativeSource->setTableViewDataSource(JSVAL_TO_OBJECT(argv[0]));
        
        CADipSize arg1;
        ok &= jsval_to_ccsize(cx, argv[1], &arg1);
        CATableView* ret = NULL;
        ret = new CATableView();
        ret->autorelease();
        
        ret->setTableViewDataSource(pNativeSource);
        
        jsval jsret;
        do {
            if (ret) {
                js_proxy_t *proxy = js_get_or_create_proxy<CATableView>(cx, ret);
                jsret = OBJECT_TO_JSVAL(proxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        
        if (argc == 2)
        {
            ret->initWithFrame(CCRectMake(0, 0, arg1.width, arg1.height));
            //initWithViewSize(arg1);
        }
        else
        {
            ret->initWithFrame(CCRectMake(0, 0, arg1.width, arg1.height));
        }
        ret->reloadData();
        
        CCDictionary* userDict = new CCDictionary();
        userDict->setObject(pNativeSource, KEY_TABLEVIEW_DATA_SOURCE);
        ret->setUserObject(userDict);
        userDict->release();
        
        pNativeSource->release();
        
        JS_SET_RVAL(cx, vp, jsret);
        return JS_TRUE;
    }
    
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}
//TODO: JSB_ControlButtonTarget
class JSB_ControlButtonTarget : public CAObject
{
public:
    JSB_ControlButtonTarget()
    : _jsFunc(NULL),
    _type(CAControlEventTouchDown),
    _jsTarget(NULL),
    _needUnroot(false)
    {}
    
    virtual ~JSB_ControlButtonTarget()
    {
        CCLOGINFO("In the destruction of JSB_ControlButtonTarget ...");
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        if (_needUnroot)
        {
            JS_RemoveObjectRoot(cx, &_jsTarget);
        }
        
        JS_RemoveObjectRoot(cx, &_jsFunc);
        
        for (std::multimap<JSObject*, JSB_ControlButtonTarget*>::iterator iter = _jsNativeTargetMap.begin(); iter != _jsNativeTargetMap.end(); ++iter)
        {
            if (this == iter->second)
            {
                _jsNativeTargetMap.erase(iter);
                break;
            }
        }
    }
    
    virtual void onEvent(CAButton *controlButton, CADipPoint pos)
    {
        js_proxy_t * p;
        JS_GET_PROXY(p, controlButton);
        if (!p)
        {
            CCLOGERROR("Failed to get proxy for control button");
            return;
        }
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        jsval dataVal[2];
        dataVal[0] = OBJECT_TO_JSVAL(p->obj);
		dataVal[1] = ccpoint_to_jsval(cx,pos);
        jsval jsRet;
        
        ScriptingCore::getInstance()->executeJSFunctionWithThisObj(OBJECT_TO_JSVAL(_jsTarget), OBJECT_TO_JSVAL(_jsFunc), 2,dataVal,&jsRet);
    }
    
    void setJSTarget(JSObject* pJSTarget)
    {
        _jsTarget = pJSTarget;
        
        js_proxy_t* p = jsb_get_js_proxy(_jsTarget);
        if (!p)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &_jsTarget, "JSB_ControlButtonTarget, target");
            _needUnroot = true;
        }
    }
    
    void setJSAction(JSObject* jsFunc)
    {
        _jsFunc = jsFunc;
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JS_AddNamedObjectRoot(cx, &_jsFunc, "JSB_ControlButtonTarget, func");
    }
    
    void setEventType(CAControlEvents type)
    {
        _type = type;
    }
public:
    
    static std::multimap<JSObject*, JSB_ControlButtonTarget*> _jsNativeTargetMap;
    JSObject* _jsFunc;
    CAControlEvents _type;
private:
    JSObject* _jsTarget;
    bool _needUnroot;
};

std::multimap<JSObject*, JSB_ControlButtonTarget*> JSB_ControlButtonTarget::_jsNativeTargetMap;
//TODO: js_CrossApp_CAButton_addTarget
static JSBool js_CrossApp_CAButton_addTarget(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CAButton* cobj = (CAButton *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    JSBool ok = JS_TRUE;
    if (argc == 3)
    {
        JSObject* jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSObject* jsFunc = JSVAL_TO_OBJECT(argv[1]);
        CAControlEvents arg2;
        ok &= jsval_to_int32(cx, argv[2], (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing control event");
        
        // Check whether the target already exists.
        std::pair<std::multimap<JSObject*, JSB_ControlButtonTarget*>::iterator,std::multimap<JSObject*, JSB_ControlButtonTarget*>::iterator> range;
        range = JSB_ControlButtonTarget::_jsNativeTargetMap.equal_range(jsDelegate);
        std::multimap<JSObject*, JSB_ControlButtonTarget*>::iterator it = range.first;
        for (; it != range.second; ++it)
        {
            if (it->second->_jsFunc == jsFunc && arg2 == it->second->_type)
            {
                // Return true directly.
                JS_SET_RVAL(cx, vp, JSVAL_VOID);
                return JS_TRUE;
            }
        }
        
        // save the delegate
        JSB_ControlButtonTarget* nativeDelegate = new JSB_ControlButtonTarget();
        
        nativeDelegate->setJSTarget(jsDelegate);
        nativeDelegate->setJSAction(jsFunc);
        nativeDelegate->setEventType(arg2);
        
        CCArray* nativeDelegateArray = static_cast<CCArray*>(cobj->getUserObject());
        if (NULL == nativeDelegateArray)
        {
            nativeDelegateArray = new CCArray();
            nativeDelegateArray->init();
            cobj->setUserObject(nativeDelegateArray);  // The reference of nativeDelegateArray is added to 2
            nativeDelegateArray->release(); // Release nativeDelegateArray to make the reference to 1
        }
        
        nativeDelegateArray->addObject(nativeDelegate); // The reference of nativeDelegate is added to 2
        nativeDelegate->release(); // Release nativeDelegate to make the reference to 1
        
        cobj->addTarget(nativeDelegate, CAControl_selector(JSB_ControlButtonTarget::onEvent), arg2);
        
        JSB_ControlButtonTarget::_jsNativeTargetMap.insert(std::make_pair(jsDelegate, nativeDelegate));
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 3);
    return JS_FALSE;
}

//TODO: JSB_ControlSliderTarget
class JSB_ControlSliderTarget : public CAObject
{
public:
    JSB_ControlSliderTarget()
    : _jsFunc(NULL),
    _type(CAControlEventTouchDown),
    _jsTarget(NULL),
    _needUnroot(false)
    {}
    
    virtual ~JSB_ControlSliderTarget()
    {
        CCLOGINFO("In the destruction of JSB_ControlButtonTarget ...");
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        if (_needUnroot)
        {
            JS_RemoveObjectRoot(cx, &_jsTarget);
        }
        
        JS_RemoveObjectRoot(cx, &_jsFunc);
        
        for (std::multimap<JSObject*, JSB_ControlSliderTarget*>::iterator iter = _jsNativeTargetMap.begin(); iter != _jsNativeTargetMap.end(); ++iter)
        {
            if (this == iter->second)
            {
                _jsNativeTargetMap.erase(iter);
                break;
            }
        }
    }
    
     void onEvent(CASlider *sender, CADipPoint pos)
    {
        js_proxy_t * p;
		JS_GET_PROXY(p, sender);
        if (!p)
        {
            CCLOGERROR("Failed to get proxy for control button");
            return;
        }
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        jsval dataVal[2];
		dataVal[0] = OBJECT_TO_JSVAL(p->obj);
		dataVal[1] = ccpoint_to_jsval(cx, pos);
        jsval jsRet;
        
        ScriptingCore::getInstance()->executeJSFunctionWithThisObj(OBJECT_TO_JSVAL(_jsTarget), OBJECT_TO_JSVAL(_jsFunc), 2,dataVal,&jsRet);
    }
    
    void setJSTarget(JSObject* pJSTarget)
    {
        _jsTarget = pJSTarget;
        
        js_proxy_t* p = jsb_get_js_proxy(_jsTarget);
        if (!p)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &_jsTarget, "JSB_ControlSliderTarget, target");
            _needUnroot = true;
        }
    }
    
    void setJSAction(JSObject* jsFunc)
    {
        _jsFunc = jsFunc;
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JS_AddNamedObjectRoot(cx, &_jsFunc, "JSB_ControlSliderTarget, func");
    }
    
    void setEventType(CAControlEvents type)
    {
        _type = type;
    }
public:
    
    static std::multimap<JSObject*, JSB_ControlSliderTarget*> _jsNativeTargetMap;
    JSObject* _jsFunc;
    CAControlEvents _type;
private:
    JSObject* _jsTarget;
    bool _needUnroot;
};

std::multimap<JSObject*, JSB_ControlSliderTarget*> JSB_ControlSliderTarget::_jsNativeTargetMap;

static JSBool js_CrossApp_CASlider_addTarget(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CASlider* cobj = (CASlider *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
//    JSBool ok = JS_TRUE;
    if (argc == 2)
    {
        JSObject* jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSObject* jsFunc = JSVAL_TO_OBJECT(argv[1]);
		CAControlEvents arg2 = CAControlEventTouchDown;
//        ok &= jsval_to_int32(cx, argv[2], (int32_t *)&arg2);
//        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing control event");
        
        // Check whether the target already exists.
        std::pair<std::multimap<JSObject*, JSB_ControlSliderTarget*>::iterator,std::multimap<JSObject*, JSB_ControlSliderTarget*>::iterator> range;
        range = JSB_ControlSliderTarget::_jsNativeTargetMap.equal_range(jsDelegate);
        std::multimap<JSObject*, JSB_ControlSliderTarget*>::iterator it = range.first;
        for (; it != range.second; ++it)
        {
            if (it->second->_jsFunc == jsFunc && arg2 == it->second->_type)
            {
                // Return true directly.
                JS_SET_RVAL(cx, vp, JSVAL_VOID);
                return JS_TRUE;
            }
        }
        
        // save the delegate
        JSB_ControlSliderTarget* nativeDelegate = new JSB_ControlSliderTarget();
        
        nativeDelegate->setJSTarget(jsDelegate);
        nativeDelegate->setJSAction(jsFunc);
        nativeDelegate->setEventType(arg2);
        
        CCArray* nativeDelegateArray = static_cast<CCArray*>(cobj->getUserObject());
        if (NULL == nativeDelegateArray)
        {
            nativeDelegateArray = new CCArray();
            nativeDelegateArray->init();
            cobj->setUserObject(nativeDelegateArray);  // The reference of nativeDelegateArray is added to 2
            nativeDelegateArray->release(); // Release nativeDelegateArray to make the reference to 1
        }
        
        nativeDelegateArray->addObject(nativeDelegate); // The reference of nativeDelegate is added to 2
		cobj->addTarget(nativeDelegate, CAControl_selector(JSB_ControlSliderTarget::onEvent));
		
		
		nativeDelegate->release();
        JSB_ControlSliderTarget::_jsNativeTargetMap.insert(std::make_pair(jsDelegate, nativeDelegate));
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 3);
    return JS_FALSE;
}

class JSB_ControlSwitchTarget : public CAObject
{
public:
    JSB_ControlSwitchTarget()
    : _jsFunc(NULL),
    _type(CAControlEventTouchDown),
    _jsTarget(NULL),
    _needUnroot(false)
    {}
    
    virtual ~JSB_ControlSwitchTarget()
    {
        CCLOGINFO("In the destruction of JSB_ControlSwitchTarget ...");
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        if (_needUnroot)
        {
            JS_RemoveObjectRoot(cx, &_jsTarget);
        }
        
        JS_RemoveObjectRoot(cx, &_jsFunc);
        
        for (std::multimap<JSObject*, JSB_ControlSwitchTarget*>::iterator iter = _jsNativeTargetMap.begin(); iter != _jsNativeTargetMap.end(); ++iter)
        {
            if (this == iter->second)
            {
                _jsNativeTargetMap.erase(iter);
                break;
            }
        }
    }
    
    virtual void onEvent(CASwitch *controlButton,CADipPoint pos)
    {
        js_proxy_t * p;
        JS_GET_PROXY(p, controlButton);
        if (!p)
        {
            CCLOGERROR("Failed to get proxy for control button");
            return;
        }
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        jsval dataVal[2];
        dataVal[0] = OBJECT_TO_JSVAL(p->obj);
		dataVal[1] = ccpoint_to_jsval(cx,pos);
        jsval jsRet;
        
        ScriptingCore::getInstance()->executeJSFunctionWithThisObj(OBJECT_TO_JSVAL(_jsTarget), OBJECT_TO_JSVAL(_jsFunc), 2,dataVal,&jsRet);
    }
    
    void setJSTarget(JSObject* pJSTarget)
    {
        _jsTarget = pJSTarget;
        
        js_proxy_t* p = jsb_get_js_proxy(_jsTarget);
        if (!p)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_AddNamedObjectRoot(cx, &_jsTarget, "JSB_ControlSwitchTarget");
            _needUnroot = true;
        }
    }
    
    void setJSAction(JSObject* jsFunc)
    {
        _jsFunc = jsFunc;
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JS_AddNamedObjectRoot(cx, &_jsFunc, "JSB_ControlSwitchTarget");
    }
    
    void setEventType(CAControlEvents type)
    {
        _type = type;
    }
public:
    
    static std::multimap<JSObject*, JSB_ControlSwitchTarget*> _jsNativeTargetMap;
    JSObject* _jsFunc;
    CAControlEvents _type;
private:
    JSObject* _jsTarget;
    bool _needUnroot;
};

std::multimap<JSObject*, JSB_ControlSwitchTarget*> JSB_ControlSwitchTarget::_jsNativeTargetMap;

static JSBool js_CrossApp_CASwitch_addTarget(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CASwitch* cobj = (CASwitch *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    //    JSBool ok = JS_TRUE;
    if (argc == 2)
    {
        JSObject* jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSObject* jsFunc = JSVAL_TO_OBJECT(argv[1]);
		CAControlEvents arg2 = CAControlEventTouchUpInSide;
        
        std::pair<std::multimap<JSObject*, JSB_ControlSwitchTarget*>::iterator,std::multimap<JSObject*, JSB_ControlSwitchTarget*>::iterator> range;
        range = JSB_ControlSwitchTarget::_jsNativeTargetMap.equal_range(jsDelegate);
        std::multimap<JSObject*, JSB_ControlSwitchTarget*>::iterator it = range.first;
        for (; it != range.second; ++it)
        {
            if (it->second->_jsFunc == jsFunc && arg2 == it->second->_type)
            {
                // Return true directly.
                JS_SET_RVAL(cx, vp, JSVAL_VOID);
                return JS_TRUE;
            }
        }
        
        // save the delegate
        JSB_ControlSwitchTarget* nativeDelegate = new JSB_ControlSwitchTarget();
        
        nativeDelegate->setJSTarget(jsDelegate);
        nativeDelegate->setJSAction(jsFunc);
        nativeDelegate->setEventType(arg2);
        
        CCArray* nativeDelegateArray = static_cast<CCArray*>(cobj->getUserObject());
        if (NULL == nativeDelegateArray)
        {
            nativeDelegateArray = new CCArray();
            nativeDelegateArray->init();
            cobj->setUserObject(nativeDelegateArray);  // The reference of nativeDelegateArray is added to 2
            nativeDelegateArray->release(); // Release nativeDelegateArray to make the reference to 1
        }
        
        nativeDelegateArray->addObject(nativeDelegate); // The reference of nativeDelegate is added to 2
         // Release nativeDelegate to make the reference to 1
        
		cobj->addTarget(nativeDelegate, CAControl_selector(JSB_ControlSwitchTarget::onEvent));
        
        JSB_ControlSwitchTarget::_jsNativeTargetMap.insert(std::make_pair(jsDelegate, nativeDelegate));
		nativeDelegate->release();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 3);
    return JS_FALSE;
}



//TODO:JSB_CAViewDelegate
class JSB_CAViewDelegate
:public CAObject
,public CAViewDelegate
{
public:
    JSB_CAViewDelegate()
    : m_pJSDelegate(NULL)
    , m_bNeedUnroot(false)
    {}
    
    virtual ~JSB_CAViewDelegate()
    {
        if (m_bNeedUnroot)
        {
            JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
            JS_RemoveObjectRoot(cx, &m_pJSDelegate);
        }
    }
    
    virtual void getSuperViewRect(const CCRect& rect)
    {
        callJSDelegate(rect,"getSuperViewRect");
    }
    virtual void viewOnEnterTransitionDidFinish()
    {
        callJSDelegate("viewOnEnterTransitionDidFinish");
    }
    virtual void viewOnExitTransitionDidStart()
    {
        callJSDelegate("viewOnExitTransitionDidStart");
    }

    
private:
    JSObject* m_pJSDelegate;
    bool m_bNeedUnroot;
    void callJSDelegate(std::string jsFunctionName)
    {
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), jsFunctionName.c_str());
    }
    void callJSDelegate(CADipRect rect,std::string jsFunctionName)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        jsval arg = ccrect_to_jsval(cx, rect);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), jsFunctionName.c_str(), 1, &arg, NULL);
    }
};

template<class T>
JSObject* bind_menu_item(JSContext *cx, T* nativeObj, jsval callback, jsval thisObj) {    
	js_proxy_t *p = jsb_get_native_proxy(nativeObj);
	if (p) {
		addCallBackAndThis(p->obj, callback, thisObj);
		return p->obj;
	} else {
		js_type_class_t *classType = js_get_type_from_native<T>(nativeObj);
		assert(classType);
		JSObject *tmp = JS_NewObject(cx, classType->jsclass, classType->proto, classType->parentProto);

		// bind nativeObj <-> JSObject
		js_proxy_t *proxy = jsb_new_proxy(nativeObj, tmp);
		JS_AddNamedObjectRoot(cx, &proxy->obj, typeid(*nativeObj).name());        
		addCallBackAndThis(tmp, callback, thisObj);

		return tmp;
	}
}


JSBool js_CrossApp_CCSequence_create(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	if (argc > 0) {
		CCArray* array = CCArray::create();
		uint32_t i = 0;
		while (i < argc) {
			js_proxy_t *proxy;
			JSObject *tmpObj = JSVAL_TO_OBJECT(argv[i]);
			proxy = jsb_get_js_proxy(tmpObj);
			CAObject *item = (CAObject*)(proxy ? proxy->ptr : NULL);
			TEST_NATIVE_OBJECT(cx, item)
			array->addObject(item);
			i++;
		}
		CCFiniteTimeAction* ret = CCSequence::create(array);
		jsval jsret;
		do {
			if (ret) {
				js_proxy_t *p = jsb_get_native_proxy(ret);
				if (p) {
					jsret = OBJECT_TO_JSVAL(p->obj);
				} else {
					// create a new js obj of that class
					js_proxy_t *proxy = js_get_or_create_proxy<CCFiniteTimeAction>(cx, ret);
					jsret = OBJECT_TO_JSVAL(proxy->obj);
				}
			} else {
				jsret = JSVAL_NULL;
			}
		} while (0);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}
    JS_ReportError(cx, "wrong number of arguments");
	return JS_FALSE;
}

JSBool js_CrossApp_CCSpawn_create(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	if (argc > 0) {
		CCArray* array = CCArray::create();
		uint32_t i = 0;
		while (i < argc) {
			js_proxy_t *proxy;
			JSObject *tmpObj = JSVAL_TO_OBJECT(argv[i]);
			proxy = jsb_get_js_proxy(tmpObj);
			CAObject *item = (CAObject*)(proxy ? proxy->ptr : NULL);
			TEST_NATIVE_OBJECT(cx, item)
			array->addObject(item);
			i++;
		}
		CCFiniteTimeAction* ret = CCSpawn::create(array);
		jsval jsret;
		do {
			if (ret) {
				js_proxy_t *p = jsb_get_native_proxy(ret);
				if (p) {
					jsret = OBJECT_TO_JSVAL(p->obj);
				} else {
					// create a new js obj of that class
					js_proxy_t *proxy = js_get_or_create_proxy<CCFiniteTimeAction>(cx, ret);
					jsret = OBJECT_TO_JSVAL(proxy->obj);
				}
			} else {
				jsret = JSVAL_NULL;
			}
		} while (0);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}
    JS_ReportError(cx, "wrong number of arguments");
	return JS_FALSE;
}

template<class T>
JSBool js_CrossApp_setCallback(JSContext *cx, uint32_t argc, jsval *vp) {

    if(argc == 1 || argc == 2) {
        jsval *argv = JS_ARGV(cx, vp);
        JSObject *obj = JS_THIS_OBJECT(cx, vp);
        jsval jsThis = JSVAL_VOID;
        jsval jsFunc = argv[0];

        if (jsFunc.isUndefined())
        {
            JS_ReportError(cx, "The callback function is undefined.");
            return JS_FALSE;
        }

        if (argc == 2) {
            jsThis = argv[1];
        }
        
        js_proxy_t *proxy = jsb_get_js_proxy(obj);
        T* item = (T*)(proxy ? proxy->ptr : NULL);
        TEST_NATIVE_OBJECT(cx, item)
        bind_menu_item(cx, item, jsFunc, jsThis);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d or %d", argc, 1, 2);
    return JS_FALSE;
}



//JSBool js_CrossApp_CCAnimation_create(JSContext *cx, uint32_t argc, jsval *vp)
//{
//    JSBool ok = JS_TRUE;
//	jsval *argv = JS_ARGV(cx, vp);
//	if (argc <= 3) {
//		CCArray* arg0;
////		if (argc > 0) {
////			ok &= jsval_to_ccarray(cx, argv[0], &arg0);
////            JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
////		}
////		CAAnimation* ret;
////		double arg1 = 0.0f;
////		if (argc > 0 && argc == 2) {
////			if (argc == 2) {
////				ok &= JS_ValueToNumber(cx, argv[1], &arg1);
////                JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
////			}
////			ret = CAAnimation::createWithSpriteFrames(arg0, arg1);
////		} else if (argc == 3) {
////			unsigned int loops;
////			ok &= JS_ValueToNumber(cx, argv[1], &arg1);
////			ok &= jsval_to_uint32(cx, argv[2], &loops);
////            JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
////			ret = CrossApp::CAAnimation::create(arg0, arg1, loops);
////		} else if (argc == 1) {
////			ret = CrossApp::CAAnimation::createWithSpriteFrames(arg0);
////		} else if (argc == 0) {
////            ret = CrossApp::CAAnimation::create();
////        }
////		jsval jsret;
////		if (ret) {
////			js_proxy_t *proxy = jsb_get_native_proxy(ret);
////			if (proxy) {
////				jsret = OBJECT_TO_JSVAL(proxy->obj);
////			} else {
////				// create a new js obj of that class
////				proxy = js_get_or_create_proxy<CrossApp::CAAnimation>(cx, ret);
////				jsret = OBJECT_TO_JSVAL(proxy->obj);
////			}
////		} else {
////			jsret = JSVAL_NULL;
////		}
////		JS_SET_RVAL(cx, vp, jsret);
//		return JS_TRUE;
//	}
//    JS_ReportError(cx, "wrong number of arguments");
//	return JS_FALSE;
//}



JSBool js_CrossApp_swap_native_object(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 2) {
		// get the native object from the second object to the first object
		jsval *argv = JS_ARGV(cx, vp);
		JSObject *one = JSVAL_TO_OBJECT(argv[0]);
		JSObject *two = JSVAL_TO_OBJECT(argv[1]);
		js_proxy_t *jsproxy = jsb_get_js_proxy(two);
		void *ptrTwo = (jsproxy ? jsproxy->ptr : NULL);
		if (jsproxy) {
			js_proxy_t *nproxy = jsb_get_native_proxy(ptrTwo);
			if (nproxy) {
                JS_RemoveObjectRoot(cx, &jsproxy->obj);
				jsb_remove_proxy(nproxy, jsproxy);
                jsproxy = jsb_new_proxy(ptrTwo, one);
                JS_AddNamedObjectRoot(cx, &jsproxy->obj, typeid(*((CAObject*)jsproxy->ptr)).name());
			}
		}
	}
	return JS_TRUE;
}


jsval anonEvaluate(JSContext *cx, JSObject *thisObj, const char* string) {
	jsval out;
	if (JS_EvaluateScript(cx, thisObj, string, strlen(string), "(string)", 1, &out) == JS_TRUE) {
		return out;
	}
	return JSVAL_VOID;
}

JSBool js_platform(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSString *str = JS_NewStringCopyZ(cx, "mobile");
	jsval out = STRING_TO_JSVAL(str);
	JS_SET_RVAL(cx, vp, out);
	return JS_TRUE;
}

JSCallbackWrapper::JSCallbackWrapper()
: jsCallback(JSVAL_VOID), jsThisObj(JSVAL_VOID), extraData(JSVAL_VOID)
{

}

JSCallbackWrapper::~JSCallbackWrapper()
{
    JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
    JS_RemoveValueRoot(cx, &jsCallback);
}

void JSCallbackWrapper::setJSCallbackFunc(jsval func) {
    jsCallback = func;
    JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	// Root the callback function.
    JS_AddNamedValueRoot(cx, &jsCallback, "JSCallbackWrapper_callback_func");
}

void JSCallbackWrapper::setJSCallbackThis(jsval thisObj) {
    jsThisObj = thisObj;
}

void JSCallbackWrapper::setJSExtraData(jsval data) {
    extraData = data;
}

const jsval& JSCallbackWrapper::getJSCallbackFunc() const
{
    return jsCallback;
}

const jsval& JSCallbackWrapper::getJSCallbackThis() const
{
    return jsThisObj;
}

const jsval& JSCallbackWrapper::getJSExtraData() const
{
    return extraData;
}

JSBool js_callFunc(JSContext *cx, uint32_t argc, jsval *vp)
{
    

    return JS_FALSE;
}

JSScheduleWrapper::~JSScheduleWrapper()
{
    if (_pPureJSTarget) {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        JS_RemoveObjectRoot(cx, &_pPureJSTarget);
    }
}

void JSScheduleWrapper::setTargetForSchedule(jsval sched, JSScheduleWrapper *target) {
    do {
        JSObject* jsfunc = JSVAL_TO_OBJECT(sched);
        CCArray* targetArray = getTargetForSchedule(sched);
        if (NULL == targetArray) {
            targetArray = new CCArray();
            schedFunc_proxy_t *p = (schedFunc_proxy_t *)malloc(sizeof(schedFunc_proxy_t));
            assert(p);
            p->jsfuncObj = jsfunc;
            p->targets = targetArray;
            HASH_ADD_PTR(_schedFunc_target_ht, jsfuncObj, p);
        }

        CCAssert(!targetArray->containsObject(target), "The target was already added.");

        targetArray->addObject(target);
    } while(0);
}

CCArray * JSScheduleWrapper::getTargetForSchedule(jsval sched) {
    schedFunc_proxy_t *t = NULL;
    JSObject *o = JSVAL_TO_OBJECT(sched);
    HASH_FIND_PTR(_schedFunc_target_ht, &o, t);
    return t != NULL ? t->targets : NULL;
}


void JSScheduleWrapper::setTargetForJSObject(JSObject* jsTargetObj, JSScheduleWrapper *target)
{
    CCArray* targetArray = getTargetForJSObject(jsTargetObj);
    if (NULL == targetArray) {
        targetArray = new CCArray();
        schedTarget_proxy_t *p = (schedTarget_proxy_t *)malloc(sizeof(schedTarget_proxy_t));
        assert(p);
        p->jsTargetObj = jsTargetObj;
        p->targets = targetArray;
        HASH_ADD_PTR(_schedObj_target_ht, jsTargetObj, p);
    }
    
    CCAssert(!targetArray->containsObject(target), "The target was already added.");
    targetArray->addObject(target);
}

CCArray * JSScheduleWrapper::getTargetForJSObject(JSObject* jsTargetObj)
{
    schedTarget_proxy_t *t = NULL;
    HASH_FIND_PTR(_schedObj_target_ht, &jsTargetObj, t);
    return t != NULL ? t->targets : NULL;
}

void JSScheduleWrapper::removeAllTargets()
{
    CCLOGINFO("removeAllTargets begin");
    dump();
    
    {
        schedFunc_proxy_t *current, *tmp;
        HASH_ITER(hh, _schedFunc_target_ht, current, tmp) {
            current->targets->removeAllObjects();
            current->targets->release();
            HASH_DEL(_schedFunc_target_ht, current);
            free(current);
        }
    }
    
    {
        schedTarget_proxy_t *current, *tmp;
        HASH_ITER(hh, _schedObj_target_ht, current, tmp) {
            current->targets->removeAllObjects();
            current->targets->release();
            HASH_DEL(_schedObj_target_ht, current);
            free(current);
        }
    }
    
    dump();
    CCLOGINFO("removeAllTargets end");
}

void JSScheduleWrapper::removeAllTargetsForMinPriority(int minPriority)
{
    CCLOGINFO("removeAllTargetsForPriority begin");
    dump();
    
    {
        schedFunc_proxy_t *current, *tmp;
        HASH_ITER(hh, _schedFunc_target_ht, current, tmp) {
            std::vector<CAObject*> objectsNeedToBeReleased;
            CCArray* targets = current->targets;
            CAObject* pObj = NULL;
            CCARRAY_FOREACH(targets, pObj)
            {
                JSScheduleWrapper* wrapper = (JSScheduleWrapper*)pObj;
                bool isUpdateSchedule = wrapper->isUpdateSchedule();
                if (!isUpdateSchedule || (isUpdateSchedule && wrapper->getPriority() >= minPriority))
                {
                    objectsNeedToBeReleased.push_back(pObj);
                }
            }
            
            std::vector<CAObject*>::iterator iter = objectsNeedToBeReleased.begin();
            for (; iter != objectsNeedToBeReleased.end(); ++iter)
            {
                targets->removeObject(*iter, true);
            }
            
            if (targets->count() == 0)
            {
                HASH_DEL(_schedFunc_target_ht, current);
                targets->release();
                free(current);
            }
        }
    }
    
    {
        schedTarget_proxy_t *current, *tmp;
        HASH_ITER(hh, _schedObj_target_ht, current, tmp) {
            std::vector<CAObject*> objectsNeedToBeReleased;
            CCArray* targets = current->targets;
            CAObject* pObj = NULL;
            CCARRAY_FOREACH(targets, pObj)
            {
                JSScheduleWrapper* wrapper = (JSScheduleWrapper*)pObj;
                bool isUpdateSchedule = wrapper->isUpdateSchedule();
                if (!isUpdateSchedule || (isUpdateSchedule && wrapper->getPriority() >= minPriority))
                {
                    objectsNeedToBeReleased.push_back(pObj);
                }
            }
            
            std::vector<CAObject*>::iterator iter = objectsNeedToBeReleased.begin();
            for (; iter != objectsNeedToBeReleased.end(); ++iter)
            {
                targets->removeObject(*iter, true);
            }
            
            if (targets->count() == 0)
            {
                HASH_DEL(_schedObj_target_ht, current);
                targets->release();
                free(current);
            }
        }
    }
    
    dump();
    CCLOGINFO("removeAllTargetsForPriority end");
}

void JSScheduleWrapper::removeAllTargetsForJSObject(JSObject* jsTargetObj)
{
    CCLOGINFO("removeAllTargetsForNatiaveNode begin");
    dump();
    CCArray* removeNativeTargets = NULL;
    schedTarget_proxy_t *t = NULL;
    HASH_FIND_PTR(_schedObj_target_ht, &jsTargetObj, t);
    if (t != NULL) {
        removeNativeTargets = t->targets;
        HASH_DEL(_schedObj_target_ht, t);
    }

    if (removeNativeTargets == NULL) return;

    schedFunc_proxy_t *current, *tmp;
    HASH_ITER(hh, _schedFunc_target_ht, current, tmp) {
        std::vector<CAObject*> objectsNeedToBeReleased;
        CCArray* targets = current->targets;
        CAObject* pObj = NULL;
        CCARRAY_FOREACH(targets, pObj)
        {
            if (removeNativeTargets->containsObject(pObj))
            {
                objectsNeedToBeReleased.push_back(pObj);
            }
        }
        
        std::vector<CAObject*>::iterator iter = objectsNeedToBeReleased.begin();
        for (; iter != objectsNeedToBeReleased.end(); ++iter)
        {
            targets->removeObject(*iter, true);
        }

        if (targets->count() == 0)
        {
            HASH_DEL(_schedFunc_target_ht, current);
            targets->release();
            free(current);
        }  
    }

    removeNativeTargets->removeAllObjects();
    removeNativeTargets->release();
    free(t);
    dump();
    CCLOGINFO("removeAllTargetsForNatiaveNode end");
}

void JSScheduleWrapper::removeTargetForJSObject(JSObject* jsTargetObj, JSScheduleWrapper* target)
{
    CCLOGINFO("removeTargetForJSObject begin");
    dump();
    schedTarget_proxy_t *t = NULL;
    HASH_FIND_PTR(_schedObj_target_ht, &jsTargetObj, t);
    if (t != NULL) {
        t->targets->removeObject(target);
        if (t->targets->count() == 0)
        {
            t->targets->release();
            HASH_DEL(_schedObj_target_ht, t);
            free(t);
        }
    }

    schedFunc_proxy_t *current, *tmp, *removed=NULL;

    HASH_ITER(hh, _schedFunc_target_ht, current, tmp) {
        CCArray* targets = current->targets;
        CAObject* pObj = NULL;
        
        CCARRAY_FOREACH(targets, pObj)
        {
            JSScheduleWrapper* pOneTarget = (JSScheduleWrapper*)pObj;
            if (pOneTarget == target)
            {
                removed = current;
                break;
            }
        }
        if (removed) break;
    }

    if (removed)
    {
        removed->targets->removeObject(target);
        if (removed->targets->count() == 0)
        {
            removed->targets->release();
            HASH_DEL(_schedFunc_target_ht, removed);
            free(removed);
        }  
    }
    dump();
    CCLOGINFO("removeTargetForJSObject end");
}

void JSScheduleWrapper::dump()
{
#if CrossApp_DEBUG > 1
    CCLOG("\n---------JSScheduleWrapper dump begin--------------\n");
    CCLOG("target hash count = %d, func hash count = %d", HASH_COUNT(_schedObj_target_ht), HASH_COUNT(_schedFunc_target_ht));
    schedTarget_proxy_t *current, *tmp;
    int nativeTargetsCount = 0;
    HASH_ITER(hh, _schedObj_target_ht, current, tmp) {
        CAObject* pObj = NULL;
        CCARRAY_FOREACH(current->targets, pObj)
        {
            CCLOG("js target ( %p ), native target[%d]=( %p )", current->jsTargetObj, nativeTargetsCount, pObj);
            nativeTargetsCount++;
        }
    }

    CCLOG("\n-----------------------------\n");

    schedFunc_proxy_t *current_func, *tmp_func;
    int jsfuncTargetCount = 0;
    HASH_ITER(hh, _schedFunc_target_ht, current_func, tmp_func) {
        CAObject* pObj = NULL;
        CCARRAY_FOREACH(current_func->targets, pObj)
        {
            CCLOG("js func ( %p ), native target[%d]=( %p )", current_func->jsfuncObj, jsfuncTargetCount, pObj);
            jsfuncTargetCount++;
        }
    }
    CCAssert(nativeTargetsCount == jsfuncTargetCount, "");
    CCLOG("\n---------JSScheduleWrapper dump end--------------\n");
#endif
}

void JSScheduleWrapper::scheduleFunc(float dt) const
{
    jsval retval = JSVAL_NULL;
    jsval data = DOUBLE_TO_JSVAL(dt);

    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();

    JSBool ok = JS_AddValueRoot(cx, &data);
    if (!ok) {
        CCLOG("scheduleFunc: Root value fails.");
        return;
    }

    if(!jsCallback.isNullOrUndefined()) {
        if (!jsThisObj.isNullOrUndefined()) {
            JSAutoCompartment ac(cx, JSVAL_TO_OBJECT(jsThisObj));
            JS_CallFunctionValue(cx, JSVAL_TO_OBJECT(jsThisObj), jsCallback, 1, &data, &retval);
        }
        else {
            JS_CallFunctionValue(cx, NULL, jsCallback, 1, &data, &retval);
        }
    }

    JS_RemoveValueRoot(cx, &data);
}

void JSScheduleWrapper::update(float dt)
{
    jsval data = DOUBLE_TO_JSVAL(dt);
    
    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
    
    JSBool ok = JS_AddValueRoot(cx, &data);
    if (!ok) {
        CCLOG("scheduleFunc: Root value fails.");
        return;
    }
    
    ScriptingCore::getInstance()->executeFunctionWithOwner(jsThisObj, "update", 1, &data);
    
    JS_RemoveValueRoot(cx, &data);
}

CAObject* JSScheduleWrapper::getTarget()
{
    return _pTarget;
}

void JSScheduleWrapper::setTarget(CAObject* pTarget)
{
    _pTarget = pTarget;
}

void JSScheduleWrapper::setPureJSTarget(JSObject* pPureJSTarget)
{
    CCAssert(_pPureJSTarget == NULL, "The pure js target has been set");
    JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
    _pPureJSTarget = pPureJSTarget;
    JS_AddNamedObjectRoot(cx, &_pPureJSTarget, "Pure JS target");
}

JSObject* JSScheduleWrapper::getPureJSTarget()
{
    return _pPureJSTarget;
}

void JSScheduleWrapper::setPriority(int priority)
{
    _priority = priority;
}

int  JSScheduleWrapper::getPriority()
{
    return _priority;
}

void JSScheduleWrapper::setUpdateSchedule(bool isUpdateSchedule)
{
    _isUpdateSchedule = isUpdateSchedule;
}

bool JSScheduleWrapper::isUpdateSchedule()
{
    return _isUpdateSchedule;
}



JSBool js_doNothing(JSContext *cx, uint32_t argc, jsval *vp) {
    return JS_TRUE;
}

JSBool js_forceGC(JSContext *cx, uint32_t argc, jsval *vp) {
    JSRuntime *rt = JS_GetRuntime(cx);
    JS_GC(rt);
    return JS_TRUE;
}

JSBool js_CrossApp_retain(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *thisObj = JS_THIS_OBJECT(cx, vp);
	if (thisObj) {
		js_proxy_t *proxy = jsb_get_js_proxy(thisObj);
		if (proxy) {
			((CAObject *)proxy->ptr)->retain();
			return JS_TRUE;
		}
	}
    JS_ReportError(cx, "Invalid Native Object.");
	return JS_FALSE;
}
JSBool js_CrossApp_setTag(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	CrossApp::CAView* cobj = (CrossApp::CAView *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 1) {
		int arg0;
		ok &= jsval_to_int32(cx, argv[0], (int32_t *)&arg0);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		cobj->setTag(arg0);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}
    
	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}
JSBool js_CrossApp_release(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *thisObj = JS_THIS_OBJECT(cx, vp);
	if (thisObj) {
		js_proxy_t *proxy = jsb_get_js_proxy(thisObj);
		if (proxy) {
            CCLog("retain count  %d",((CAObject *)proxy->ptr)->retainCount());
            CC_SAFE_RELEASE((CAObject *)proxy->ptr);
			//((CAObject *)proxy->ptr)->release();
            CCLog("after retain count  %d",((CAObject *)proxy->ptr)->retainCount());
			return JS_TRUE;
		}
	}
    JS_ReportError(cx, "Invalid Native Object.");
	return JS_FALSE;
}



JSBool js_CrossApp_ccGLEnableVertexAttribs(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSBool ok = JS_TRUE;
	jsval *argv = JS_ARGV(cx, vp);
    
	if (argc == 1) {
		unsigned int arg0;
		ok &= jsval_to_uint32(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		ccGLEnableVertexAttribs(arg0);
		JS_SET_RVAL(cx, vp, JSVAL_NULL);
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}



JSBool js_CrossApp_ccpAdd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSBool ok = JS_TRUE;
	jsval *argv = JS_ARGV(cx, vp);
    
	if (argc == 2) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpAdd(arg0, arg1);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}



JSBool js_CrossApp_ccpDistance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSBool ok = JS_TRUE;
	jsval *argv = JS_ARGV(cx, vp);
    
	if (argc == 2) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
		
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		float ret = ccpDistance(arg0, arg1);
		
		jsval jsret = DOUBLE_TO_JSVAL(ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpClamp(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 3) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
		CrossApp::CADipPoint arg2;
		ok &= jsval_to_ccpoint(cx, argv[2], &arg2);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpClamp(arg0, arg1, arg2);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpLengthSQ(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 1) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		float ret = ccpLengthSQ(arg0);

		jsval jsret = DOUBLE_TO_JSVAL(ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;

	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpLength(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 1) {
		CrossApp::CADipPoint arg0;
        ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

		float ret = ccpLength(arg0);

		jsval jsret = DOUBLE_TO_JSVAL(ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;

	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpNeg(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 1) {
		CrossApp::CADipPoint arg0;
        ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpNeg(arg0);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpSub(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 2) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpSub(arg0, arg1);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
        
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpMult(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 2) {
        CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
        
		double arg1;
		ok &= JS_ValueToNumber(cx, argv[1], &arg1);
        
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		
		CADipPoint ret = ccpMult(arg0, arg1);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpMidpoint(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 2) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpMidpoint(arg0, arg1);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
        
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return JS_FALSE;
}


JSBool js_CrossApp_ccpDot(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 2) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		float ret = ccpDot(arg0, arg1);
		
		jsval jsret = DOUBLE_TO_JSVAL(ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
        
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpCross(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 2) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		float ret = ccpCross(arg0, arg1);
		
		jsval jsret = DOUBLE_TO_JSVAL(ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
        
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpPerp(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 1) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpPerp(arg0);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}


JSBool js_CrossApp_ccpRPerp(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 1) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpRPerp(arg0);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
        
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}


JSBool js_CrossApp_ccpProject(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 2) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpProject(arg0, arg1);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
        
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpRotate(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 2) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
		CrossApp::CADipPoint arg1;
		ok &= jsval_to_ccpoint(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpRotate(arg0, arg1);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
        
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return JS_FALSE;
}

JSBool js_CrossApp_ccpNormalize(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
	if (argc == 1) {
		CrossApp::CADipPoint arg0;
		ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
		CADipPoint ret = ccpNormalize(arg0);
		
		jsval jsret = ccpoint_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		
		return JS_TRUE;
	}
	
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}

// setBlendFunc
template<class T>
JSBool js_CrossApp_setBlendFunc(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj;
    T* cobj;
    obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (T*)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, cobj)
    if (argc == 2)
    {
        GLenum src, dst;
        jsval_to_int32(cx, argv[0], (int32_t*)&src);
        jsval_to_int32(cx, argv[1], (int32_t*)&dst);
        ccBlendFunc blendFunc = {src, dst};
        cobj->setBlendFunc(blendFunc);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
    return JS_FALSE;
}

JSBool js_CrossApp_CAPageView_setViews(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	CAPageView* cobj = (CAPageView *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 1) {
        
        CrossApp::CCArray* arg0;
		do {
			if (!argv[0].isObject()) { ok = JS_FALSE; break; }
            ok &=jsval_to_ccarray(cx, argv[0], &arg0);
			JSB_PRECONDITION2( arg0, cx, JS_FALSE, "Invalid Native Object");
		} while (0);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

        
        CAVector<CAView*> rets;
        CAObject* pObj;
        CCARRAY_FOREACH(arg0, pObj)
        {
            CAView* pInteger = (CAView*)pObj;
            rets.pushBack(pInteger);
        }

		cobj->setViews(rets);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}
    
	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}
JSBool js_CrossApp_CATabBarController_initWithViewControllers(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	CrossApp::CATabBarController* cobj = (CrossApp::CATabBarController *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 1) {
		CrossApp::CCArray* arg0;
		do {
			if (!argv[0].isObject()) { ok = JS_FALSE; break; }
            ok &=jsval_to_ccarray(cx, argv[0], &arg0);
			JSB_PRECONDITION2( arg0, cx, JS_FALSE, "Invalid Native Object");
		} while (0);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
        std::vector<CAViewController *> rets;
        CAObject* pObj;
        CCARRAY_FOREACH(arg0, pObj)
        {
            CAViewController* pInteger = (CAViewController*)pObj;
            rets.push_back(pInteger);
        }
		bool ret = cobj->initWithViewControllers(rets);
		jsval jsret;
		jsret = BOOLEAN_TO_JSVAL(ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}
	if (argc == 2) {
		std::vector<CrossApp::CAViewController *, std::allocator<CrossApp::CAViewController *> > arg0;
		CrossApp::CABarVerticalAlignment arg1;
		do {
			if (!argv[0].isObject()) { ok = JS_FALSE; break; }
			js_proxy_t *proxy;
			JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
			proxy = jsb_get_js_proxy(tmpObj);
			arg0 = *(std::vector<CrossApp::CAViewController *, std::allocator<CrossApp::CAViewController *> >*)(proxy ? proxy->ptr : NULL);
			//JSB_PRECONDITION2( arg0, cx, JS_FALSE, "Invalid Native Object");
		} while (0);
		ok &= jsval_to_int32(cx, argv[1], (int32_t *)&arg1);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		bool ret = cobj->initWithViewControllers(arg0, arg1);
        
		jsval jsret;
		jsret = BOOLEAN_TO_JSVAL(ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}
    
	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}
static JSBool jsval_to_string_vector(JSContext* cx, jsval v, std::vector<std::string>& ret) {
    JSObject *jsobj;
    JSBool ok = JS_ValueToObject( cx, v, &jsobj );
    JSB_PRECONDITION2( ok, cx, JS_FALSE, "Error converting value to object");
	JSB_PRECONDITION2( jsobj && JS_IsArrayObject( cx, jsobj),  cx, JS_FALSE, "Object must be an array");
    
    uint32_t len = 0;
    JS_GetArrayLength(cx, jsobj, &len);

    for (uint32_t i=0; i < len; i++) {
        jsval elt;
        if (JS_GetElement(cx, jsobj, i, &elt)) {
            
            if (JSVAL_IS_STRING(elt))
            {
                JSStringWrapper str(JSVAL_TO_STRING(elt));
                ret.push_back(str.get());
            }
        }
    }

    return JS_TRUE;
}


static jsval string_vector_to_jsval(JSContext* cx, const std::vector<std::string>& arr) {
    
    JSObject *jsretArr = JS_NewArrayObject(cx, 0, NULL);
    
    int i = 0;
    for(std::vector<std::string>::const_iterator iter = arr.begin(); iter != arr.end(); ++iter, ++i) {
        jsval arrElement = c_string_to_jsval(cx, iter->c_str());
        if(!JS_SetElement(cx, jsretArr, i, &arrElement)) {
            break;
        }
    }
    return OBJECT_TO_JSVAL(jsretArr);
}

JSBool js_CrossApp_CCFileUtils_setSearchResolutionsOrder(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CrossApp::CCFileUtils* cobj = (CrossApp::CCFileUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= jsval_to_string_vector(cx, argv[0], arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        cobj->setSearchResolutionsOrder(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}

JSBool js_CrossApp_CCFileUtils_setSearchPaths(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CrossApp::CCFileUtils* cobj = (CrossApp::CCFileUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= jsval_to_string_vector(cx, argv[0], arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        cobj->setSearchPaths(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}
JSBool js_CrossApp_CCFileUtils_getSearchPaths(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CrossApp::CCFileUtils* cobj = (CrossApp::CCFileUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 0) {
        std::vector<std::string> ret = cobj->getSearchPaths();
        jsval jsret;
        jsret = string_vector_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
    return JS_FALSE;
}

JSBool js_CrossApp_CCFileUtils_getStringFromFile(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CrossApp::CCFileUtils* cobj = (CrossApp::CCFileUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        const char* arg0;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        unsigned long size = 0;
        unsigned char* data = cobj->getFileData(arg0, "rb", &size);
        if (data && size > 0) {
            jsval jsret = c_string_to_jsval(cx, (char*)data, size);
            CC_SAFE_DELETE_ARRAY(data);
            
            JS_SET_RVAL(cx, vp, jsret);
            return JS_TRUE;
        }
        JS_ReportError(cx, "get file(%s) data fails", arg0);
        return JS_FALSE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 3);
    return JS_FALSE;
}

JSBool js_CrossApp_CCFileUtils_getByteArrayFromFile(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CrossApp::CCFileUtils* cobj = (CrossApp::CCFileUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        const char* arg0;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        unsigned long size = 0;
        unsigned char* data = cobj->getFileData(arg0, "rb", &size);
        do
        {
            if (data && size > 0) {
                JSObject* array = JS_NewUint8Array(cx, size);
                if (NULL == array) {
                    break;
                }
                uint8_t* bufdata = (uint8_t*)JS_GetArrayBufferViewData(array);
                memcpy(bufdata, data, size*sizeof(uint8_t));
                CC_SAFE_DELETE_ARRAY(data);
                
                JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(array));
                return JS_TRUE;
            }
        } while(false);
        
        JS_ReportError(cx, "get file(%s) data fails", arg0);
        return JS_FALSE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 3);
    return JS_FALSE;
}

JSBool js_CrossApp_CCFileUtils_getSearchResolutionsOrder(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CrossApp::CCFileUtils* cobj = (CrossApp::CCFileUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 0) {
        std::vector<std::string> ret = cobj->getSearchResolutionsOrder();
        jsval jsret;
        jsret = string_vector_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
    return JS_FALSE;
}

#pragma schedule
JSBool js_CAScheduler_schedule(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc >= 1) {
        JSBool ok = JS_TRUE;
		jsval *argv = JS_ARGV(cx, vp);
        
        JSObject *obj = JS_THIS_OBJECT(cx, vp);
		js_proxy_t *proxy = jsb_get_js_proxy(obj);
		CAView *node = (CAView *)(proxy ? proxy->ptr : NULL);
        CAScheduler *sched = CAScheduler::getScheduler();
        
        JSScheduleWrapper *tmpCobj = NULL;
        
    	double interval = 0.0;
        if( argc >= 2 ) {
            ok &= JS_ValueToNumber(cx, argv[1], &interval );
        }
        
        //
        // repeat
        //
        double repeat = 0.0;
        if( argc >= 3 ) {
            ok &= JS_ValueToNumber(cx, argv[2], &repeat );
        }
        
        //
        // delay
        //
        double delay = 0.0;
        if( argc >= 4 ) {
            ok &= JS_ValueToNumber(cx, argv[3], &delay );
        }
        
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        
        bool bFound = false;
        CCArray* pTargetArr = JSScheduleWrapper::getTargetForJSObject(obj);
        CAObject* pObj = NULL;
        CCARRAY_FOREACH(pTargetArr, pObj)
        {
            JSScheduleWrapper* pTarget = (JSScheduleWrapper*)pObj;
            if (argv[0] == pTarget->getJSCallbackFunc())
            {
                tmpCobj = pTarget;
                bFound = true;
                break;
            }
        }
        
        if (!bFound)
        {
            tmpCobj = new JSScheduleWrapper();
            tmpCobj->autorelease();
            tmpCobj->setJSCallbackThis(OBJECT_TO_JSVAL(obj));
            tmpCobj->setJSCallbackFunc(argv[0]);
            tmpCobj->setTarget(node);
            JSScheduleWrapper::setTargetForSchedule(argv[0], tmpCobj);
            JSScheduleWrapper::setTargetForJSObject(obj, tmpCobj);
        }
        
        if(argc == 1) {
            CAScheduler::schedule(schedule_selector(JSScheduleWrapper::scheduleFunc), tmpCobj, 0, !node->isRunning(), 0);
           // sched->scheduleSelector(schedule_selector(JSScheduleWrapper::scheduleFunc), tmpCobj, 0, !node->isRunning());
        } if(argc == 2) {
            CAScheduler::schedule(schedule_selector(JSScheduleWrapper::scheduleFunc), tmpCobj, interval, !node->isRunning(), 0);
          //  sched->scheduleSelector(schedule_selector(JSScheduleWrapper::scheduleFunc), tmpCobj, interval, !node->isRunning());
        } if(argc == 3) {
            CAScheduler::schedule(schedule_selector(JSScheduleWrapper::scheduleFunc), tmpCobj, interval,(unsigned int)repeat,0 ,!node->isRunning());
           // sched->scheduleSelector(schedule_selector(JSScheduleWrapper::scheduleFunc), tmpCobj, interval, (unsigned int)repeat, 0, !node->isRunning());
        } if (argc == 4) {
            CAScheduler::schedule(schedule_selector(JSScheduleWrapper::scheduleFunc), tmpCobj, interval,(unsigned int)repeat,delay ,!node->isRunning());
           // sched->scheduleSelector(schedule_selector(JSScheduleWrapper::scheduleFunc), tmpCobj, interval, (unsigned int)repeat, delay, !node->isRunning());
        }
		
        // I comment next line with the same reason in the js_CCNode_scheduleOnce.
        //jsb_set_reserved_slot(proxy->obj, 0, argv[0]);
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}



#define js_CrossApp_CCCamera_getXYZ(funcName) \
    JSBool ok = JS_TRUE;                                                                                                  \
	JSObject *obj = JS_THIS_OBJECT(cx, vp);                                                                               \
	js_proxy_t *proxy = jsb_get_js_proxy(obj);                                                                            \
	CrossApp::CACamera* cobj = (CrossApp::CACamera *)(proxy ? proxy->ptr : NULL);                                           \
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");                                                      \
	if (argc == 0) {                                                                                                      \
		float x;                                                                                                          \
		float y;                                                                                                          \
		float z;                                                                                                          \
		cobj->funcName(&x, &y, &z);                                                                                       \
        JSObject* tmp = JS_NewObject(cx, NULL, NULL, NULL);                                                               \
                                                                                                                          \
        do                                                                                                                \
        {                                                                                                                 \
            if (NULL == tmp) break;                                                                                       \
                                                                                                                          \
            ok = JS_DefineProperty(cx, tmp, "x", DOUBLE_TO_JSVAL(x), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&  \
            JS_DefineProperty(cx, tmp, "y", DOUBLE_TO_JSVAL(y), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&       \
            JS_DefineProperty(cx, tmp, "z", DOUBLE_TO_JSVAL(z), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT);         \
                                                                                                                          \
            if (ok) {                                                                                                     \
                JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(tmp));                                                                \
                return JS_TRUE;                                                                                           \
            }                                                                                                             \
        } while (false);                                                                                                  \
                                                                                                                          \
		JS_SET_RVAL(cx, vp, JSVAL_NULL);                                                                                  \
		return JS_TRUE;                                                                                                   \
	}                                                                                                                     \
                                                                                                                          \
	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);                                       \
	return JS_FALSE;



template<class T>
static JSBool js_CrossApp_setAnchorPoint(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    JSBool ok = JS_TRUE;
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    T* cobj = (T *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, cobj)

        if (argc == 1) {
            CrossApp::CADipPoint arg0;
            ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
            JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
            JS_SET_RVAL(cx, vp, JSVAL_VOID);
            return JS_TRUE;
        } if (argc == 2) {
            double x;
            ok &= JS_ValueToNumber(cx, argv[0], &x );
            double y;
            ok &= JS_ValueToNumber(cx, argv[1], &y );

            JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
            JS_SET_RVAL(cx, vp, JSVAL_VOID);
            return JS_TRUE;
        }
        JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
        return JS_FALSE;
}

template<class T>
static JSBool js_CrossApp_setContentSize(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    JSBool ok = JS_TRUE;
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    T* cobj = (T *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, cobj)
    
    if (argc == 1) {
        CrossApp::CADipSize arg0;
        ok &= jsval_to_ccsize(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    } if (argc == 2) {
        double x;
        ok &= JS_ValueToNumber(cx, argv[0], &x );
        double y;
        ok &= JS_ValueToNumber(cx, argv[1], &y );
        
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}

template<class T>
static JSBool js_CrossApp_setPosition(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    JSBool ok = JS_TRUE;
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    T* cobj = (T *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, cobj)
    
    if (argc == 1) {
        CrossApp::CADipPoint arg0;
        ok &= jsval_to_ccpoint(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        cobj->setPosition(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    } if (argc == 2) {
        double x;
        ok &= JS_ValueToNumber(cx, argv[0], &x );
        double y;
        ok &= JS_ValueToNumber(cx, argv[1], &y );
        
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        cobj->setPosition(CADipPoint(x,y));
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}






extern JSObject* jsb_CAScrollView_prototype;
extern JSObject* jsb_CATableView_prototype;
extern JSObject* jsb_CAControl_prototype;
extern JSObject* jsb_CAButton_prototype;
extern JSObject* jsb_CAPageView_prototype;
extern JSObject* jsb_CAPickerView_prototype;
extern JSObject* jsb_CATextField_prototype;
extern JSObject* jsb_CASlider_prototype;
extern JSObject* jsb_CAListView_prototype;
//extern JSObject* jsb_JSViewController_prototype;
//extern JSObject* jsb_CASwitch_prototype;
void register_CrossApp_js_special(JSContext* cx, JSObject* global)
{
	// first, try to get the ns
	jsval nsval;
	JSObject *ns;
	JS_GetProperty(cx, global, "ca", &nsval);
	if (nsval == JSVAL_VOID) {
		ns = JS_NewObject(cx, NULL, NULL, NULL);
		nsval = OBJECT_TO_JSVAL(ns);
		JS_SetProperty(cx, global, "ca", &nsval);
	} else {
		JS_ValueToObject(cx, nsval, &ns);
	}
    JS_DefineFunction(cx, jsb_CASlider_prototype, "addTarget", js_CrossApp_CASlider_addTarget, 2, JSPROP_READONLY | JSPROP_PERMANENT);
	JS_DefineFunction(cx, global, "__associateObjWithNative", js_CrossApp_swap_native_object, 2, JSPROP_READONLY | JSPROP_PERMANENT);
	JS_DefineFunction(cx, global, "__getPlatform", js_platform, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CATabBarController_prototype, "initWithViewControllers", js_CrossApp_CATabBarController_initWithViewControllers, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAView_prototype, "setTag", js_CrossApp_setTag, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CATextField_prototype, "setDelegate", js_CrossApp_CATextField_setDelegate, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CASwitch_prototype, "addTarget", js_CrossApp_CASwitch_addTarget, 2, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAView_prototype, "retain", js_CrossApp_retain, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAView_prototype, "release", js_CrossApp_release, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAViewController_prototype, "retain", js_CrossApp_retain, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAViewController_prototype, "release", js_CrossApp_release, 0, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAScheduler_prototype, "schedule", js_CAScheduler_schedule, 2, JSPROP_READONLY | JSPROP_PERMANENT);
     JS_DefineFunction(cx, jsb_CAPageView_prototype, "setViews", js_CrossApp_CAPageView_setViews, 1, JSPROP_READONLY | JSPROP_PERMANENT);
//    JS_DefineFunction(cx, jsb_JSViewController_prototype, "ctor", js_CrossApp_JSViewController_ctor, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    //JS_DefineFunction(cx, jsb_JSViewController_prototype, "constructor", js_CrossApp_JSViewController_constructor, 1, JSPROP_READONLY | JSPROP_PERMANENT);js_CrossApp_CAListView_setDataSource
    JS_DefineFunction(cx, jsb_CAListView_prototype, "setListViewDataSource", js_CrossApp_CAListView_setDataSource, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAScrollView_prototype, "setDelegate", js_CrossApp_CAScrollView_setDelegate, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CATableView_prototype, "setDelegate", js_CrossApp_CATableView_setDelegate, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CATableView_prototype, "setDataSource", js_CrossApp_CATableView_setDataSource, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAPageView_prototype, "setDelegate", js_CrossApp_CAPageView_setDelegate, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAPickerView_prototype, "setDelegate", js_CrossApp_CAPickerView_setDelegate, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAPickerView_prototype, "setDataSource", js_CrossApp_CAPickerView_setDataSource, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_CAButton_prototype, "addTarget", js_CrossApp_CAButton_addTarget, 3, JSPROP_READONLY | JSPROP_PERMANENT);

}

