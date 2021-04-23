
#import <Foundation/Foundation.h>
#import <QuartzCore/CVDisplayLink.h>

@interface CAApplicationCaller : NSObject {
        CVDisplayLinkRef displayLink;
		NSTimer *renderTimer;
        int interval;
}
@property (readwrite) int interval;
-(void) startMainLoop;
-(void) end;
-(void) doCaller: (id) sender;
-(void) setAnimationInterval:(double)interval;

+(id) sharedCAApplicationCaller;
@end
