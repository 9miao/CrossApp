
#import <Foundation/Foundation.h>
#import <OpenGLES/EAGL.h>
#import "CAApplicationCaller.h"
#import "CAApplication.h"
#import "EAGLView.h"

static id s_sharedDirectorCaller;

@interface NSObject(CADisplayLink)
+(id) displayLinkWithTarget: (id)arg1 selector:(SEL)arg2;
-(void) addToRunLoop: (id)arg1 forMode: (id)arg2;
-(void) setFrameInterval: (int)interval;
-(void) invalidate;
@end

@implementation CAApplicationCaller

@synthesize interval;

+(id) sharedApplicationCaller
{
    if (s_sharedDirectorCaller == nil)
    {
        s_sharedDirectorCaller = [CAApplicationCaller new];
    }
    
    return s_sharedDirectorCaller;
}

+(void) destroy
{
    [s_sharedDirectorCaller release];
    s_sharedDirectorCaller = nil;
}

-(void) alloc
{
        interval = 1;
}

-(void) dealloc
{
    [displayLink release];
    [super dealloc];
}

-(void) startMainLoop
{
        // CCDirector::setAnimationInterval() is called, we should invalidate it first
        [displayLink invalidate];
        displayLink = nil;
        
        displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doCaller:)];
        [displayLink setFrameInterval: self.interval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) setAnimationInterval:(double)intervalNew
{
        // CCDirector::setAnimationInterval() is called, we should invalidate it first
        [displayLink invalidate];
        displayLink = nil;
        
        self.interval = 60.0 * intervalNew;
        
        displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doCaller:)];
        [displayLink setFrameInterval: self.interval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}
                      
-(void) doCaller: (id) sender
{
    if ([EAGLView sharedEGLView])
    {
        [EAGLContext setCurrentContext: [[EAGLView sharedEGLView] context]];
    }
    else
    {
        [displayLink release];
    }

    CrossApp::CAApplication::getApplication()->mainLoop();
}

@end
