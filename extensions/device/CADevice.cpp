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
    void openCamera(CAMediaDelegate* target)
    {
        
    }
    
    void openAlbum(CAMediaDelegate* target)
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
        
    }
    
    void setVolume(float sender,int type)
    {
        
    }
    
    float getVolume(int type)
    {
        
    }
    
    void OpenURL(const std::string &url)
    {
        
    }
    
    float getBatteryLevel()
    {
        
    }
    
    bool isNetWorkAvailble()
    {
        
    }
    
    void sendLocalNotification(const char* title,const char* content,unsigned long time)
    {
        
    }
    
    CAWifiInfo getWifiConnectionInfo()
    {
        
    }
    
    void initBlueTooth(CABlueToothDelegate *target)
    {
        
    }
    
    void setBlueToothType(CABlueToothType type)
    {
        
    }
}