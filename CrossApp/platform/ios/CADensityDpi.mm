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

float CADensityDpi::getDensityDpi()
{
    float relust = 320;

    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *deviceString = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    
    NSArray *modelArray =
    @[
                            
                            @"i386",
                            @"x86_64",
                            
                            @"iPhone1,1",
                            @"iPhone1,2",
                            @"iPhone2,1",
                            
                            @"iPhone3,1",
                            @"iPhone3,2",
                            @"iPhone3,3",
                            @"iPhone4,1",
                            
                            @"iPhone5,1",
                            @"iPhone5,2",
                            @"iPhone5,3",
                            @"iPhone5,4",
                            @"iPhone6,1",
                            @"iPhone6,2",
                            
                            @"iPhone7,1",
                            @"iPhone7,2",
                            
                            @"iPod1,1",
                            @"iPod2,1",
                            @"iPod3,1",
                            @"iPod4,1",
                            @"iPod5,1",
                            
                            @"iPad1,1",
                            @"iPad2,1",
                            @"iPad2,2",
                            @"iPad2,3",
                            @"iPad2,4",
                            
                            @"iPad3,1",
                            @"iPad3,2",
                            @"iPad3,3",
                            
                            @"iPad3,4",
                            @"iPad3,5",
                            @"iPad3,6",
                            
                            @"iPad4,1",
                            @"iPad4,2",
                            @"iPad4,3",
                            
                            @"iPad2,5",
                            @"iPad2,6",
                            @"iPad2,7",
                            @"iPad4,4",
                            @"iPad4,5",
                            @"iPad4,6",
    ];
    
    NSArray *modelNameArray =
    @[
                                
                                @"iPhone Simulator",
                                @"iPhone Simulator",
                                
                                @"iPhone 2G",
                                @"iPhone 3G",
                                @"iPhone 3GS",
                                
                                @"iPhone 4",
                                @"iPhone 4",
                                @"iPhone 4",
                                @"iPhone 4S",
                                
                                @"iPhone 5",
                                @"iPhone 5",
                                @"iPhone 5C",
                                @"iPhone 5C",
                                @"iphone 5S",
                                @"iphone 5S",
                                
                                @"iphone 6 Plus",
                                @"iphone 6",
                                
                                @"iPod Touch 1G",
                                @"iPod Touch 2G",
                                @"iPod Touch 3G",
                                @"iPod Touch 4G",
                                @"iPod Touch 5G",
                                
                                @"iPad",
                                @"iPad 2",
                                @"iPad 2",
                                @"iPad 2",
                                @"iPad 2",
                                
                                @"iPad 3",
                                @"iPad 3",
                                @"iPad 3",
                                
                                @"iPad 4",
                                @"iPad 4",
                                @"iPad 4",
                                
                                @"iPad Air",
                                @"iPad Air",
                                @"iPad Air",

                                @"iPad mini",
                                @"iPad mini",
                                @"ipad mini",
                                
                                @"iPad mini 2",
                                @"iPad mini 2",
                                @"ipad mini 2"
    ];
    
    
    
    std::vector<float> densityDpis;
    
    densityDpis.push_back(320);//@"iPhone Simulator"
    densityDpis.push_back(320);//@"iPhone Simulator"
    
    densityDpis.push_back(160);//@"iPhone 2G"
    densityDpis.push_back(160);//@"iPhone 3G"
    densityDpis.push_back(160);//@"iPhone 3GS"
    
    densityDpis.push_back(320);//@"iPhone 4"
    densityDpis.push_back(320);//@"iPhone 4"
    densityDpis.push_back(320);//@"iPhone 4"
    densityDpis.push_back(320);//@"iPhone 4S"
    
    densityDpis.push_back(320);//@"iPhone 5"
    densityDpis.push_back(320);//@"iPhone 5"
    densityDpis.push_back(320);//@"iPhone 5C"
    densityDpis.push_back(320);//@"iPhone 5C"
    densityDpis.push_back(320);//@"iphone 5S"
    densityDpis.push_back(320);//@"iphone 5S"
    
    densityDpis.push_back(400);//@"iphone 6 Plus"
    densityDpis.push_back(320);//@"iphone 6"
    
    densityDpis.push_back(160);//@"iPod Touch 1G"
    densityDpis.push_back(160);//@"iPod Touch 2G"
    densityDpis.push_back(160);//@"iPod Touch 3G"
    densityDpis.push_back(320);//@"iPod Touch 4G"
    densityDpis.push_back(320);//@"iPod Touch 5G"
    
    densityDpis.push_back(160);//@"iPad"
    densityDpis.push_back(160);//@"iPad 2"
    densityDpis.push_back(160);//@"iPad 2"
    densityDpis.push_back(160);//@"iPad 2"
    densityDpis.push_back(160);//@"iPad 2"
    
    densityDpis.push_back(320);//@"iPad 3"
    densityDpis.push_back(320);//@"iPad 3"
    densityDpis.push_back(320);//@"iPad 3"
    
    densityDpis.push_back(320);//@"iPad 4"
    densityDpis.push_back(320);//@"iPad 4"
    densityDpis.push_back(320);//@"iPad 4"
    
    densityDpis.push_back(320);//@"iPad Air"
    densityDpis.push_back(320);//@"iPad Air"
    densityDpis.push_back(320);//@"iPad Air"
    
    densityDpis.push_back(160);//@"iPad mini"
    densityDpis.push_back(160);//@"iPad mini"
    densityDpis.push_back(160);//@"iPad mini"
    
    densityDpis.push_back(320);//@"iPad mini 2"
    densityDpis.push_back(320);//@"iPad mini 2"
    densityDpis.push_back(320);//@"iPad mini 2"
    
    
    NSInteger modelIndex = - 1;
    NSString *modelNameString = nil;
    modelIndex = [modelArray indexOfObject:deviceString];
    
    if (modelIndex >= 0 && modelIndex < [modelNameArray count])
    {
        modelNameString = [modelNameArray objectAtIndex:modelIndex];
        relust = densityDpis.at(modelIndex);
    }
    
    return relust;
}

NS_CC_END