//
//  CADevice.cpp
//  PublishPhoto
//
//  Created by lihui on 14-10-21.
//
//

#include "../CADevice.h"
#include "CACameraController.h"
#include "CAAlbumController.h"
#include "CAAddress.h"
#include "CALocation.h"
#include "CABrightness_iOS.h"
#import <MediaPlayer/MPMusicPlayerController.h>
#import <SystemConfiguration/CaptiveNetwork.h>
#import "CABlueTooth.h"
#import "Reachability.h"

NS_CC_BEGIN

namespace CADevice
{

static std::vector<CAAddressBookRecord> _addressBookArr;
    
const char* getSystemVersionWithIOS()
{
    return [[[UIDevice currentDevice]systemVersion] UTF8String];
}
    
const char* getAppVersion()
{
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    NSString *app_Version = [infoDictionary objectForKey:@"CFBundleShortVersionString"];
    return [app_Version UTF8String];
}

void openCamera(CAMediaDelegate* target)
{
    CACameraController *camera = [[CACameraController alloc] init];
    [camera setSender:target];
    [camera openCameraView:false];
}

void openAlbum(CAMediaDelegate* target)
{
    CAAlbumController *album = [[CAAlbumController alloc] init];
    [album setSender:target];
    [album openAlbumView:false];
}
    
void writeToSavedPhotosAlbum(const std::string &s)
{
    CAAlbumController *album = [[CAAlbumController alloc] init];
    [album writeImageToPhoto:s];

}
    
vector<CAAddressBookRecord> getAddressBook()
{
    if (_addressBookArr.size()>0) {
        return _addressBookArr;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CAAddress *address = [[CAAddress alloc] init];
    _addressBookArr =  [address getAddressBook];
#endif
    return _addressBookArr;
}

void OpenURL(const std::string &url)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
    
#endif
}
  
/* //EX
 
void startLocation(CALocationDelegate* target)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CALocation *location = [[CALocation alloc] init];
    [location setLocation:target];
    [location startUpdatingLocation];
#endif
}
 
*/
    
void sendLocalNotification(const char* title,const char* content,unsigned long time)
{
//    let _app = UIApplication.sharedApplication()
//    _app.registerUserNotificationSettings(UIUserNotificationSettings(forTypes: UIUserNotificationType.Alert|UIUserNotificationType.Badge|UIUserNotificationType.Sound, categories: nil));
//    [[UIApplication sharedApplication] registerUserNotificationSettings:[]]
    
    

    
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
        
        UIUserNotificationType myTypes = UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound;
        
        UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:myTypes categories:nil];
        
        [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
        
    }
    else
    {
        
        UIRemoteNotificationType myTypes = UIRemoteNotificationTypeBadge|UIRemoteNotificationTypeAlert|UIRemoteNotificationTypeSound;
        
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:myTypes];
        
    }
    
    UILocalNotification *notification = [[[UILocalNotification alloc] init] autorelease];
//    设置10秒之后
    NSDate *pushDate = [NSDate dateWithTimeIntervalSinceNow:10];
    if (notification != nil) {
        // 设置推送时间
        notification.fireDate = pushDate;
        // 设置时区
        notification.timeZone = [NSTimeZone defaultTimeZone];
        // 设置重复间隔
        notification.repeatInterval = kCFCalendarUnitDay;
        // 推送声音
        notification.soundName = UILocalNotificationDefaultSoundName;
        // 推送内容
        notification.alertBody = [NSString stringWithCString:content encoding:NSUTF8StringEncoding];
        //显示在icon上的红色圈中的数子
        notification.applicationIconBadgeNumber = 1;
        //设置userinfo 方便在之后需要撤销的时候使用
        NSDictionary *info = [NSDictionary dictionaryWithObject:@"name" forKey:@"key"];
        notification.userInfo = info;
        //添加推送到UIApplication
        UIApplication *app = [UIApplication sharedApplication];
        [app scheduleLocalNotification:notification]; 
        
    }
    
}

    
float getScreenBrightness()
{
    float percent = 0;
    percent = [[CABrightness_iOS alloc] getBrightnessPercent];
    return percent;
}
    
void setScreenBrightness(float brightness)
{
    float brightnessPer = MIN(brightness, 1.0);
    [[CABrightness_iOS alloc] setBrightnessPercent:brightnessPer];
}
    
void updateVersion(const std::string &url
                      ,unsigned int versionNumber
                      ,const std::string &appId)
{
    //itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=
    NSString *str = [NSString stringWithFormat:@"%s%s",url.c_str(),appId.c_str() ];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}
    
CANetWorkType getNetWorkType()
{
    CANetWorkType networkType = CANetWorkTypeNone;
    Reachability *reach = [Reachability reachabilityWithHostname:@"www.baidu.com"];
    switch ([reach currentReachabilityStatus])
    {
        case NotReachable:
            networkType=CANetWorkTypeNone;
            break;
        case ReachableViaWWAN:
            networkType=CANetWorkType3G;
            break;
        case ReachableViaWiFi:
            networkType=CANetWorkTypeWifi;
            break;
    }
    return networkType;
}
    
bool isNetWorkAvailble()
{
    Reachability *reach = [Reachability reachabilityWithHostname:@"www.baidu.com"];
    return [reach isReachable];
}
    
float getVolume(int type)
{
    return [MPMusicPlayerController applicationMusicPlayer].volume;
}
    
void setVolume(float sender,int type)
{
    [MPMusicPlayerController applicationMusicPlayer].volume = sender;
    //[MPMusicPlayerController applicationMusicPlayer]
}
    
float getBatteryLevel()
{
    [UIDevice currentDevice].batteryMonitoringEnabled = YES;
    double deviceLevel = [UIDevice currentDevice].batteryLevel;
    return deviceLevel;
}
void getWifiList(CAWifiDelegate *target)
{
    
    
}
CAWifiInfo getWifiConnectionInfo()
{
    NSArray *ifs = (id)CNCopySupportedInterfaces();
    id info = nil;
    for (NSString *ifnam in ifs) {
        info = (id)CNCopyCurrentNetworkInfo((CFStringRef)ifnam);
        if (info && [info count]) {
            
            break;
        }
        [info release];
    }
    [ifs release];
    NSDictionary *dic = info;
    NSString *ssid = [[dic objectForKey:@"SSID"] lowercaseString];
    NSString *mac = [[dic objectForKey:@"BSSID"] lowercaseString];
    CAWifiInfo wifiInfo;
    wifiInfo.ssid = [ssid cStringUsingEncoding:NSUTF8StringEncoding];
    wifiInfo.mac = [mac cStringUsingEncoding:NSUTF8StringEncoding];
    wifiInfo.level = 0;
    return wifiInfo;
}
 
void initBlueTooth(CABlueToothDelegate *target)
{
    target->getBlueToothState((CABlueToothState)[[[CABlueTooth alloc] init] getBlueToothState]);
}

void setBlueToothType(CABlueToothType type)
{
        
}
    
}

NS_CC_END
