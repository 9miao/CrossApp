

#import "AccelerometerDelegateWrapper.h"

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
    acceleration_ = new CrossApp::CCAcceleration();
    return self;
}

- (void) dealloc
{
    s_pAccelerometerDispatcher = 0;
    delegate_ = 0;
    delete acceleration_;
    [super dealloc];
}

- (void) addDelegate: (CrossApp::CCAccelerometerDelegate *) delegate
{
    delegate_ = delegate;
    
    if (delegate_)
    {
        [[UIAccelerometer sharedAccelerometer] setDelegate:self];
    }
    else 
    {
        [[UIAccelerometer sharedAccelerometer] setDelegate:nil];
    }
}

-(void) setAccelerometerInterval:(float)interval
{
    [[UIAccelerometer sharedAccelerometer] setUpdateInterval:interval];
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{   
    if (! delegate_)
    {
        return;
    }
    
    acceleration_->x = acceleration.x;
    acceleration_->y = acceleration.y;
    acceleration_->z = acceleration.z;
    acceleration_->timestamp = acceleration.timestamp;
    
    double tmp = acceleration_->x;
    
    switch ([[UIApplication sharedApplication] statusBarOrientation]) 
    {
    case UIInterfaceOrientationLandscapeRight:
        acceleration_->x = -acceleration_->y;
        acceleration_->y = tmp;
        break;
        
    case UIInterfaceOrientationLandscapeLeft:
        acceleration_->x = acceleration_->y;
        acceleration_->y = -tmp;
        break;
        
    case UIInterfaceOrientationPortraitUpsideDown:
        acceleration_->x = -acceleration_->y;
        acceleration_->y = -tmp;
        break;
            
    case UIInterfaceOrientationPortrait:
        break;
    }
    
    delegate_->didAccelerate(acceleration_);
}

@end

