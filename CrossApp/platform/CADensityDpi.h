//
//  CADensityDpi.h
//  CrossApp
//
//  Created by 栗元峰 on 14-7-3.
//  Copyright (c) 2014年 厦门雅基软件有限公司. All rights reserved.
//

#ifndef __CrossApp__CADensityDpi__
#define __CrossApp__CADensityDpi__

#include <iostream>
#include "basics/CAApplication.h"

NS_CC_BEGIN

typedef enum
{
    CADeviceIdiomUnknown = -1,
    CADeviceIdiomPad = 0,
    CADeviceIdiomPhone = 1
}
CADeviceIdiom;


class CADensityDpi
{
public:
    
    static float getDensityDpi();
    
    static CADeviceIdiom getIdiom();
};

static inline float s_dip_to_px(float dip)
{
    return dip * (CADensityDpi::getDensityDpi() / 320.0f);
}

static inline float s_px_to_dip(float px)
{
    return px / (CADensityDpi::getDensityDpi() / 320.0f);
}

NS_CC_END

#endif /* defined(__CrossApp__CADensityDpi__) */
