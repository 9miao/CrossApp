

#import <Foundation/Foundation.h>
#import "CCAccelerometerDelegate.h"
#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>

@interface AccelerometerDispatcher : NSObject<UIAccelerometerDelegate>
{
    CrossApp::CCAccelerometerDelegate *delegate_;
    CrossApp::CCAcceleration *acceleration_;
}

@property(readwrite) CrossApp::CCAccelerometerDelegate *delegate_;
@property(readwrite) CrossApp::CCAcceleration *acceleration_;

+ (id) sharedAccelerometerDispather;
- (id) init;
- (void) addDelegate: (CrossApp::CCAccelerometerDelegate *) delegate;
- (void) setAccelerometerInterval:(float)interval;

@end
