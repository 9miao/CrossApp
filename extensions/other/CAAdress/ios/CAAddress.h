//
//  CAAddress.h
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#import <Foundation/Foundation.h>
#import <AddressBook/AddressBook.h>
#import <AddressBookUI/AddressBookUI.h>
#include "../CAAddressManager.h"

@interface CAAddress : NSObject
{
    
}
-(std::vector<CAAddressBookRecord*>)getAddressBook;
@end
