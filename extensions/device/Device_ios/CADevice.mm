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
    
}
