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
#import "Reachability.h"
namespace CADevice
{

static std::vector<CAAddressBookRecord> _addressBookArr;

void openCamera(CAMediaDelegate* target)
{
    CACameraController *camera = [[CACameraController alloc] init];
    [camera setSender:target];
    [camera openCameraView];
}

void openAlbum(CAMediaDelegate* target)
{
    CAAlbumController *album = [[CAAlbumController alloc] init];
    [album setSender:target];
    [album openAlbumView];
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
    
void startLocation(CALocationDelegate* target)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CALocation *location = [[CALocation alloc] init];
    [location setLocation:target];
    [location startUpdatingLocation];
#endif
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
}
    
float getBatteryLevel()
{
    [UIDevice currentDevice].batteryMonitoringEnabled = YES;
    double deviceLevel = [UIDevice currentDevice].batteryLevel;
    return deviceLevel;
}
    
}

