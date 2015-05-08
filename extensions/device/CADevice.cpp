//
//  CADevice.cpp
//  PublishPhoto
//
//  Created by lihui on 14-10-21.
//
//

#include "CADevice.h"

namespace CADevice
{
    void openCamera(CAMediaDelegate* target,bool allowEdit)
    {
        
    }
    
    void openAlbum(CAMediaDelegate* target,bool allowEdit)
    {
        
    }
    
    vector<CAAddressBookRecord> getAddressBook()
    {
        std::vector<CAAddressBookRecord> _addressBookArr;
        return _addressBookArr;
    }
    void startLocation(CALocationDelegate* target)
    {
        
    }
    
    float getScreenBrightness()
    {
        return 1.0f;
    }
    void setScreenBrightness(float brightness)
    {
        
    }
    
    void updateVersion(const std::string &url
                       ,unsigned int versionNumber
                       ,const std::string &appId)
    {
        
    }
    
    CANetWorkType getNetWorkType()
    {
        return CANetWorkTypeWifi;
    }
    
    void setVolume(float sender,int type)
    {
        
    }
    
    float getVolume(int type)
    {
        return 1.0f;
    }
    
    void OpenURL(const std::string &url)
    {
        
    }
    
    float getBatteryLevel()
    {
        return 0.0f;
    }
    
    bool isNetWorkAvailble()
    {
        return 0;
    }
    
    void sendLocalNotification(const char* title,const char* content,unsigned long time)
    {
        
    }
    
    CAWifiInfo getWifiConnectionInfo()
    {
        return CAWifiInfo();
    }
    
    void initBlueTooth(CABlueToothDelegate *target)
    {
        
    }
    
    void setBlueToothType(CABlueToothType type)
    {
        
    }
}