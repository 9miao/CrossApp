//
//  CAGyroscope.mm
//  CrossApp
//
//  Created by 秦乐 on 2017/2/20.
//  Copyright © 2017年 . All rights reserved.
//

#include "CAMotionManager.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#import <CoreMotion/CoreMotion.h>
#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>

NS_CC_BEGIN

CMMotionManager* convert(void* var)
{
    return (CMMotionManager*)var;
}

CAMotionManager* CAMotionManager::getInstance()
{
    return CAApplication::getApplication()->getMotionManager();
}

CAMotionManager::CAMotionManager()
{
    m_pMotionManager = [[CMMotionManager alloc] init];
    
    convert(m_pMotionManager).deviceMotionUpdateInterval = 0.001f;
}

CAMotionManager::~CAMotionManager()
{
    [convert(m_pMotionManager) release];
}

void CAMotionManager::startGyroscope(const CAMotionManager::Callback& callback)
{
    if ([convert(m_pMotionManager) isGyroAvailable] == NO)
    {
        NSLog(@"Gyro is not available.");
        return;
    }
    
    m_callback = callback;

    if (convert(m_pMotionManager).accelerometerAvailable)
    {
        // 启动设备的运动更新，通过给定的队列向给定的处理程序提供数据。
        [convert(m_pMotionManager) startDeviceMotionUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMDeviceMotion *motion, NSError *error)
         {
             //convert(m_pMotionManager).deviceMotionUpdateInterval = 0.1f;
             
             static CAMotionManager::Data s_data;
             s_data.x = motion.gravity.x;
             s_data.y = motion.gravity.y;
             s_data.z = motion.gravity.z;
             s_data.timestamp = convert(m_pMotionManager).deviceMotionUpdateInterval;
             
             double tmp = s_data.x;
             
             switch ([[UIApplication sharedApplication] statusBarOrientation])
             {
                 case UIInterfaceOrientationLandscapeRight:
                     s_data.x = -s_data.y;
                     s_data.y = tmp;
                     break;
                     
                 case UIInterfaceOrientationLandscapeLeft:
                     s_data.x = s_data.y;
                     s_data.y = -tmp;
                     break;
                     
                 case UIInterfaceOrientationPortraitUpsideDown:
                     s_data.x = -s_data.y;
                     s_data.y = -tmp;
                     break;
                     
                 case UIInterfaceOrientationPortrait:
                     break;
                 default:
                     CCASSERT(false, "unknow orientation");
             }
             
             CAScheduler::getScheduler()->performFunctionInUIThread([&]()
             {
                 if (m_callback)
                 {
                     m_callback(s_data);
                 }
             });
             
             
         }];
    }
    else
    {
        
    }
}

void CAMotionManager::setGyroInterval(float interval)
{
    convert(m_pMotionManager).deviceMotionUpdateInterval = interval;
}

void CAMotionManager::stopGyroscope()
{
   [convert(m_pMotionManager) stopDeviceMotionUpdates];
}

NS_CC_END


