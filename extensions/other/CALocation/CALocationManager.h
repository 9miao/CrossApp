//
//  CALocationManager.h
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#ifndef __iBook__CALocationManager__
#define __iBook__CALocationManager__

#include "CrossApp.h"

USING_NS_CC;
class CC_DLL CALocationDelegate
{
public:
    virtual void getLocations(CCDictionary *locations) = 0;
};
class CALocationManager:public CAObject
{
public:
    CALocationManager();
    
    virtual ~CALocationManager();
    
    static CALocationManager* create();
    
    CC_SYNTHESIZE(CALocationDelegate*, m_pLocationDelegate, LocationDelegate);
    
    void startLocation();
    
    void setLocation(double longt,double lat);

};
#endif /* defined(__iBook__CALocationManager__) */
