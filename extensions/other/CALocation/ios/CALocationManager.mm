//
//  CALocationManager.cpp
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#include "../CALocationManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "CALocation.h"
#endif
static CALocationManager *manger =NULL;
CALocationManager::CALocationManager()
{
    
}
CALocationManager::~CALocationManager()
{
    
}
CALocationManager* CALocationManager::create()
{
    if (!manger) {
        manger = new CALocationManager();
    }
    return manger;
}
void CALocationManager::startLocation()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CALocation *location = [[CALocation alloc] init];
    [location setLocation:this];
    [location startUpdatingLocation];
#endif
}


