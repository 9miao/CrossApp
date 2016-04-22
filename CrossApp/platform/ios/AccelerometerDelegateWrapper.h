
#import <Foundation/Foundation.h>
#import "CAAccelerometerDelegate.h"
#import <UIKit/UIKit.h>

@interface AccelerometerDispatcher : NSObject<UIAccelerometerDelegate>
{
    CrossApp::CAAccelerometerDelegate *delegate_;
    CrossApp::CAAcceleration *acceleration_;
    
}

@property(readwrite) CrossApp::CAAccelerometerDelegate *delegate_;
@property(readwrite) CrossApp::CAAcceleration *acceleration_;

+ (id) sharedAccelerometerDispather;
- (id) init;
- (void) addDelegate: (CrossApp::CAAccelerometerDelegate *) delegate;
- (void) setAccelerometerInterval:(float)interval;
- (void) stopAccelerometerInterval;

@end