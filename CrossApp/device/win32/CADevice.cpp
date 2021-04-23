//
//  CADevice.cpp
//  CrossApp
//
//  Created by 秦乐 on 2017/2/21.
//  Copyright © 2017年 cocos2d-x. All rights reserved.
//

#include "../CADevice.h"

NS_CC_BEGIN

const CAValueMap& CADevice::getSystemVersion()
{
    CAValueMap valueMap;
    valueMap["os"] = "Windows";
    valueMap["version"] = "Windows";
    return valueMap;
}

const std::string& CADevice::getAppVersion()
{
    return "";
}

void CADevice::setScreenBrightness(float brightness)
{
}

float CADevice::getScreenBrightness()
{
    return 0.f;
}

CADevice::NetWorkData CADevice::getNetWorkType()
{
    return NetWorkData::Wifi;
}

bool CADevice::isNetWorkAvailble()
{
    return true;
}

void CADevice::setVolume(float sender, CADevice::VolumeData type)
{
}

float CADevice::getVolume(CADevice::VolumeData type)
{
    return 0.0f;
}

float CADevice::getBatteryLevel()
{
    return 0.f;
}

void CADevice::sendLocalNotification(const char* title, const char* content,int time)
{
}

void CADevice::openUrl(const std::string &url, const std::function<void(bool)>& callback)
{
    int errCode = ::ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOW);
    if (callback)
    {
        callback(bool(errCode > 32));
    }
}

void CADevice::setIdleTimerDisabled(bool isIdleTimerDisabled)
{
    
}

NS_CC_END
