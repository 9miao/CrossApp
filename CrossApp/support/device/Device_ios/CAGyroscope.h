//
//  CAGryoscope.h
//  CrossApp
//
//  Created by 秦 on 16/5/11.
//  Copyright © 2016年 cocos2d-x. All rights reserved.
//

#ifndef CAGyroscope_h
#define CAGyroscope_h

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "CAGyroDelegate.h"

@interface CAGyroscope : NSObject
{
    CrossApp::CAGyroDelegate *delegate_;
    CrossApp::CAGyroDate *gyroDate;
}

+ (CAGyroscope *)sharedGyroscope;

-(void)addGyroDelegate:(CrossApp::CAGyroDelegate*)delegate;

-(void)setGyroInterval:(float)fValue;

-(void)stopUpdatingGyroscope;

@end

#endif /* CAGryoscope_h */
