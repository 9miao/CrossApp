/*
 * Created by Rohan Kuruvilla
 * Copyright (c) 2012 Zynga Inc.
 * Copyright (c) 2013-2014 Chukong Technologies Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __JS_MANUAL_CONVERSIONS_H__
#define __JS_MANUAL_CONVERSIONS_H__

#include "jsapi.h"
#include "jsfriendapi.h"
#include "js_bindings_core.h"
#include "js_bindings_config.h"
#include "CrossApp.h"
#include "spidermonkey_specifics.h"

#define JSB_COMPATIBLE_WITH_COCOS2D_HTML5_BASIC_TYPES


// just a simple utility to avoid mem leaking when using JSString
class JSStringWrapper
{
public:
    JSStringWrapper();
    JSStringWrapper(JSString* str, JSContext* cx = NULL);
    JSStringWrapper(jsval val, JSContext* cx = NULL);
    ~JSStringWrapper();
    
    void set(jsval val, JSContext* cx);
    void set(JSString* str, JSContext* cx);
    const char* get();
    
private:
    const char* _buffer;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(JSStringWrapper);
};



// wraps a function and "this" object
class JSFunctionWrapper
{
public:
    JSFunctionWrapper(JSContext* cx, JSObject *jsthis, jsval fval);
    ~JSFunctionWrapper();
 
    bool invoke(unsigned int argc, jsval *argv, JS::MutableHandleValue rval);
private:
    JSContext *_cx;
    JS::Heap<JSObject*> _jsthis;
    JS::Heap<JS::Value> _fval;
private:
    CC_DISALLOW_COPY_AND_ASSIGN(JSFunctionWrapper);
};

bool jsval_to_opaque( JSContext *cx, JS::HandleValue vp, void **out );
bool jsval_to_int( JSContext *cx, JS::HandleValue vp, int *out);
bool jsval_to_uint( JSContext *cx, JS::HandleValue vp, unsigned int *out);
bool jsval_to_c_class( JSContext *cx, JS::HandleValue vp, void **out_native, struct jsb_c_proxy_s **out_proxy);
/** converts a jsval (JS string) into a char */
bool jsval_to_charptr( JSContext *cx, JS::HandleValue vp, const char **out);

jsval opaque_to_jsval( JSContext *cx, void* opaque);
jsval c_class_to_jsval( JSContext *cx, void* handle, JS::HandleObject object, JSClass *klass, const char* class_name);

///* Converts a char ptr into a jsval (using JS string) */
jsval charptr_to_jsval( JSContext *cx, const char *str);//zero
bool JSB_jsval_typedarray_to_dataptr( JSContext *cx, JS::HandleValue vp, GLsizei *count, void **data, js::Scalar::Type t);
bool JSB_get_arraybufferview_dataptr( JSContext *cx, JS::HandleValue vp, GLsizei *count, GLvoid **data );
//
//// some utility functions
//// to native
bool jsval_to_ushort( JSContext *cx, JS::HandleValue vp, unsigned short *ret );
bool jsval_to_int32( JSContext *cx, JS::HandleValue vp, int32_t *ret );
bool jsval_to_uint32( JSContext *cx, JS::HandleValue vp, uint32_t *ret );
bool jsval_to_uint16( JSContext *cx, JS::HandleValue vp, uint16_t *ret );
bool jsval_to_long( JSContext *cx, JS::HandleValue vp, long *out);
bool jsval_to_ulong( JSContext *cx, JS::HandleValue vp, unsigned long *out);
bool jsval_to_long_long(JSContext *cx, JS::HandleValue v, long long* ret);
bool jsval_to_float( JSContext *cx, JS::HandleValue vp, float *ret );
bool jsval_to_double( JSContext *cx, JS::HandleValue vp, double *ret );
bool jsval_to_std_u16string(JSContext *cx, JS::HandleValue v, std::u16string* ret);
bool jsval_to_std_string(JSContext *cx, JS::HandleValue v, std::string* ret);


