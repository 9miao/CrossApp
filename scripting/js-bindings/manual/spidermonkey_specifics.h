/*
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

#ifndef __SPIDERMONKEY_SPECIFICS_H__
#define __SPIDERMONKEY_SPECIFICS_H__

#include "jsapi.h"
#include "jsfriendapi.h"
#include "ccMacros.h"
#include "support/data_support/uthash.h"
#include <unordered_map>

typedef struct js_proxy {
    void *ptr;
    JS::Heap<JSObject*> obj;
    UT_hash_handle hh;
    JSObject* _jsobj;
} js_proxy_t;

extern js_proxy_t *_native_js_global_ht;
extern js_proxy_t *_js_native_global_ht;

typedef struct js_type_class {
    JSClass *jsclass;
    JS::Heap<JSObject*> proto;
    JS::Heap<JSObject*> parentProto;
} js_type_class_t;

extern std::unordered_map<std::string, js_type_class_t*> _js_global_type_map;
 
template< typename DERIVED >
class TypeTest
{
public:
    static const char* s_name()
    {
        // return id unique for DERIVED
        // ALWAYS VALID BUT STRING, NOT INT - BUT VALID AND CROSS-PLATFORM/CROSS-VERSION COMPATIBLE
        // AS FAR AS YOU KEEP THE CLASS NAME
        return typeid( DERIVED ).name();
    }
};

#endif
 
