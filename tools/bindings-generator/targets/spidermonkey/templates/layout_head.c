\#include "${out_file}.hpp"
#if $macro_judgement
$macro_judgement
#end if
\#include "crossapp_specifics.hpp"
#for header in $headers
    #set include_header = os.path.basename(header)
    #if $replace_headers.has_key(include_header)
\#include "${replace_headers[include_header]}"
    #else
\#include "${include_header}"
    #end if
#end for
#if $cpp_headers
#for header in $cpp_headers
\#include "${header}"
#end for
#end if

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedValue initializing(cx);
    bool isNewValid = true;
#if not $script_control_cpp
    JS::RootedObject global(cx, ScriptingCore::getInstance()->getGlobalObject());
    isNewValid = JS_GetProperty(cx, global, "initializing", &initializing) && initializing.toBoolean();
#end if
    if (isNewValid)
    {
        TypeTest<T> t;
        js_type_class_t *typeClass = nullptr;
        std::string typeName = t.s_name();
        auto typeMapIter = _js_global_type_map.find(typeName);
        CCAssert(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
        typeClass = typeMapIter->second;
        CCAssert(typeClass, "The value is null.");

        JS::RootedObject proto(cx, typeClass->proto.get());
        JS::RootedObject parent(cx, typeClass->parentProto.get());
        JS::RootedObject _tmp(cx, JS_NewObject(cx, typeClass->jsclass, proto, parent));

    #if $script_control_cpp
        T* cobj = new T();
        js_proxy_t *pp = jsb_new_proxy(cobj, _tmp);
        AddObjectRoot(cx, &pp->obj);
    #end if
        args.rval().set(OBJECT_TO_JSVAL(_tmp));
        return true;
    }

#if not $script_control_cpp
    JS_ReportError(cx, "Constructor for the requested class is not available, please refer to the API reference.");
#end if
    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    return false;
}

static bool js_is_native_obj(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().setBoolean(true);
    return true;
}