bool javal_to_viodpointe(JSContext *cx, JS::HandleValue v, void* context);
bool jsval_to_dpoint(JSContext *cx, JS::HandleValue v, CrossApp::DPoint* ret);
bool jsval_to_dpoint3d(JSContext *cx, JS::HandleValue v, CrossApp::DPoint3D* ret);

bool jsval_to_drect(JSContext *cx, JS::HandleValue v, CrossApp::DRect* ret);
bool jsval_to_dsize(JSContext *cx, JS::HandleValue v, CrossApp::DSize* ret);
bool jsval_to_dhorizontallayout(JSContext *cx, JS::HandleValue v, CrossApp::DHorizontalLayout* horizontal);
bool jsval_to_dverticallayout(JSContext *cx, JS::HandleValue v, CrossApp::DVerticalLayout* vertical);
bool jsval_to_dlayout(JSContext *cx, JS::HandleValue v, CrossApp::DLayout* layout);
bool jsval_to_cafont(JSContext *cx, JS::HandleValue v, CrossApp::CAFont* ret);
bool jsval_to_cacolor4b(JSContext *cx, JS::HandleValue v, CrossApp::CAColor4B* ret);
bool jsval_to_cacolor4f(JSContext *cx, JS::HandleValue v, CrossApp::CAColor4F* ret);
bool jsval_cacolor_to_opacity(JSContext *cx, JS::HandleValue v, int32_t* ret);
bool jsval_to_quaternion( JSContext *cx, JS::HandleValue v, CrossApp::Quaternion* ret);

// forward declaration
js_proxy_t* jsb_get_js_proxy(JSObject* jsObj);

template <class T>
bool jsval_to_cavector(JSContext* cx, JS::HandleValue v, CrossApp::CAVector<T>* ret)
{
    JS::RootedObject jsobj(cx);

    bool ok = v.isObject() && JS_ValueToObject( cx, v, &jsobj );
    JSB_PRECONDITION3( ok, cx, false, "Error converting value to object");
    JSB_PRECONDITION3( jsobj && JS_IsArrayObject( cx, jsobj),  cx, false, "Object must be an array");
    
    uint32_t len = 0;
    JS_GetArrayLength(cx, jsobj, &len);

    for (uint32_t i=0; i < len; i++)
    {
        JS::RootedValue value(cx);
        if (JS_GetElement(cx, jsobj, i, &value))
        {
            CCAssert(value.isObject(), "the element in Vector isn't a native object.");

            js_proxy_t *proxy;
            JSObject *tmp = value.toObjectOrNull();
            proxy = jsb_get_js_proxy(tmp);
            T cobj = (T)(proxy ? proxy->ptr : nullptr);
            if (cobj)
            {
                ret->pushBack(cobj);
            }
        }
    }

    return true;
}


bool jsval_to_cavalue(JSContext* cx, JS::HandleValue v, CrossApp::CAValue* ret);//**
bool jsval_to_cavaluemap(JSContext* cx, JS::HandleValue v, CrossApp::CAValueMap* ret);//**
bool jsval_to_cavaluemapintkey(JSContext* cx, JS::HandleValue v, CrossApp::CAValueMapIntKey* ret);//**
bool jsval_to_cavaluevector(JSContext* cx, JS::HandleValue v, CrossApp::CAValueVector* ret);//**
bool jsval_to_ssize( JSContext *cx, JS::HandleValue vp, ssize_t* ret);
bool jsval_to_std_vector_string( JSContext *cx, JS::HandleValue vp, std::vector<std::string>* ret);//**
bool jsval_to_matrix(JSContext *cx, JS::HandleValue vp, CrossApp::Mat4* ret);
bool jsval_to_vector2(JSContext *cx, JS::HandleValue vp, CrossApp::DPoint* ret);
bool jsval_to_blendfunc(JSContext *cx, JS::HandleValue vp, CrossApp::BlendFunc* ret);
bool jsval_to_vector_vec2(JSContext* cx, JS::HandleValue v, std::vector<CrossApp::DPoint>* ret);
//bool jsval_to_cctex2f(JSContext* cx, JS::HandleValue vp, CrossApp::ccTex2F* ret);//**
//bool jsval_to_v3fc4bt2f(JSContext* cx, JS::HandleValue v, CrossApp::ccV3F_C4B_T2F* ret);//**
//bool jsval_to_v3fc4bt2f_quad(JSContext* cx, JS::HandleValue v, CrossApp::ccV3F_C4B_T2F_Quad* ret);//**
//bool jsval_to_vector_v3fc4bt2f(JSContext* cx, JS::HandleValue v, std:vector<CrossApp::ccV3F_C4B_T2F>* ret);//**
bool jsval_to_std_map_string_string(JSContext* cx, JS::HandleValue v, std::map<std::string, std::string>* ret);



