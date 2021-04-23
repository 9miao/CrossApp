//
//  CADensityDpi.cpp
//  CrossApp
//
//  Created by 栗元峰 on 14-7-3.
//  Copyright (c) 2014年 厦门雅基软件有限公司. All rights reserved.
//

#include "platform/CADensityDpi.h"
#import <UIKit/UIKit.h>
#import "sys/utsname.h"
#include <vector>
NS_CC_BEGIN

static float s_fDPI = -1.f;

void CADensityDpi::setDensityDpi(float dpi)
{
    s_fDPI = dpi;
}

float CADensityDpi::getDensityDpi()
{
    if (s_fDPI < 0)
    {
        float scale = 1.0f;
        
        if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)])
        {
            scale = [[UIScreen mainScreen] scale];
        }
        
        s_fDPI = DPI_IPHONE_6S / 2 * scale;
        CCLog("\n*******************\nDEVICE DPI:%f\n*******************\n", s_fDPI);
    }
    return s_fDPI;
}

CADeviceIdiom CADensityDpi::getIdiom()
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        return CADeviceIdiom::iPad;
    }
    else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    {
        return CADeviceIdiom::iPhone;
    }
    return CADeviceIdiom::iPhone;
}


NS_CC_END
