//
//  CALocation.m
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#import "CAGyroscope.h"
#include "../CADevice.h"
#import <CoreMotion/CoreMotion.h>

@interface CAGyroscope ()

@property (strong, nonatomic) CMMotionManager *motionManager;

@end

@implementation CAGyroscope

static CAGyroscope* m_pGyroscope = NULL;

-(id)init
{
    if (![super init])
    {
        return nil;
    }
    
    _motionManager = [[CMMotionManager alloc]init];
    
    _motionManager.gyroUpdateInterval = 0.001f;
    
    gyroDate = new CrossApp::CAGyroDate();

    return self;
}

- (void) dealloc
{
    delegate_ = 0;
    
    delete gyroDate;
    
    [super dealloc];
}

+ (CAGyroscope *)sharedGyroscope
{
    if( m_pGyroscope == NULL )
        m_pGyroscope = [[CAGyroscope alloc] init];
    
    return m_pGyroscope;
}

-(void)addGyroDelegate:(CrossApp::CAGyroDelegate*)delegate
{
    NSLog(@"Gyro is  available.");
    
    delegate_ = delegate;
    
    if ([_motionManager isGyroAvailable] == YES) {
        
        [_motionManager setGyroUpdateInterval:_motionManager.gyroUpdateInterval];
        
        [_motionManager startGyroUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMGyroData *gyroData, NSError *error) {
            [self performSelectorOnMainThread:@selector(handleDeviceMotion:) withObject:gyroData waitUntilDone:YES];
        }];
    }
    else {
        NSLog(@"Gyro is not available.");
        [self setMotionManager:nil];
    }
}

-(void)setGyroInterval:(float)fValue
{
    if(!_motionManager)
        return;
    
    _motionManager.gyroUpdateInterval = fValue;
}

- (void)handleDeviceMotion:(CMGyroData *)deviceDate
{
    if(!delegate_)
    {
        return;
    }
    
    gyroDate->x = deviceDate.rotationRate.x;
    gyroDate->y = deviceDate.rotationRate.y;
    gyroDate->z = deviceDate.rotationRate.z;
    gyroDate->timestamp = _motionManager.gyroUpdateInterval;
    
    delegate_->didGyroscope(gyroDate);
    
}

-(void)stopUpdatingGyroscope
{
    [_motionManager stopGyroUpdates];
}


@end
