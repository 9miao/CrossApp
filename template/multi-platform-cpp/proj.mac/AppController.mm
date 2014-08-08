
 
#import "AppController.h"
#import "AppDelegate.h"

@implementation AppController

	static AppDelegate s_sharedApplication;

	@synthesize window, glView;

	-(void) applicationDidFinishLaunching:(NSNotification *)aNotification
	{
		// create the window
		// note that using NSResizableWindowMask causes the window to be a little
		// smaller and therefore ipad graphics are not loaded
        NSRect rect = NSMakeRect(100, 100, 640, 960);
		window = [[NSWindow alloc] initWithContentRect:rect
			styleMask:( NSClosableWindowMask | NSTitledWindowMask )
			backing:NSBackingStoreBuffered
			defer:YES];
        
        NSOpenGLPixelFormatAttribute attributes[] = {
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAStencilSize, 8,
            0
        };
        
        NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
		
		// allocate our GL view
		// (isn't there already a shared EAGLView?)
		glView = [[EAGLView alloc] initWithFrame:rect pixelFormat:pixelFormat];

		// set window parameters
		[window becomeFirstResponder];
		[window setContentView:glView];
		[window setTitle:@"HelloCpp"];
		[window makeKeyAndOrderFront:self];
		[window setAcceptsMouseMovedEvents:NO];

		CrossApp::CCApplication::sharedApplication()->run();
	}

	-(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
	{
		return YES;
	}

	-(void) dealloc
	{
		CrossApp::CAApplication::getApplication()->end();
		[super dealloc];
	}

#pragma mark -
#pragma mark IB Actions

	-(IBAction) toggleFullScreen:(id)sender
	{
		EAGLView* pView = [EAGLView sharedEGLView];
		[pView setFullScreen:!pView.isFullScreen];
	}

	-(IBAction) exitFullScreen:(id)sender
	{
		[[EAGLView sharedEGLView] setFullScreen:NO];
	}

@end