template <class T>
bool jsval_to_calist(JSContext* cx, JS::HandleValue v, CrossApp::CAList<T>* ret)
{
   
    JS::RootedObject jsobj(cx);
    
    
    bool ok = v.isObject() && JS_ValueToObject( cx, v, &jsobj );
    
    
    JSB_PRECONDITION3( ok, cx, false, "Error converting value to object");
    
    JSB_PRECONDITION3( jsobj && JS_IsArrayObject( cx, jsobj),  cx, false, "Object must be an array");
    
    uint32_t len = 0;
    JS_GetArrayLength(cx, jsobj, &len);
    
    
    for (uint32_t i=0; i < len; i++)
    {
        JS::RootedValue value(cx);
        if (JS_GetElement(cx, jsobj, i, &value))
        {
            CCAssert(value.isObject(), "the element in Vector isn't a native object.");
            
            js_proxy_t *proxy;
            JSObject *tmp = value.toObjectOrNull();
            proxy = jsb_get_js_proxy(tmp);
            T cobj = (T)(proxy ? proxy->ptr : nullptr);
            if (cobj)
            {
                ret->pushBack(cobj);
            }
        }
    }
    
    return true;
    
}

template <class T>
bool jsval_to_cadeque(JSContext* cx, JS::HandleValue v, CrossApp::CADeque<T>* ret)
{

    JS::RootedObject jsobj(cx);
    

    bool ok = v.isObject() && JS_ValueToObject( cx, v, &jsobj );
    

    JSB_PRECONDITION3( ok, cx, false, "Error converting value to object");

    JSB_PRECONDITION3( jsobj && JS_IsArrayObject( cx, jsobj),  cx, false, "Object must be an array");

    uint32_t len = 0;
    JS_GetArrayLength(cx, jsobj, &len);
    

    for (uint32_t i=0; i < len; i++)
    {
        JS::RootedValue value(cx);
        if (JS_GetElement(cx, jsobj, i, &value))
        {
            CCAssert(value.isObject(), "the element in Vector isn't a native object.");
            
            js_proxy_t *proxy;
            JSObject *tmp = value.toObjectOrNull();
            proxy = jsb_get_js_proxy(tmp);
            T cobj = (T)(proxy ? proxy->ptr : nullptr);
            if (cobj)
            {
                ret->pushBack(cobj);
            }
        }
    }
    
    return true;
}


