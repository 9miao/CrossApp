//
//  CADevice.h
//  PublishPhoto
//
//  Created by lihui on 14-10-21.
//
//

#ifndef __PublishPhoto__CADevice__
#define __PublishPhoto__CADevice__

#include <iostream>
#include "CrossApp.h"
#include "CrossAppExt.h"
USING_NS_CC;
USING_NS_CC_EXT;
class CAMediaDelegate
{
public:
    virtual ~CAMediaDelegate(){};
    
    virtual void getSelectedImage(CAImage *image) = 0;
};

class CALocationDelegate
{
public:
    virtual ~CALocationDelegate(){};
    
    virtual void getLocations(CCDictionary *locations) = 0;
};

struct CAAddressBookRecord
{
    string firstName;
    string lastName;
    string middleName;
    string prefix;
    string suffix;
    string nickname;
    string firstNamePhonetic;
    string lastNamePhonetic;
    string middleNamePhonetic;
    string organization;
    string jobtitle;
    string department;
    string birthday;
    string note;
    string lastEdit;
    string email;
    string country;
    string city;
    string province;
    string street;
    string zip;
    string countrycode;
    string phoneNumber;
    string fullname;
};

typedef enum
{
    CANetWorkTypeWifi=0,
    CANetWorkType3G,
    CANetWorkTypeNone
    
}CANetWorkType;

namespace CADevice
{
   
    void openCamera(CAMediaDelegate* target);
    
    void openAlbum(CAMediaDelegate* target);
    
    void startLocation(CALocationDelegate* target);
    
    float getScreenBrightness();
    
    void setScreenBrightness(float brightness);
    
    void writeToSavedPhotosAlbum(const std::string &s);
    
    std::vector<CAAddressBookRecord> getAddressBook();
    
    void updateVersion(const std::string &url
                      ,unsigned int versionNumber
                      ,const std::string &appId);
    CANetWorkType getNetWorkType();
    
    
    void setVolume(float sender,int type);
    
    float getVolume(int type);
    
    void OpenURL(const std::string &url);
    
    float getBatteryLevel();
    
    bool isNetWorkAvailble();
};
#endif /* defined(__PublishPhoto__CADevice__) */
