
#import <Availability.h>

#import "EAGLView.h"
#import "CCEGLView.h"
#import <OpenGL/gl.h>
#import "basics/CAApplication.h"
#import "ccConfig.h"
#import "CCSet.h"
#import "CATouch.h"
#import "CAIMEDispatcher.h"
#import "CCWindow.h"
#import "CCEventDispatcher.h"
#import "CCEGLView.h"


//USING_NS_CC;
static EAGLView *view;

@implementation EAGLView

@synthesize eventDelegate = eventDelegate_, isFullScreen = isFullScreen_, frameZoomFactor=frameZoomFactor_;
@synthesize textfield = _textfield;

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
	
	NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	
	if (!pixelFormat)
		NSLog(@"No OpenGL pixel format");
	
	if( (self = [super initWithFrame:frameRect pixelFormat:[pixelFormat autorelease]]) ) {
		
		if( context )
			[self setOpenGLContext:context];

		// event delegate
		eventDelegate_ = [CCEventDispatcher sharedDispatcher];
	}
    
    CrossApp::CCEGLView::sharedOpenGLView()->setFrameSize(frameRect.size.width, frameRect.size.height);
    
    frameZoomFactor_ = 1.0f;
	
	view = self;
	return self;
}

- (id) initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format{
    // event delegate
    eventDelegate_ = [CCEventDispatcher sharedDispatcher];
    
    _textfield = [[CAMACTextField alloc] init];
    [_textfield setCadelegate:self];
    [_textfield setFrame:CGRectMake(-2000, -2000, 100, 50)];
    _textfield.hidden = YES;
    [self addSubview:_textfield];
    
    CrossApp::CCEGLView::sharedOpenGLView()->setFrameSize(frameRect.size.width, frameRect.size.height);
    
    frameZoomFactor_ = 1.0f;
	
	view = self;
    
    [super initWithFrame:frameRect pixelFormat:format];

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

- (void) setFrameZoomFactor:(float)frameZoomFactor
{
    frameZoomFactor_ = frameZoomFactor;
    
    NSRect winRect = [[self window] frame];
    NSRect viewRect = [self frame];
    
    // compute the margin width and margin height
    float diffX = winRect.size.width - viewRect.size.width;
    float diffY = winRect.size.height - viewRect.size.height;
    
    // new window width and height
    float newWindowWidth = (int)(viewRect.size.width * frameZoomFactor + diffX);
    float newWindowHeight = (int)(viewRect.size.height * frameZoomFactor + diffY);
    
    // display window in the center of the screen
    NSRect screenRect = [[NSScreen mainScreen] frame];
    float originX = (screenRect.size.width - newWindowWidth) / 2;
    float originY = (screenRect.size.height - newWindowHeight) / 2;
    
    [[self window] setFrame:NSMakeRect(originX, originY, newWindowWidth, newWindowHeight) display:true];
}

- (void) reshape
{
	// We draw on a secondary thread through the display link
	// When resizing the view, -reshape is called automatically on the main thread
	// Add a mutex around to avoid the threads accessing the context simultaneously when resizing

	[self lockOpenGLContext];
	//NO_FINISH
	NSRect rect = [self bounds];
	
	CrossApp::CAApplication *director = CrossApp::CAApplication::getApplication();
	CGSize size = NSSizeToCGSize(rect.size);
	CrossApp::CCSize ccsize = CrossApp::CCSizeMake(size.width, size.height);
	director->reshapeProjection(ccsize);
	
	// avoid flicker
//	director->drawScene();
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
	CCLOGINFO(@"CrossApp: deallocing EAGLView %@", self);
	[super dealloc];
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

    } else {

        // Remove glView from fullscreen window
        [openGLview removeFromSuperview];

        // Release fullscreen window
        [fullScreenWindow_ release];
        fullScreenWindow_ = nil;

        // Attach glView to superview
        [superViewGLView_ addSubview:openGLview];

        // Set new frame
        [openGLview setFrame:originalWinRect_];

        // Show the window
        [windowGLView_ makeKeyAndOrderFront:self];
		[windowGLView_ makeMainWindow];
    }
	
	// issue #1189
	[windowGLView_ makeFirstResponder:openGLview];

    isFullScreen_ = fullscreen;

    //[openGLview retain]; // Retain +1

	// is this necessary?
    // re-configure glView
	//CrossApp::CAApplication *director = CrossApp::CAApplication::getApplication();
	//director->setOpenGLView(openGLview); //[self setView:openGLview];

    //[openGLview release]; // Retain -1

    [openGLview setNeedsDisplay:YES];
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
	xs[0] = x / frameZoomFactor_;
	ys[0] = y / frameZoomFactor_;

	CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesBegin(1, ids, xs, ys);
}

- (void)mouseMoved:(NSEvent *)theEvent
{
	DISPATCH_EVENT(theEvent, _cmd);
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
	xs[0] = x / frameZoomFactor_;
	ys[0] = y / frameZoomFactor_;

	CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesMove(1, ids, xs, ys);
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
	xs[0] = x / frameZoomFactor_;
	ys[0] = y / frameZoomFactor_;

	CrossApp::CAApplication::getApplication()->getOpenGLView()->handleTouchesEnd(1, ids, xs, ys);
}

- (void)rightMouseDown:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);

	// pass the event along to the next responder (like your NSWindow subclass)
	[super rightMouseDown:theEvent];
}

- (void)rightMouseDragged:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super rightMouseDragged:theEvent];
}

- (void)rightMouseUp:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super rightMouseUp:theEvent];
}

- (void)otherMouseDown:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super otherMouseDown:theEvent];
}