template <class T>
bool jsval_to_ccmap_string_key(JSContext *cx, JS::HandleValue v, CrossApp::CAMap<std::string, T>* ret)
{
    if (v.isNullOrUndefined())
    {
        return true;
    }

    JS::RootedObject tmp(cx, v.toObjectOrNull());

    if (!tmp) {
        CCLOG("%s", "jsval_to_ccvaluemap: the jsval is not an object.");
        return false;
    }

    JS::RootedObject it(cx, JS_NewPropertyIterator(cx, tmp));
    
    while (true)
    {
        JS::RootedId idp(cx);
        JS::RootedValue key(cx);
        if (! JS_NextProperty(cx, it, idp.address()) || ! JS_IdToValue(cx, idp, &key)) {
            return false; // error
        }
        
        if (key.isUndefined()) {
            break; // end of iteration
        }
        
        if (!key.isString()) {
            continue; // ignore integer properties
        }
        
        JSStringWrapper keyWrapper(key.toString(), cx);
        
        JS::RootedValue value(cx);
        JS_GetPropertyById(cx, tmp, idp, &value);
        if (value.isObject())
        {
            js_proxy_t *proxy = nullptr;
            JSObject* jsobj = value.toObjectOrNull();
            proxy = jsb_get_js_proxy(jsobj);
            CCAssert(proxy, "Native object should be added!");
            T cobj = (T)(proxy ? proxy->ptr : nullptr);
            ret->insert(keyWrapper.get(), cobj);
        }
        else
        {
            CCAssert(false, "not supported type");
        }
    }
    
    return true;
}

//DHorizontalLayout horizontal;

//DVerticalLayout vertical;
//DLayout
// from native
jsval int32_to_jsval( JSContext *cx, int32_t l);
jsval uint32_to_jsval( JSContext *cx, uint32_t number );
jsval ushort_to_jsval( JSContext *cx, unsigned short number );
jsval long_to_jsval( JSContext *cx, long number );
jsval ulong_to_jsval(JSContext* cx, unsigned long v);
jsval long_long_to_jsval(JSContext* cx, long long v);
jsval float_to_jsval( JSContext *cx, float number);
jsval double_to_jsval( JSContext *cx, double number);
jsval std_u16String_to_jsval(JSContext* cx, const std::u16string& v);
jsval std_string_to_jsval(JSContext* cx, const std::string& v);
jsval c_string_to_jsval(JSContext* cx, const char* v, size_t length = -1);
jsval u_char_to_jsval(JSContext* cx, const unsigned char* v, size_t length = -1);
jsval tm_to_jsval(JSContext* cx, const struct tm& v);
jsval dpoint_to_jsval(JSContext* cx, const CrossApp::DPoint& v);
jsval dpoint3d_to_jsval(JSContext* cx, const CrossApp::DPoint3D& v);

jsval viodpointe_to_javal(JSContext *cx, JS::HandleValue v, void* context);
jsval drect_to_jsval(JSContext* cx, const CrossApp::DRect& v);
jsval dsize_to_jsval(JSContext* cx, const CrossApp::DSize& v);
jsval dhorizontallayout_to_jsval( JSContext *cx, const CrossApp::DHorizontalLayout& v);
jsval dverticallayout_to_jsval(JSContext *cx, const CrossApp::DVerticalLayout& v);
jsval dlayout_to_jsval(JSContext *cx, const CrossApp::DLayout& v);
jsval cafont_to_jsval(JSContext *cx, const CrossApp::CAFont& v);
jsval cacolor4b_to_jsval(JSContext* cx, const CrossApp::CAColor4B& v);
jsval cacolor4f_to_jsval(JSContext* cx, const CrossApp::CAColor4F& v);
jsval quaternion_to_jsval(JSContext* cx, const CrossApp::Quaternion& q);

template<class T>
js_proxy_t *js_get_or_create_proxy(JSContext *cx, T *native_obj);

template <class T>
jsval cavector_to_jsval(JSContext* cx, const CrossApp::CAVector<T>& v)
{
    JS::RootedObject jsretArr(cx, JS_NewArrayObject(cx, 0));
    
    int i = 0;
    for (const auto& obj : v)
    {
        JS::RootedValue arrElement(cx);
        
        //First, check whether object is associated with js object.
        js_proxy_t* jsproxy = js_get_or_create_proxy(cx, obj);
        if (jsproxy) {
            arrElement = OBJECT_TO_JSVAL(jsproxy->obj);
        }

        if (!JS_SetElement(cx, jsretArr, i, arrElement)) {
            break;
        }
        ++i;
    }
    return OBJECT_TO_JSVAL(jsretArr);
}


