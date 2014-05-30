//
//  CAAddressManager.cpp
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#include "../CAAddressManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "CAAddress.h"
#endif
static CAAddressManager *manger = NULL;
CAAddressManager::CAAddressManager()
:_addressBookArr(NULL)
{
    RecordArr= CCArray::create();
    RecordArr->retain();
}
CAAddressManager::~CAAddressManager()
{
    
}
CAAddressManager *CAAddressManager::shareAddressManager()
{
    if (!manger) {
        manger = new CAAddressManager();
        
    
    }
    return manger;
}

vector<CAAddressBookRecord*> CAAddressManager::getAddressBook()
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