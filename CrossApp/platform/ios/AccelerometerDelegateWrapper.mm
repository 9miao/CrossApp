

#import "AccelerometerDelegateWrapper.h"
#import <CoreMotion/CoreMotion.h>

@interface AccelerometerDispatcher ()

@property (strong, nonatomic) CMMotionManager *motionManager;

@end


@implementation AccelerometerDispatcher

static AccelerometerDispatcher* s_pAccelerometerDispatcher;

@synthesize delegate_;
@synthesize acceleration_;

+ (id) sharedAccelerometerDispather
{
    if (s_pAccelerometerDispatcher == nil) {
        s_pAccelerometerDispatcher = [[self alloc] init];
    }
    
    return s_pAccelerometerDispatcher;
}

- (id) init
{
    acceleration_ = new CrossApp::CAAcceleration();
    return self;
}

- (void) dealloc
{
    s_pAccelerometerDispatcher = 0;
    delegate_ = 0;
    delete acceleration_;
    [super dealloc];
}

- (void) addDelegate: (CrossApp::CAAccelerometerDelegate *) delegate
{
    delegate_ = delegate;
    
    _motionManager = [[CMMotionManager alloc] init];
    if (_motionManager.accelerometerAvailable)
    {
        // 启动设备的运动更新，通过给定的队列向给定的处理程序提供数据。
        [_motionManager startDeviceMotionUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMDeviceMotion *motion, NSError *error) {
            
            [self performSelectorOnMainThread:@selector(handleDeviceMotion:) withObject:motion waitUntilDone:YES];
        }];
    }
    else
    {
        [self setMotionManager:nil];
    }
}

-(void) setAccelerometerInterval:(float)interval
{
    // 提供设备运动数据到指定的时间间隔
    if(!_motionManager)
    {
        return;
    }
    _motionManager.deviceMotionUpdateInterval = interval;
}


- (void)handleDeviceMotion:(CMDeviceMotion *)deviceMotion
{
    if(!delegate_)
    {
        return;
    }
    
    acceleration_->x = deviceMotion.gravity.x;
    acceleration_->y = deviceMotion.gravity.y;
    acceleration_->z = deviceMotion.gravity.z;
    acceleration_->timestamp = _motionManager.deviceMotionUpdateInterval;
    
    delegate_->didAccelerate(acceleration_);
    
}

-(void) stopAccelerometerInterval
{
    [_motionManager stopDeviceMotionUpdates];
}

@end

