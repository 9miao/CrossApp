//
//  CADensityDpi.h
//  CrossApp
//
//  Created by 栗元峰 on 14-7-3.
//  Copyright (c) 2014年 厦门雅基软件有限公司. All rights reserved.
//

#ifndef __CrossApp__CADensityDpi__
#define __CrossApp__CADensityDpi__

#include "CrossApp-Prefix.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

enum class CADeviceIdiom : int
{
    Unknown = -1,
    iPad = 0,
    iPhone = 1
};


class CC_DLL CADensityDpi
{
public:
    
    static float getDensityDpi();
    
    static CADeviceIdiom getIdiom();
    
    static void setDensityDpi(float dpi);
};


static inline float s_dip_to_px(float dip)
{
    return dip * (CADensityDpi::getDensityDpi() / DPI_STANDARD);
}

static inline float s_px_to_dip(float px)
{
    return px / (CADensityDpi::getDensityDpi() / DPI_STANDARD);
}

NS_CC_END

#endif /* defined(__CrossApp__CADensityDpi__) */
