//
//  CADevice.cpp
//  CrossApp
//
//  Created by 秦乐 on 2017/2/21.
//  Copyright © 2017年 CossApp-x. All rights reserved.
//

#include "CADevice.h"
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AddressBook/AddressBook.h>
#import <AddressBookUI/AddressBookUI.h>
#import "CrossAppReachability.h"
#import <SystemConfiguration/CaptiveNetwork.h>
#import <MediaPlayer/MPMusicPlayerController.h>
#import <MediaPlayer/MediaPlayer.h>

#import "Availability.h"
#import <UserNotifications/UserNotifications.h>
#import <UserNotificationsUI/UserNotificationsUI.h>


NS_CC_BEGIN

const CAValueMap& CADevice::getSystemVersion()
{
    CAValueMap valueMap;
    valueMap["os"] = "iOS";
    valueMap["version"] = [[[UIDevice currentDevice]systemVersion] UTF8String];
    return valueMap;
}

const std::string& CADevice::getAppVersion()
{
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    NSString *app_Version = [infoDictionary objectForKey:@"CFBundleVersion"];
    return [app_Version UTF8String];
}

void CADevice::setScreenBrightness(float brightness)
{
    float brightnessPer = MIN(brightness, 1.0);
    [[UIScreen mainScreen] setBrightness:brightnessPer];
}

float CADevice::getScreenBrightness()
{
    float percent = [[UIScreen mainScreen] brightness];
    return percent;
}

CADevice::NetWorkData CADevice::getNetWorkType()
{
    CADevice::NetWorkData networkType = CADevice::NetWorkData::None;
    CrossAppReachability *reach = [CrossAppReachability reachabilityWithHostname:@"www.baidu.com"];
    switch ([reach currentReachabilityStatus])
    {
        case NotReachable:
            networkType = CADevice::NetWorkData::None;
            break;
        case ReachableViaWWAN:
            networkType = CADevice::NetWorkData::ReachableViaWWAN;
            break;
        case ReachableViaWiFi:
            networkType = CADevice::NetWorkData::Wifi;
            break;
    }
    return networkType;
}

bool CADevice::isNetWorkAvailble()
{
    CrossAppReachability *reach = [CrossAppReachability reachabilityWithHostname:@"www.baidu.com"];
    return [reach isReachable];
}

//CADevice::WifiDate CADevice::getWifiConnectionInfo()
//{
//    NSArray *array = (id)CNCopySupportedInterfaces();
//    id info = nil;
//    for (NSString *ifnam in array) {
//        info = (id)CNCopyCurrentNetworkInfo((CFStringRef)ifnam);
//        if (info && [info count]) {
//            
//            break;
//        }
//        [info release];
//    }
//    [array release];
//    NSDictionary *dic = info;
//    NSString *ssid = [[dic objectForKey:@"SSID"] lowercaseString];
//    NSString *mac = [[dic objectForKey:@"BSSID"] lowercaseString];
//    
//    CADevice::WifiDate wifiInfo;
//    wifiInfo.ssid = [ssid cStringUsingEncoding:NSUTF8StringEncoding];
//    wifiInfo.mac = [mac cStringUsingEncoding:NSUTF8StringEncoding];
//    wifiInfo.level = 0;
//    return wifiInfo;
//}

void CADevice::setVolume(float sender, CADevice::VolumeData type)
{
    [MPMusicPlayerController applicationMusicPlayer].volume = sender;
}

float CADevice::getVolume(CADevice::VolumeData type)
{
    return [MPMusicPlayerController applicationMusicPlayer].volume;
}

float CADevice::getBatteryLevel()
{
    [UIDevice currentDevice].batteryMonitoringEnabled = YES;
    return [UIDevice currentDevice].batteryLevel;
}

void CADevice::sendLocalNotification(const char* title, const char* message, int leftMessage)
{
    float version = [UIDevice currentDevice].systemVersion.floatValue;
    
    if (version >= 10)
    {
        //第二步：新建通知内容对象
        UNMutableNotificationContent *content = [[UNMutableNotificationContent alloc] init];
        //content.title = @"手持矿机";
        content.subtitle = [NSString stringWithUTF8String:title];
        content.body = [NSString stringWithUTF8String:message];
        content.badge = @0;
        UNNotificationSound *sound = [UNNotificationSound soundNamed:@"caodi.m4a"];
        content.sound = sound;
        
        //第三步：通知触发机制。（重复提醒，时间间隔要大于60s）
        UNTimeIntervalNotificationTrigger *trigger1 = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:1 repeats:NO];
        
        //第四步：创建UNNotificationRequest通知请求对象
        NSString *requertIdentifier = @"RequestIdentifier";
        UNNotificationRequest *request = [UNNotificationRequest requestWithIdentifier:requertIdentifier content:content trigger:trigger1];
        
        //第五步：将通知加到通知中心
        [[UNUserNotificationCenter currentNotificationCenter] addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {
            NSLog(@"Error:%@",error);
            
        }];
    }
    else
    {
        // 1.创建通知
        UILocalNotification *localNotification = [[UILocalNotification alloc] init];
        // 2.设置通知的必选参数
        // 设置通知显示的标题
        localNotification.alertTitle = [NSString stringWithUTF8String:title];
        // 设置通知显示的内容
        localNotification.alertBody = [NSString stringWithUTF8String:message];
        // 设置通知的发送时间,单位秒
        localNotification.fireDate = [NSDate dateWithTimeIntervalSinceNow:1];
        //解锁滑动时的事件
        localNotification.alertAction = @"";
        //收到通知时App icon的角标
        localNotification.applicationIconBadgeNumber = leftMessage;
        //推送是带的声音提醒，设置默认的字段为UILocalNotificationDefaultSoundName
        localNotification.soundName = UILocalNotificationDefaultSoundName;
        // 3.发送通知(? : 根据项目需要使用)
        // 方式二: 立即发送通知
        [[UIApplication sharedApplication] presentLocalNotificationNow:localNotification];
    }
}

void CADevice::openUrl(const std::string &url, const std::function<void(bool)>& callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    bool isOpen = [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
    if (callback)
    {
        callback(isOpen);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]] options:@{} completionHandler:^(BOOL success)
    {
        if (callback)
        {
            callback((success == YES) ? true : false);
        }
    }];
#endif
    
}

void CADevice::setIdleTimerDisabled(bool isIdleTimerDisabled)
{
    [[UIApplication sharedApplication] setIdleTimerDisabled:isIdleTimerDisabled];
}

NS_CC_END
