
#import <Foundation/Foundation.h>
#import "CAApplicationCaller.h"
#import "CAApplication.h"
#import "EAGLView.h"
#import "CCEventDispatcher.h"
#include "CAAutoreleasePool.h"

static id s_sharedDirectorCaller;

@interface NSObject(CADisplayLink)
+(id) displayLinkWithTarget: (id)arg1 selector:(SEL)arg2;
-(void) addToRunLoop: (id)arg1 forMode: (id)arg2;
-(void) setFrameInterval: (int)interval;
-(void) invalidate;
@end

@implementation CAApplicationCaller

@synthesize interval;

+(id) sharedCAApplicationCaller
{
	if (s_sharedDirectorCaller == nil)
	{
		s_sharedDirectorCaller = [[CAApplicationCaller alloc] init];
	}
	
	return s_sharedDirectorCaller;
}

-(void) alloc
{
    interval = 1;
}

-(void) dealloc
{
    s_sharedDirectorCaller = nil;
    CCLOG("CrossApp: deallocing CAApplicationCaller %p", self);
	if (displayLink) {
		CVDisplayLinkRelease(displayLink);
	}
    
    CCLOG("--------------------------------------------------------------------------------");
    CCLOG("");
    CCLOG("");
    CCLOG("");
	[super dealloc];
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
#if CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
	//if( ! runningThread_ )
	//runningThread_ = [NSThread currentThread];
    
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	CrossApp::CAPoolManager::sharedPoolManager()->pop();
	[[CCEventDispatcher sharedDispatcher] dispatchQueuedEvents];
	
	[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:nil];
	
	// release the objects
	[pool release];
	
#else
	[self performSelector:@selector(drawScene) onThread:[NSThread currentThread] withObject:nil waitUntilDone:YES];
#endif
	
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    //    CVReturn result = [(CAApplicationCaller*)displayLinkContext getFrameForTime:outputTime];
    //    return result;
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	CrossApp::CAApplication::getApplication()->mainLoop();
	[pool release];
    
	return kCVReturnSuccess;
}

- (void)timerFired:(id)sender
{
    // It is good practice in a Cocoa application to allow the system to send the -drawRect:
    // message when it needs to draw, and not to invoke it directly from the timer.
    // All we do here is tell the display it needs a refresh
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
	// get the opengl view
	EAGLView *openGLView = [EAGLView sharedEGLView];
	[openGLView lockOpenGLContext];
    
	// run the main CrossApp loop
	CrossApp::CAApplication::getApplication()->mainLoop();
    
	// flush buffer (this line is very important!)
	[[openGLView openGLContext] flushBuffer];
	
	[openGLView unlockOpenGLContext];
    
	// send any queued events
	[[CCEventDispatcher sharedDispatcher] dispatchQueuedEvents];
    
	[pool release];
}

-(void) startMainLoop
{
    // CAApplication::setAnimationInterval() is called, we should invalide it first
    //        [displayLink invalidate];
    //        displayLink = nil;
    //
    //        displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doCaller:)];
    //        [displayLink setFrameInterval: self.interval];
    //        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
#if ! CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
	NSThread* thread = [[NSThread alloc] initWithTarget:self selector:@selector(mainLoop) object:nil];
	[thread start];
#endif
	// NSTimer
	[renderTimer invalidate];
	renderTimer = nil;
    
	renderTimer = [NSTimer timerWithTimeInterval:self.interval/60.0f   //a 1ms time interval
                                          target:self
                                        selector:@selector(timerFired:)
                                        userInfo:nil
                                         repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:renderTimer
                                 forMode:NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] addTimer:renderTimer
                                 forMode:NSEventTrackingRunLoopMode]; //Ensure timer fires during resize

}

-(void) end
{
    [renderTimer invalidate];
	renderTimer = nil;
    [self release];
}

-(void) setAnimationInterval:(double)intervalNew
{
	self.interval = 60.0 * intervalNew;
	[renderTimer invalidate];
	renderTimer = nil;
	renderTimer = [NSTimer timerWithTimeInterval:self.interval/60.0f   //a 1ms time interval
										  target:self
										selector:@selector(timerFired:)
										userInfo:nil
										 repeats:YES];
	
	[[NSRunLoop currentRunLoop] addTimer:renderTimer 
								 forMode:NSDefaultRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:renderTimer 
								 forMode:NSEventTrackingRunLoopMode];
}

-(void) doCaller: (id) sender
{
	CrossApp::CAApplication::getApplication()->mainLoop();
}

@end
