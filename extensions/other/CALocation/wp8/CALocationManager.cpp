//
//  CALocationManager.cpp
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#include "../CALocationManager.h"

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

}


