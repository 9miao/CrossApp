

#ifndef CADevice_h
#define CADevice_h

#include "basics/CAObject.h"
#include "basics/CAValue.h"
#include <functional>
NS_CC_BEGIN

class CC_DLL CADevice
{
public:
    
    enum class NetWorkData : int
    {
        Wifi = 0,
        ReachableViaWWAN,
        None,
    };
    
//    struct WifiDate
//    {
//        std::string ssid;
//        int level{0};
//        std::string mac;
//    };
    
    enum class VolumeData : int
    {
        Music = 0,
        System,
        Ring,
        VoicCall,
        Alarm,
        Notification
    };

    static const CAValueMap& getSystemVersion();

    static const std::string& getAppVersion();
        
    static void setScreenBrightness(float brightness);
    
    static float getScreenBrightness();
        
    static CADevice::NetWorkData getNetWorkType();
    
//    static CADevice::WifiDate getWifiConnectionInfo();
    
    static bool isNetWorkAvailble();
        
    static void setVolume(float sender, CADevice::VolumeData type);
    
    static float getVolume(CADevice::VolumeData type);
    
    static float getBatteryLevel();
        
    static void sendLocalNotification(const char* title, const char* content, int leftMessage);
        
    static void openUrl(const std::string &url, const std::function<void(bool)>& callback = nullptr);
        
    static void setIdleTimerDisabled(bool isIdleTimerDisabled);
};

NS_CC_END

#endif /* CADevice_h */