- (void)otherMouseDragged:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super otherMouseDragged:theEvent];
}

- (void)otherMouseUp:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super otherMouseUp:theEvent];
}

- (void)mouseEntered:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super mouseEntered:theEvent];
}

- (void)mouseExited:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super mouseExited:theEvent];
}

-(void) scrollWheel:(NSEvent *)theEvent {
	DISPATCH_EVENT(theEvent, _cmd);
	[super scrollWheel:theEvent];
}

#pragma mark EAGLView - Key events

-(BOOL) becomeFirstResponder
{
    return  [_textfield becomeFirstResponder];

	return YES;
}

-(BOOL) acceptsFirstResponder
{
    return  [_textfield acceptsFirstResponder];

	return YES;
}

-(BOOL) resignFirstResponder
{
    return  [_textfield resignFirstResponder];

	return YES;
}

- (BOOL)hasText
{
    return NO;
}

- (void)insertText:(NSString *)text
{
    if (nil != markedText_) {
        [markedText_ release];
        markedText_ = nil;
    }
    const char * pszText = [text cStringUsingEncoding:NSUTF8StringEncoding];
    NSLog(@"insertext:%@； length:%lu", text, strlen(pszText));

    CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchInsertText(pszText, strlen(pszText));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    [_textfield setStringValue:@""];

#endif
}

- (void)deleteBackward
{
    if (nil != markedText_) {
        [markedText_ release];
        markedText_ = nil;
    }
    CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
}

- (void)keyDown:(NSEvent *)theEvent
{
	DISPATCH_EVENT(theEvent, _cmd);
	// pass the event along to the next responder (like your NSWindow subclass)
	[super keyDown:theEvent];
}

- (void)keyUp:(NSEvent *)theEvent
{
	DISPATCH_EVENT(theEvent, _cmd);
    //[_textfield insertText:[theEvent characters]];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    //NSLog(@"%hu", [theEvent keyCode]);
    if(51 == [theEvent keyCode])//delete
    {
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
    }else if(123 == [theEvent keyCode])//leftsaf
    {
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchCursorMoveBackward();
    }else if(124 == [theEvent keyCode])//right
    {
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchCursorMoveForward();
    }else if(125 == [theEvent keyCode])//down
    {
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchCursorMoveDown();

    }else if(126 == [theEvent keyCode])//up
    {
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchCursorMoveUp();
    }
#endif


	// pass the event along to the next responder (like your NSWindow subclass)
	[super keyUp:theEvent];
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

@synthesize inputDelegate;


- (BOOL)textShouldBeginEditing:(NSText *)textObject
{
    [_textfield textShouldBeginEditing:textObject];
    NSLog(@"textShouldBeginEditing:%@",textObject.string);
    return true;
}

- (BOOL)textShouldEndEditing:(NSText *)textObject
{
    [_textfield textShouldBeginEditing:textObject];
    
    NSLog(@"textShouldEndEditing:%@",textObject.string);
    return true;
}


- (void)textDidBeginEditing:(NSNotification *)notification
{
    [_textfield textDidBeginEditing:notification];
    NSLog(@"textDidBeginEditing:");
    
}

- (void)textDidEndEditing:(NSNotification *)notification
{
  //  NSLog(@"textDidEndEditing:");
    [_textfield textDidEndEditing:notification];

}

- (void)textDidChange:(NSNotification *)notification
{
    NSLog(@"textDidChange:%@",notification);
    [_textfield textDidChange:notification];

}

- (void)setMarkedText:(id)aString selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange
{
    if (aString == markedText_) {
        return;
    }
    if (nil != markedText_) {
        [markedText_ release];
    }
    
    const char * pszText = [aString cStringUsingEncoding:NSUTF8StringEncoding];
    NSRange range;
    range.length = 0;
    range.location = [aString length];
    
    //    UITextPosition *beginning = self.beginningOfDocument;
    //    UITextPosition *start = [self positionFromPosition:beginning offset:range.location];
    //    UITextPosition *end = [self positionFromPosition:start offset:range.length];
    //    UITextRange *textRange = [self textRangeFromPosition:start toPosition:end];
    //
    //    [self textInRange:textRange];
    
    CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchWillInsertText(pszText, (int)strlen(pszText));
    markedText_ = aString;
    [markedText_ retain];

}
- (void)setMarkedText:(NSString *)markedText selectedRange:(NSRange)selectedRange;
{
    
    //CCLOG("setMarkedText");
    if (markedText == markedText_) {
        return;
    }
    if (nil != markedText_) {
        [markedText_ release];
    }
    
    const char * pszText = [markedText cStringUsingEncoding:NSUTF8StringEncoding];
    NSRange range;
    range.length = 0;
    range.location = [markedText length];
    
    //    UITextPosition *beginning = self.beginningOfDocument;
    //    UITextPosition *start = [self positionFromPosition:beginning offset:range.location];
    //    UITextPosition *end = [self positionFromPosition:start offset:range.length];
    //    UITextRange *textRange = [self textRangeFromPosition:start toPosition:end];
    //
    //    [self textInRange:textRange];
    
    CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchWillInsertText(pszText, (int)strlen(pszText));
    markedText_ = markedText;
    [markedText_ retain];
}

- (void)unmarkText;
{
    //CCLOG("unmarkText");
    if (nil == markedText_)
    {
        return;
    }
    const char * pszText = [markedText_ cStringUsingEncoding:NSUTF8StringEncoding];
    CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchInsertText(pszText, (int)strlen(pszText));
    [markedText_ release];
    markedText_ = nil;
}
@end