template <class T>
jsval calist_to_jsval(JSContext* cx, const CrossApp::CAList<T>& v)
{

    JS::RootedObject jsretArr(cx, JS_NewArrayObject(cx, 0));
    
    int i = 0;
    for (const auto& obj : v)
    {

        JS::RootedValue arrElement(cx);

        js_proxy_t* jsproxy = js_get_or_create_proxy(cx, obj);
        if (jsproxy) {
            arrElement = OBJECT_TO_JSVAL(jsproxy->obj);
        }
        if (!JS_SetElement(cx, jsretArr, i, arrElement)) {
            break;
        }
        ++i;
    }
    
    return OBJECT_TO_JSVAL(jsretArr);
    
}


template <class T>
jsval cadeque_to_jsval(JSContext* cx, const CrossApp::CADeque<T>& v)
{

    JS::RootedObject jsretArr(cx, JS_NewArrayObject(cx, 0));
    
    int i = 0;
    for (const auto& obj : v)
    {

        JS::RootedValue arrElement(cx);

        js_proxy_t* jsproxy = js_get_or_create_proxy(cx, obj);
        if (jsproxy) {
            arrElement = OBJECT_TO_JSVAL(jsproxy->obj);
        }
        
        if (!JS_SetElement(cx, jsretArr, i, arrElement)) {
            break;
        }
        ++i;
    }
    
    return OBJECT_TO_JSVAL(jsretArr);
}


template <class T>
jsval ccmap_string_key_to_jsval(JSContext* cx, const CrossApp::CAMap<std::string, T>& v)
{
    JS::RootedObject proto(cx);
    JS::RootedObject parent(cx);
    JS::RootedObject jsRet(cx, JS_NewObject(cx, NULL, proto, parent));
    
    for (auto iter = v.begin(); iter != v.end(); ++iter)
    {
        JS::RootedValue element(cx);
        
        std::string key = iter->first;
        T obj = iter->second;
        
        //First, check whether object is associated with js object.
        js_proxy_t* jsproxy = js_get_or_create_proxy(cx, obj);
        if (jsproxy) {
            element = OBJECT_TO_JSVAL(jsproxy->obj);
        }
        
        if (!key.empty())
        {
            JS_SetProperty(cx, jsRet, key.c_str(), element);
        }
    }
    return OBJECT_TO_JSVAL(jsRet);
}

jsval cavalue_to_jsval(JSContext* cx, const CrossApp::CAValue& v);
jsval cavaluemap_to_jsval(JSContext* cx, const CrossApp::CAValueMap& v);
jsval cavaluemapintkey_to_jsval(JSContext* cx, const CrossApp::CAValueMapIntKey& v);
jsval cavaluevector_to_jsval(JSContext* cx, const CrossApp::CAValueVector& v);
jsval ssize_to_jsval(JSContext *cx, ssize_t v);
jsval std_vector_string_to_jsval( JSContext *cx, const std::vector<std::string>& v);
jsval std_vector_char_to_jsval( JSContext *cx, const std::vector<char>& v);
jsval std_vector_int_to_jsval( JSContext *cx, const std::vector<int>& v);
jsval std_vector_float_to_jsval( JSContext *cx, const std::vector<float>& v);
jsval std_set_int_to_jsval( JSContext *cx, const std::set<int>& v);
jsval matrix_to_jsval(JSContext *cx, const CrossApp::Mat4& v);
jsval vector2_to_jsval(JSContext *cx, const CrossApp::DPoint& v);
jsval blendfunc_to_jsval(JSContext *cx, const CrossApp::BlendFunc& v);
jsval std_map_string_string_to_jsval(JSContext* cx, const std::map<std::string, std::string>& v);

#endif /* __JS_MANUAL_CONVERSIONS_H__ */

