
#import <Availability.h>

#import "EAGLView.h"
#import "CCEGLView.h"
#import <OpenGL/gl.h>
#import "basics/CAApplication.h"
#import "ccConfig.h"
#import "CCSet.h"
#import "CATouch.h"
#import "CCWindow.h"
#import "CCEventDispatcher.h"
#include "CADensityDpi.h"
#include "CCApplication.h"

//USING_NS_CC;
static EAGLView *view;

@implementation EAGLView

@synthesize eventDelegate = eventDelegate_, isFullScreen = isFullScreen_;

+(id) sharedEGLView
{
	return view;
}

- (id) initWithFrame:(NSRect)frameRect
{
	self = [self initWithFrame:frameRect shareContext:nil];
	return self;
}

- (id) initWithFrame:(NSRect)frameRect shareContext:(NSOpenGLContext*)context
{
    NSOpenGLPixelFormatAttribute attribs[] =
    {
//		NSOpenGLPFAAccelerated,
//		NSOpenGLPFANoRecovery,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 24,
		
		0
    };
	
    view = self;
    
	NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	
	if (!pixelFormat)
		NSLog(@"No OpenGL pixel format");
	
	if((self = [super initWithFrame:frameRect pixelFormat:[pixelFormat autorelease]]))
    {
        if(context)
        {
            [self setOpenGLContext:context];
        }
		eventDelegate_ = [CCEventDispatcher sharedDispatcher];
	}
    
    CrossApp::CCEGLView::sharedOpenGLView()->setFrameSize(frameRect.size.width, frameRect.size.height);
    
	return self;
}

