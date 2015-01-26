

#import "CCWindow.h"
#import "EAGLView.h"

@implementation CCWindow

- (id) initWithFrame:(NSRect)frame fullscreen:(BOOL)fullscreen
{
	int styleMask = fullscreen ? NSBackingStoreBuffered : ( NSTitledWindowMask | NSClosableWindowMask );
	self = [self initWithContentRect:frame
						   styleMask:styleMask
							 backing:NSBackingStoreBuffered
							   defer:YES];

	if (self != nil)
	{
		if(fullscreen)
		{
			[self setLevel:NSMainMenuWindowLevel+1];
			[self setHidesOnDeactivate:YES];
			[self setHasShadow:NO];
		}

		[self setAcceptsMouseMovedEvents:NO];
		[self setOpaque:YES];
	}
	return self;
}

- (BOOL) canBecomeKeyWindow
{
	return YES;
}

- (BOOL) canBecomeMainWindow
{
	return YES;
}

- (void) keyDown:(NSEvent *)event
{
	// exit fullscreen if user pressed esc
	if([event keyCode] == 53)
	{
		EAGLView* eaglView = [EAGLView sharedEGLView];
		
		// cancel full screen
		if( [eaglView isFullScreen] )
			[eaglView setFullScreen:NO];

		// let another responder take it
		else
			[super keyDown:event];
	}
}

@end

