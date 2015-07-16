//
//  umSDK.cpp
//  HelloCpp
//
//  Created by 栗元峰 on 14-11-14.
//
//

#include "umSDK.h"
#include "MobClick.h"

namespace umSDK
{
    inline static NSString* Get_NSString(const char* string)
    {
        return [NSString stringWithUTF8String:string];
    }
    
    void startWithAppkey(const char* appkey, const char* channel)
    {
        NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
        [MobClick setAppVersion:version];
        
        [MobClick startWithAppkey:Get_NSString(appkey) reportPolicy:BATCH channelId:Get_NSString(channel)];
    }
    
    void onPageStart(const char* ViewID)
    {
        [MobClick beginLogPageView:Get_NSString(ViewID)];
    }
    
    void onPageEnd(const char* ViewID)
    {
        [MobClick endLogPageView:Get_NSString(ViewID)];
    }
    
    void onEventBegin(const char* eventId)
    {
        [MobClick beginEvent:Get_NSString(eventId)];
    }
    
    void onEventBeginValue(const char* eventId, const char* value)
    {
        [MobClick beginEvent:Get_NSString(eventId) label:Get_NSString(value)];
    }
    
    void onEventEnd(const char* eventId)
    {
        [MobClick endEvent:Get_NSString(eventId)];
    }
    
    void onEventEndValue(const char* eventId, const char* value)
    {
        [MobClick endEvent:Get_NSString(eventId) label:Get_NSString(value)];
    }
    
    void onEvent(const char* eventId)
    {
        [MobClick event:Get_NSString(eventId)];
    }
    
    void onEventValue(const char* eventId, const char* value)
    {
        [MobClick event:Get_NSString(eventId) label:Get_NSString(value)];
    }
    
    void onEventMap(const char* eventId, const std::map<std::string,std::string>& map)
    {
        
    }
    
    void onEventMapValue(const char* eventId, const std::map<std::string,std::string>& map, int value)
    {
        
    }
};