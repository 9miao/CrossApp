//
//  CAGyroDelegate.h
//  CrossApp
//
//  Created by 秦乐 on 16/5/11.
//  Copyright © 2016年 . All rights reserved.
//

#ifndef CAGyroDelegate_h
#define CAGyroDelegate_h


#include "CCCommon.h"

NS_CC_BEGIN

class CAGyroDate
{
public:
    double x;
    double y;
    double z;
    
    float timestamp;
};


class CC_DLL CAGyroDelegate
{
public:
    virtual void didGyroscope(CAGyroDate* pGyroValue) {CC_UNUSED_PARAM(pGyroValue);}
};

NS_CC_END


#endif /* CAGyroDelegate_h */
