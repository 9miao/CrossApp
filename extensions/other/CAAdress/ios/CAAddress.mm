//
//  CAAddress.m
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#import "CAAddress.h"

@implementation CAAddress
-(id)init
{
    if (![super init]) {
        return nil;
    }
    
    
    return self;
}
-(vector<CAAddressBookRecord*>)getAddressBook
{
    
    CFErrorRef *error = nil;
    ABAddressBookRef addressBook = ABAddressBookCreateWithOptions(NULL, error);            __block BOOL accessGranted = NO;
    if (ABAddressBookRequestAccessWithCompletion != NULL)
    { // we're on iOS 6
        dispatch_semaphore_t sema = dispatch_semaphore_create(0);
        ABAddressBookRequestAccessWithCompletion(addressBook, ^(bool granted, CFErrorRef error)
        {                accessGranted = granted;                dispatch_semaphore_signal(sema);
        });
        dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
    }
    else
    { // we're on iOS 5 or older
        
        accessGranted = YES;
    }
    
    std::vector<CAAddressBookRecord*> arr;
    
    if (accessGranted) {
        
        CFArrayRef results = ABAddressBookCopyArrayOfAllPeople(addressBook);
        
        for (int i = 0; i<CFArrayGetCount(results); i++) {
            ABRecordRef person = CFArrayGetValueAtIndex(results, i);
            CAAddressBookRecord *address = new CAAddressBookRecord();
            
            
            NSString *_firstName = (NSString*)ABRecordCopyValue(person, kABPersonFirstNameProperty);
            if (_firstName != nil)
                address->firstName = [_firstName cStringUsingEncoding:NSUTF8StringEncoding];
            
            NSString *lastName = (NSString*)ABRecordCopyValue(person, kABPersonLastNameProperty);
            if (lastName != nil)
                address->lastName = [lastName cStringUsingEncoding:NSUTF8StringEncoding];
            NSString *st = [NSString stringWithFormat:@"%@%@",lastName,_firstName];
            address->fullname =[st cStringUsingEncoding:NSUTF8StringEncoding];
            NSString *middlename = (NSString*)ABRecordCopyValue(person, kABPersonMiddleNameProperty);
            if (middlename != nil)
                address->middleName = [middlename cStringUsingEncoding:NSUTF8StringEncoding];
            
            
            NSString *prefix = (NSString*)ABRecordCopyValue(person, kABPersonPrefixProperty);
            
            if(prefix != nil)
                address->prefix = [prefix cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取suffix后缀
            
            NSString *suffix = (NSString*)ABRecordCopyValue(person, kABPersonSuffixProperty);
            
            if(suffix != nil)
                address->suffix = [suffix cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取nickname呢称
            
            NSString *nickname = (NSString*)ABRecordCopyValue(person, kABPersonNicknameProperty);
            
            if(nickname != nil)
                address->nickname = [nickname cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取firstname拼音音标
            
            NSString *firstnamePhonetic = (NSString*)ABRecordCopyValue(person, kABPersonFirstNamePhoneticProperty);
            
            if(firstnamePhonetic != nil)
                address->firstNamePhonetic = [firstnamePhonetic cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取lastname拼音音标
            
            NSString *lastnamePhonetic = (NSString*)ABRecordCopyValue(person, kABPersonLastNamePhoneticProperty);
            
            if(lastnamePhonetic != nil)
                address->lastNamePhonetic = [lastnamePhonetic cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取middlename拼音音标
            
            NSString *middlenamePhonetic = (NSString*)ABRecordCopyValue(person, kABPersonMiddleNamePhoneticProperty);
            
            if(middlenamePhonetic != nil)
                address->middleNamePhonetic = [middlenamePhonetic cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取organization公司
            
            NSString *organization = (NSString*)ABRecordCopyValue(person, kABPersonOrganizationProperty);
            
            if(organization != nil)
                address->organization = [organization cStringUsingEncoding:NSUTF8StringEncoding];
            
            
            //读取jobtitle工作
            
            NSString *jobtitle = (NSString*)ABRecordCopyValue(person, kABPersonJobTitleProperty);
            
            if(jobtitle != nil)
                
                address->jobtitle = [jobtitle cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取department部门
            
            NSString *department = (NSString*)ABRecordCopyValue(person, kABPersonDepartmentProperty);
            
            if(department != nil)
                
                address->department = [department cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取birthday生日
            
            NSDate *birthday = (NSDate*)ABRecordCopyValue(person, kABPersonBirthdayProperty);
            
            if(birthday != nil)
                
                address->birthday = [department cStringUsingEncoding:NSUTF8StringEncoding];
            
            //读取note备忘录
            
            NSString *note = (NSString*)ABRecordCopyValue(person, kABPersonNoteProperty);
            
            if(note != nil)
                
                address->note = [note cStringUsingEncoding:NSUTF8StringEncoding];
            
            //第一次添加该条记录的时间
            
            NSString *firstknow = (NSString*)ABRecordCopyValue(person, kABPersonCreationDateProperty);
            
            NSLog(@"第一次添加该条记录的时间%@\n",firstknow);
            
            //最后一次修改該条记录的时间
            
            NSString *lastknow = (NSString*)ABRecordCopyValue(person, kABPersonModificationDateProperty);
            
            NSLog(@"最后一次修改該条记录的时间%@\n",lastknow);
            
            
            
            //获取email多值
            
            ABMultiValueRef email = ABRecordCopyValue(person, kABPersonEmailProperty);
            
            int emailcount = ABMultiValueGetCount(email);
            
            for (int x = 0; x < emailcount; x++)
                
            {
                
                
                //获取email值
                
                NSString* emailContent = (NSString*)ABMultiValueCopyValueAtIndex(email, x);
                
                address->email = [emailContent cStringUsingEncoding:NSUTF8StringEncoding];
                
            }
            
            
            ABMultiValueRef addresses = ABRecordCopyValue(person, kABPersonAddressProperty);
            
            int count = ABMultiValueGetCount(addresses);
            
            
            
            for(int j = 0; j < count; j++)
                
            {
                
                
                //获取該label下的地址6属性
                
                NSDictionary* personaddress =(NSDictionary*) ABMultiValueCopyValueAtIndex(addresses, j);
                
                NSString* country = [personaddress valueForKey:(NSString *)kABPersonAddressCountryKey];
                
                if(country != nil)
                    
                    address->country = [country cStringUsingEncoding:NSUTF8StringEncoding];
                
                NSString* city = [personaddress valueForKey:(NSString *)kABPersonAddressCityKey];
                
                if(city != nil)
                    
                    address->city = [city cStringUsingEncoding:NSUTF8StringEncoding];
                
                NSString* state = [personaddress valueForKey:(NSString *)kABPersonAddressStateKey];
                
                if(state != nil)
                    
                    address->province = [state cStringUsingEncoding:NSUTF8StringEncoding];
                
                NSString* street = [personaddress valueForKey:(NSString *)kABPersonAddressStreetKey];
                
                if(street != nil)
                    
                    address->street = [street cStringUsingEncoding:NSUTF8StringEncoding];
                
                NSString* zip = [personaddress valueForKey:(NSString *)kABPersonAddressZIPKey];
                
                if(zip != nil)
                    
                    address->zip = [zip cStringUsingEncoding:NSUTF8StringEncoding];
                
                NSString* coutntrycode = [personaddress valueForKey:(NSString *)kABPersonAddressCountryCodeKey];
                
                if(coutntrycode != nil)
                    
                    address->countrycode = [coutntrycode cStringUsingEncoding:NSUTF8StringEncoding];
                
            }
            ABMultiValueRef phone = ABRecordCopyValue(person, kABPersonPhoneProperty);
            
            for (int k = 0; k<ABMultiValueGetCount(phone); k++)
                
            {
                //获取該Label下的电话值
                
                NSString * personPhone = (NSString*)ABMultiValueCopyValueAtIndex(phone, k);
                
                
                
                address->phoneNumber = [personPhone cStringUsingEncoding:NSUTF8StringEncoding];
                
            }
            
            
            
            
            arr.push_back(address);
            
            
            
        }

    }


    return arr;
}
@end
