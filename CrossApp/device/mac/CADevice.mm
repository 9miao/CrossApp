

#include "CADevice.h"
#import <Foundation/Foundation.h>
#import <AddressBook/AddressBook.h>
#import "Reachability.h"
#import <SystemConfiguration/CaptiveNetwork.h>
//#import <MediaPlayer/MediaPlayer.h>
#import <AppKit/AppKit.h>

NS_CC_BEGIN

const CAValueMap& CADevice::getSystemVersion()
{
    NSTask *task;
    task = [[NSTask alloc] init];
    [task setLaunchPath: @"/usr/bin/sw_vers"];
    
    NSArray *arguments;
    arguments = [NSArray arrayWithObjects: @"-productVersion", nil];
    [task setArguments: arguments];
    
    NSPipe *pipe;
    pipe = [NSPipe pipe];
    [task setStandardOutput: pipe];
    
    NSFileHandle *file;
    file = [pipe fileHandleForReading];
    
    [task launch];
    
    NSData *data;
    data = [file readDataToEndOfFile];
    
    NSString *string;
    string = [[NSString alloc] initWithData: data
                                   encoding: NSUTF8StringEncoding];
    
    CAValueMap valueMap;
    valueMap["os"] = "OS X";
    valueMap["version"] = [string UTF8String];
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
    
}

float CADevice::getScreenBrightness()
{
    return 0;
}

CADevice::NetWorkData CADevice::getNetWorkType()
{
    return CADevice::NetWorkData::Wifi;
}

bool CADevice::isNetWorkAvailble()
{
    return true;
}

//CADevice::WifiDate CADevice::getWifiConnectionInfo()
//{
//    CADevice::WifiDate wifiInfo;
//
//    return wifiInfo;
//}

void CADevice::setVolume(float sender, CADevice::VolumeData type)
{
}

float CADevice::getVolume(CADevice::VolumeData type)
{
    return 0;
}

float CADevice::getBatteryLevel()
{
    return 1.0f;
}

void CADevice::sendLocalNotification(const char* title, const char* content,int time)
{
}

void CADevice::openUrl(const std::string &url, const std::function<void(bool)>& callback)
{
    bool isOpen = [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
    if (callback)
    {
        callback(isOpen);
    }
}

void CADevice::setIdleTimerDisabled(bool isIdleTimerDisabled)
{
}

NS_CC_END