- (id) initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format{
    // event delegate
    eventDelegate_ = [CCEventDispatcher sharedDispatcher];
    
	
	view = self;

    [super initWithFrame:frameRect pixelFormat:format];

    CrossApp::CCEGLView::sharedOpenGLView()->setFrameSize(frameRect.size.width, frameRect.size.height);
    
    [self lockOpenGLContext];
    
    NSRect bounds = frameRect;
    bounds.origin = CGPointZero;
    
    NSTrackingArea* trackingArea = [[NSTrackingArea alloc]
                    initWithRect:bounds
                    options: NSTrackingMouseMoved | NSTrackingActiveAlways
                    owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
    
    return self;
}

- (void) update
{
	// XXX: Should I do something here ?
	[super update];
}

- (void) prepareOpenGL
{
	// XXX: Initialize OpenGL context

	[super prepareOpenGL];
	
	// Make this openGL context current to the thread
	// (i.e. all openGL on this thread calls will go to this context)
	[[self openGLContext] makeCurrentContext];
	
	// Synchronize buffer swaps with vertical refresh rate
	GLint swapInt = 1;
	[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];	

//	GLint order = -1;
//	[[self openGLContext] setValues:&order forParameter:NSOpenGLCPSurfaceOrder];
}

- (NSUInteger) depthFormat
{
	return 24;
}

- (void) reshape
{
	// We draw on a secondary thread through the display link
	// When resizing the view, -reshape is called automatically on the main thread
	// Add a mutex around to avoid the threads accessing the context simultaneously when resizing

	[self lockOpenGLContext];
	//NO_FINISH
	NSRect rect = [self bounds];
	CGSize size = NSSizeToCGSize(rect.size);
    CrossApp::CCEGLView::sharedOpenGLView()->setFrameSize(size.width, size.height);
	
    if (NSTrackingArea* trackingArea = [self.trackingAreas objectAtIndex:0])
    {
        [self removeTrackingArea:trackingArea];
        trackingArea = [[NSTrackingArea alloc]
                        initWithRect:rect
                        options: NSTrackingMouseMoved | NSTrackingActiveAlways
                        owner:self userInfo:nil];
        [self addTrackingArea:trackingArea];
    }
    
	// avoid flicker
//	[self setNeedsDisplay:YES];
	[self unlockOpenGLContext];
}

-(void) lockOpenGLContext
{
	NSOpenGLContext *glContext = [self openGLContext];
	NSAssert( glContext, @"FATAL: could not get openGL context");

	[glContext makeCurrentContext];
	CGLLockContext((CGLContextObj)[glContext CGLContextObj]);
}

-(void) unlockOpenGLContext
{
	NSOpenGLContext *glContext = [self openGLContext];
	NSAssert( glContext, @"FATAL: could not get openGL context");

	CGLUnlockContext((CGLContextObj)[glContext CGLContextObj]);
}

- (void) dealloc
{
	CCLOG(@"ERROE!!! CrossApp: deallocing EAGLView %@", self);
	[super dealloc];
    if(windowGLView_)
       [windowGLView_ release];
    if (fullScreenWindow_)
        [fullScreenWindow_ close];
}
	
-(int) getWidth
{
	NSSize bound = [self bounds].size;
	return bound.width;
}

-(int) getHeight
{
	NSSize bound = [self bounds].size;
	return bound.height;
}

-(void) swapBuffers
{
}

//
// setFullScreen code taken from GLFullScreen example by Apple
//
- (void) setFullScreen:(BOOL)fullscreen
{
	// Mac OS X 10.6 and later offer a simplified mechanism to create full-screen contexts
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_5

    if (isFullScreen_ == fullscreen)
		return;

	EAGLView *openGLview = [[self class] sharedEGLView];

    if( fullscreen ) {
        originalWinRect_ = [openGLview frame];

        // Cache normal window and superview of openGLView
        if(!windowGLView_)
            windowGLView_ = [[openGLview window] retain];

        [superViewGLView_ release];
        superViewGLView_ = [[openGLview superview] retain];


        // Get screen size
        NSRect displayRect = [[NSScreen mainScreen] frame];

        // Create a screen-sized window on the display you want to take over
        if (fullScreenWindow_ != nil)
        {
            [fullScreenWindow_ close];
            fullScreenWindow_ = nil;

        }

        fullScreenWindow_ = [[CCWindow alloc] initWithFrame:displayRect fullscreen:YES];
       
        // Remove glView from window
        [openGLview removeFromSuperview];

        // Set new frame
        [openGLview setFrame:displayRect];

        // Attach glView to fullscreen window
        [fullScreenWindow_ setContentView:openGLview];

        // Show the fullscreen window
        [fullScreenWindow_ makeKeyAndOrderFront:self];
		[fullScreenWindow_ makeMainWindow];
		//[fullScreenWindow_ setNextResponder:superViewGLView_];
                
        CrossApp::CCApplication::sharedApplication()->applicationDidToggleFullScreen();
    }
    else
    {

        // Remove glView from fullscreen window
        [openGLview removeFromSuperview];

        // Release fullscreen window
        [fullScreenWindow_ close];
        fullScreenWindow_ = nil;
        
        // Attach glView to superview
        [superViewGLView_ addSubview:openGLview];

        // Set new frame
        [openGLview setFrame:originalWinRect_];

        // Show the window
        [windowGLView_ makeKeyAndOrderFront:self];
		[windowGLView_ makeMainWindow];
        
        CrossApp::CCApplication::sharedApplication()->applicationDidExitFullScreen();
    }
	
	// issue #1189
	[windowGLView_ makeFirstResponder:openGLview];

    isFullScreen_ = fullscreen;

    [openGLview setNeedsDisplay:YES];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"FULL_SCREEN_CHANGED" object:nil];
#else
#error Full screen is not supported for Mac OS 10.5 or older yet
#error If you don't want FullScreen support, you can safely remove these 2 lines
#endif
}

#if CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
#define DISPATCH_EVENT(__event__, __selector__) [eventDelegate_ queueEvent:__event__ selector:__selector__];
#else
#define DISPATCH_EVENT(__event__, __selector__)												\
	id obj = eventDelegate_;																\
	[obj performSelector:__selector__														\
			onThread:[(CrossApp::CAApplication*)[CAApplication sharedApplication] runningThread]			\
		  withObject:__event__																\
	   waitUntilDone:NO];
#endif

#pragma mark EAGLView - Mouse events

- (void)mouseDown:(NSEvent *)theEvent
{
	NSPoint event_location = [theEvent locationInWindow];
	NSPoint local_point = [self convertPoint:event_location fromView:nil];
	
	float x = local_point.x;
	float y = [self getHeight] - local_point.y;
	
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
	ids[0] = [theEvent eventNumber];
    xs[0] = x;
	ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::leftMouseEvent);

	CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesBegin(1, ids, xs, ys, event);
    event->release();
} 

- (void)mouseDragged:(NSEvent *)theEvent
{
	NSPoint event_location = [theEvent locationInWindow];
	NSPoint local_point = [self convertPoint:event_location fromView:nil];
	
	float x = local_point.x;
	float y = [self getHeight] - local_point.y;

    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
	ids[0] = [theEvent eventNumber];
	xs[0] = x;
	ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::leftMouseEvent);

	CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesMove(1, ids, xs, ys, event);
    event->release();
}

