#set has_constructor = False
#if $current_class.methods.has_key('constructor')
#set has_constructor = True
#set constructor = $current_class.methods.constructor
${current_class.methods.constructor.generate_code($current_class)}
#end if

#set generator = $current_class.generator
#set methods = $current_class.methods_clean()
#set st_methods = $current_class.static_methods_clean()
#set public_fields = $current_class.public_fields
#if len($current_class.parents) > 0
extern JSObject *jsb_${current_class.parents[0].underlined_class_name}_prototype;

#end if
void js_${current_class.underlined_class_name}_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOG("jsbindings: finalizing JS object %p (${current_class.class_name})", obj);
#if (not $current_class.is_ref_class and $has_constructor) or $generator.script_control_cpp
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    jsproxy = jsb_get_js_proxy(obj);
    if (jsproxy) {
        ${current_class.namespaced_class_name} *nobj = static_cast<${current_class.namespaced_class_name} *>(jsproxy->ptr);
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        if (nobj) {
            jsb_remove_proxy(nproxy, jsproxy);
    #if $current_class.is_ref_class
            nobj->release();
    #else
            delete nobj;
    #end if
        }
        else jsb_remove_proxy(nullptr, jsproxy);
    }
#end if
}
#if $generator.in_listed_extend_classed($current_class.class_name) and $has_constructor
#if not $constructor.is_overloaded
    ${constructor.generate_code($current_class, None, False, True)}
#else
    ${constructor.generate_code($current_class, False, True)}
#end if
#end if
void js_register_${generator.prefix}_${current_class.class_name}(JSContext *cx, JS::HandleObject global) {
    jsb_${current_class.underlined_class_name}_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_${current_class.underlined_class_name}_class->name = "${current_class.target_class_name}";
    jsb_${current_class.underlined_class_name}_class->addProperty = JS_PropertyStub;
    jsb_${current_class.underlined_class_name}_class->delProperty = JS_DeletePropertyStub;
    jsb_${current_class.underlined_class_name}_class->getProperty = JS_PropertyStub;
    jsb_${current_class.underlined_class_name}_class->setProperty = JS_StrictPropertyStub;
    jsb_${current_class.underlined_class_name}_class->enumerate = JS_EnumerateStub;
    jsb_${current_class.underlined_class_name}_class->resolve = JS_ResolveStub;
    jsb_${current_class.underlined_class_name}_class->convert = JS_ConvertStub;
    jsb_${current_class.underlined_class_name}_class->finalize = js_${current_class.underlined_class_name}_finalize;
    jsb_${current_class.underlined_class_name}_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PSG("__nativeObj", js_is_native_obj, JSPROP_PERMANENT | JSPROP_ENUMERATE),
#for m in public_fields
    #if $generator.should_bind_field($current_class.class_name, m.name)
        JS_PSGS("${m.name}", ${m.signature_name}_get_${m.name}, ${m.signature_name}_set_${m.name}, JSPROP_PERMANENT | JSPROP_ENUMERATE),
    #end if
#end for
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        #for m in methods
        #set fn = m['impl']
        JS_FN("${m['name']}", ${fn.signature_name}, ${fn.min_args}, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        #end for
#if $generator.in_listed_extend_classed($current_class.class_name) and $has_constructor
        JS_FN("ctor", js_${generator.prefix}_${current_class.class_name}_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
#end if
        JS_FS_END
    };

    #if len(st_methods) > 0
    static JSFunctionSpec st_funcs[] = {
        #for m in st_methods
        #set fn = m['impl']
        JS_FN("${m['name']}", ${fn.signature_name}, ${fn.min_args}, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        #end for
        JS_FS_END
    };
    #else
    JSFunctionSpec *st_funcs = NULL;
    #end if

    jsb_${current_class.underlined_class_name}_prototype = JS_InitClass(
        cx, global,
#if len($current_class.parents) > 0
        JS::RootedObject(cx, jsb_${current_class.parents[0].underlined_class_name}_prototype),
#else
        JS::NullPtr(), // parent proto
#end if
        jsb_${current_class.underlined_class_name}_class,
#if has_constructor
        js_${generator.prefix}_${current_class.class_name}_constructor, 0, // constructor
#else if $current_class.is_abstract
        empty_constructor, 0,
#else
        dummy_constructor<${current_class.namespaced_class_name}>, 0, // no constructor
#end if
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27
//  JS_SetPropertyAttributes(cx, global, "${current_class.target_class_name}", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<${current_class.namespaced_class_name}> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_${current_class.underlined_class_name}_class;
        p->proto = jsb_${current_class.underlined_class_name}_prototype;
#if len($current_class.parents) > 0
        p->parentProto = jsb_${current_class.parents[0].underlined_class_name}_prototype;
#else
        p->parentProto = NULL;
#end if
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
#if $generator.in_listed_extend_classed($current_class.class_name) and not $current_class.is_abstract
    anonEvaluate(cx, global, "(function () { ${generator.target_ns}.${current_class.target_class_name}.extend = ca.Class.extend; })()");
#end if
}
