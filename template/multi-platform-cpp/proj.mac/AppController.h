

#pragma once

#import "CrossApp.h"
#import "EAGLView.h"

@interface AppController : NSObject <NSApplicationDelegate>
	{
		NSWindow *window;
		EAGLView *glView;
	}

	@property (nonatomic, assign) IBOutlet NSWindow* window;
	@property (nonatomic, assign) IBOutlet EAGLView* glView;

	-(IBAction) toggleFullScreen:(id)sender;
	-(IBAction) exitFullScreen:(id)sender;

@end
