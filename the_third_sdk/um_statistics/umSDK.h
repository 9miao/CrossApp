//
//  umSDK.h
//
//  Created by 栗元峰 on 14-11-14.
//
//

#ifndef __CrossApp__umSDK__
#define __CrossApp__umSDK__

#define UMSDK_SWITCH 1

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

namespace umSDK
{
    void startWithAppkey(const char* appkey, const char* channel);
    
    void onPageStart(const char* ViewID);
    
    void onPageEnd(const char* ViewID);
    
    void onEventBegin(const char* eventId);
    
    void onEventBeginValue(const char* eventId, const char* value);
    
    void onEventEnd(const char* eventId);
    
    void onEventEndValue(const char* eventId, const char* value);
    
    void onEvent(const char* eventId);

    void onEventValue(const char* eventId, const char* value);
    
    void onEventMap(const char* eventId, const std::map<std::string,std::string>& map);
    
    void onEventMapValue(const char* eventId, const std::map<std::string,std::string>& map, int value);
};

#if (UMSDK_SWITCH == 1)

#define MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL(appkey, channel) umSDK::startWithAppkey(appkey, channel)
#define UMENG_PageStart(_ID_) umSDK::onPageStart(_ID_)
#define UMENG_PageEnd(_ID_) umSDK::onPageEnd(_ID_)
#define UMENG_EventBegin(_ID_) umSDK::onEventBegin(_ID_)
#define UMENG_EventBeginValue(_ID_, _VALUE_) umSDK::onEventBeginValue(_ID_, _VALUE_)
#define UMENG_EventEnd(_ID_) umSDK::onEventEnd(_ID_)
#define UMENG_EventEndValue(_ID_, _VALUE_) umSDK::onEventEndValue(_ID_, _VALUE_)
#define UMENG_Event(_ID_) umSDK::onEvent(_ID_)
#define UMENG_EventValue(_ID_, _VALUE_) umSDK::onEventValue(_ID_, _VALUE_)
#define UMENG_EventMap(_ID_, _MAP_) umSDK::onEventMap(_ID_, _MAP_)
#define UMENG_EventMapValue(_ID_, _MAP_, _VALUE_) umSDK::onEventMapValue(_ID_, _MAP_, _VALUE_)

#else

#define MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL(appkey, channel)
#define UMENG_PageStart(_ID_)
#define UMENG_PageEnd(_ID_)
#define UMENG_EventBegin(_ID_)
#define UMENG_EventBeginValue(_ID_, _VALUE_)
#define UMENG_EventEnd(_ID_)
#define UMENG_EventEndValue(_ID_, _VALUE_)
#define UMENG_Event(_ID_)
#define UMENG_EventValue(_ID_, _VALUE_)
#define UMENG_EventMap(_ID_, _MAP_)
#define UMENG_EventMapValue(_ID_, _MAP_, _VALUE_)

#endif

#endif /* defined(__umSDK__) */