- (void)mouseUp:(NSEvent *)theEvent
{
	NSPoint event_location = [theEvent locationInWindow];
	NSPoint local_point = [self convertPoint:event_location fromView:nil];
	
	float x = local_point.x;
	float y = [self getHeight] - local_point.y;

    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
	ids[0] = [theEvent eventNumber];
	xs[0] = x;
	ys[0] = y;

    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::leftMouseEvent);
	CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesEnd(1, ids, xs, ys, event);
    event->release();
}

- (void)rightMouseDown:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);

	// pass the event along to the next responder (like your NSWindow subclass)
	[super rightMouseDown:theEvent];
    
    
    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = [self getHeight] - local_point.y;
    
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
    ids[0] = [theEvent eventNumber];
    xs[0] = x;
    ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::rightMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesBegin(1, ids, xs, ys, event);
    event->release();
}

- (void)rightMouseDragged:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super rightMouseDragged:theEvent];
    
    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = [self getHeight] - local_point.y;
    
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
    ids[0] = [theEvent eventNumber];
    xs[0] = x;
    ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::rightMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesMove(1, ids, xs, ys, event);
    event->release();
}

- (void)rightMouseUp:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super rightMouseUp:theEvent];
    
    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = [self getHeight] - local_point.y;
    
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
    ids[0] = [theEvent eventNumber];
    xs[0] = x;
    ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::rightMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesEnd(1, ids, xs, ys, event);
    event->release();
}

- (void)otherMouseDown:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
    [super otherMouseDown:theEvent];
    
    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = [self getHeight] - local_point.y;
    
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
    ids[0] = [theEvent eventNumber];
    xs[0] = x;
    ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::middleMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleOtherMouseDown(1, ids, xs, ys, event);
    event->release();
}

- (void)otherMouseDragged:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
    [super otherMouseDragged:theEvent];
    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = [self getHeight] - local_point.y;
    
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
    ids[0] = [theEvent eventNumber];
    xs[0] = x;
    ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::middleMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleOtherMouseDragged(1, ids, xs, ys, event);
    event->release();
}

- (void)otherMouseUp:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
    [super otherMouseUp:theEvent];
    
    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = [self getHeight] - local_point.y;
    
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
    ids[0] = [theEvent eventNumber];
    xs[0] = x;
    ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::middleMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleOtherMouseUp(1, ids, xs, ys, event);
    event->release();
}

- (void)mouseEntered:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super mouseEntered:theEvent];
    
    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = [self getHeight] - local_point.y;
    
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
    ids[0] = [theEvent eventNumber];
    xs[0] = x;
    ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::rightMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleMouseEntered(1, ids, xs, ys, event);
    event->release();
}

- (void)mouseExited:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super mouseExited:theEvent];
    
    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = [self getHeight] - local_point.y;
    
    intptr_t ids[1] = {0};
    float xs[1] = {0.0f};
    float ys[1] = {0.0f};
    
    ids[0] = [theEvent eventNumber];
    xs[0] = x;
    ys[0] = y;
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::rightMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleMouseExited(1, ids, xs, ys, event);
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    DISPATCH_EVENT(theEvent, _cmd);
    [super mouseMoved:theEvent];

    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = ([self getHeight] - local_point.y);
    
    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::movedMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleMouseMoved(x, y, event);
    event->release();
    
}

-(void) scrollWheel:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super scrollWheel:theEvent];

    NSPoint event_location = [theEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    float x = local_point.x;
    float y = ([self getHeight] - local_point.y);

    CrossApp::CAEvent* event = new CrossApp::CAEvent();
    event->setEventType(CrossApp::EventType::middleMouseEvent);
    CrossApp::CAApplication::getApplication()->getOpenGLView()->handleScrollWheel(x, y, theEvent.scrollingDeltaX, theEvent.scrollingDeltaY, event);
    event->release();
}

- (void)flagsChanged:(NSEvent *)theEvent
{
	DISPATCH_EVENT(theEvent, _cmd);
}

#pragma mark EAGLView - Touch events
- (void)touchesBeganWithEvent:(NSEvent *)theEvent
{
	DISPATCH_EVENT(theEvent, _cmd);
}

- (void)touchesMovedWithEvent:(NSEvent *)theEvent
{
	DISPATCH_EVENT(theEvent, _cmd);
}

- (void)touchesEndedWithEvent:(NSEvent *)theEvent
{
	DISPATCH_EVENT(theEvent, _cmd);
}

- (void)touchesCancelledWithEvent:(NSEvent *)theEvent
{
	DISPATCH_EVENT(theEvent, _cmd);
}

@end
