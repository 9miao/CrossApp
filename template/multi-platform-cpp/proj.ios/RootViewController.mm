#import "RootViewController.h"
#import "EAGLView.h"
#import "CrossApp.h"

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self.view setBackgroundColor:[UIColor whiteColor]];
    
    CGRect rect = [[UIScreen mainScreen] bounds];
    //CGFloat scale = [[UIScreen mainScreen] scale];
    
//    if ( [[UIDevice currentDevice].systemVersion floatValue] >= 7.0)
//    {
//        rect.size.height -= 20;
//        rect.origin.y = 20;
//    }
//    else
//    {
//        rect.size = [[UIScreen mainScreen]applicationFrame].size;
//    }
    
    EAGLView *__glView = [EAGLView viewWithFrame: rect
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];
    [__glView setMultipleTouchEnabled:YES];
    [self.view addSubview:__glView];
}


// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsPortrait( interfaceOrientation );
    //return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
    
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskPortrait;
    //return UIInterfaceOrientationMaskLandscapeRight;
#endif
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return UIInterfaceOrientationPortrait;
    //return UIInterfaceOrientationLandscapeRight;
}

- (BOOL) shouldAutorotate
{
    return NO;
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return NO;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc
{
    [super dealloc];
}


@end
