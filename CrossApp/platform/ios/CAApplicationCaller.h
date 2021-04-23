
#import <Foundation/Foundation.h>

@interface CAApplicationCaller : NSObject {
        id displayLink;
        int interval;
}
@property (readwrite) int interval;
-(void) startMainLoop;
-(void) doCaller: (id) sender;
-(void) setAnimationInterval:(double)interval;
+(id) sharedApplicationCaller;
+(void) destroy;
@end
