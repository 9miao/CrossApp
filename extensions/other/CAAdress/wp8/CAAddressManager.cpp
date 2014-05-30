//
//  CAAddressManager.cpp
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#include "../CAAddressManager.h"

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

    return _addressBookArr;
    

}