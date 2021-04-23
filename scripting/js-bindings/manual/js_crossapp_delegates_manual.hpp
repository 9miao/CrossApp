//
//  js_crossapp_delegates_manual.hpp
//  HelloJavacript
//
//  Created by zhanglei on 16/7/6.
//
//

#ifndef js_crossapp_delegates_manual_hpp
#define js_crossapp_delegates_manual_hpp

#include "jsapi.h"
#include "jsfriendapi.h"

void register_all_crossapp_delegates_manual(JSContext* cx, JS::HandleObject global);

#endif /* js_crossapp_delegates_manual_hpp */
