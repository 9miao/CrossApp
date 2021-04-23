do {
    if(JS_TypeOfValue(cx, ${in_value}) == JSTYPE_FUNCTION)
    {
        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, args.thisv().toObjectOrNull(), ${in_value}));
        auto lambda = [=, &ok](${lambda_parameters}) -> ${ret_type.name} {
            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
            #set arg_count = len($param_types)
            #if $arg_count > 0
            jsval largv[${arg_count}];
            #end if
            #set $count = 0
            #while $count < $arg_count
                #set $arg = $param_types[$count]
            ${arg.from_native({"generator": $generator,
                                 "in_value": "larg" + str(count),
                                 "out_value": "largv[" + str(count) + "]",
                                 "class_name": $class_name,
                                 "level": 2,
                                 "ntype": str($arg)})};
                #set $count = $count + 1
            #end while
            JS::RootedValue rval(cx);
            #if $arg_count > 0
            bool succeed = func->invoke(${arg_count}, &largv[0], &rval);
            #else
            bool succeed = func->invoke(${arg_count}, nullptr, &rval);
            #end if
            if (!succeed && JS_IsExceptionPending(cx)) {
                JS_ReportPendingException(cx);
            }
            #if $ret_type.name != "void"
            ${ret_type.get_whole_name($generator)} ret;
            ${ret_type.to_native({"generator": $generator,
                                 "in_value": "rval",
                                 "out_value": "ret",
                                 "ntype": str($ret_type),
                                 "level": 2})};
            return ret;
            #end if
        };
        ${out_value} = lambda;
    }
    else
    {
        ${out_value} = nullptr;
    }
} while(0)
