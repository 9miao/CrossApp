//
//  CAAddressManager.h
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#ifndef __iBook__CAAddressManager__
#define __iBook__CAAddressManager__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
using namespace std;
class CAAddressBookRecord:public CCObject
{
public:
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
    
    CAAddressBookRecord()
    {
        firstName = "";
        lastName = "";
        middleName = "";
        prefix ="";
        suffix = "";
        nickname ="";
        firstNamePhonetic = "";
        lastNamePhonetic = "";
        middleNamePhonetic = "";
        organization = "";
        jobtitle = "";
        department = "";
        birthday = "";
        note = "";
        lastEdit = "";
        email = "";
        country = "";
        city = "";
        province = "";
        street = "";
        zip = "";
        countrycode = "";
        phoneNumber = "";
        fullname = "";
        
    }

    
};

class CAAddressManager:public CCObject
{
public:
    CAAddressManager();
    
    virtual ~CAAddressManager();
    
    static CAAddressManager *shareAddressManager();
    
    std::vector<CAAddressBookRecord*> _addressBookArr;
    
    std::vector<CAAddressBookRecord*> getAddressBook();
    
    void releaseJson(const char* json);
    
    CCArray *RecordArr;
    
};
#endif /* defined(__iBook__CAAddressManager__) */
