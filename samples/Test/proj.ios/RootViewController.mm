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

    __glView = [EAGLView viewWithFrame: rect
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];
    [__glView setMultipleTouchEnabled:YES];
    [self.view addSubview:__glView];
    

//    NSString *path=@"System/Library/Fonts"; // 要列出来的目录
//    
//    NSFileManager *myFileManager=[NSFileManager defaultManager];
//    
//    NSDirectoryEnumerator *myDirectoryEnumerator;
//    
//    myDirectoryEnumerator=[myFileManager enumeratorAtPath:path];
//    
//    //列举目录内容，可以遍历子目录
//    
//    NSLog(@"用enumeratorAtPath:显示目录%@的内容：",path);
//    
//    NSString* ss = nil;
//    while((ss=[myDirectoryEnumerator nextObject])!=nil)
//    {
//        NSString* s = @"System/Library/Fonts/";
//        s = [s stringByAppendingString:ss];
//        NSFileHandle *fileHandle=[NSFileHandle fileHandleForReadingAtPath:s];
//        NSUInteger length= [fileHandle availableData].length;
//        NSLog(@"%@ --- %lu",ss, (unsigned long)length);
//    }
}

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return ((interfaceOrientation) == UIInterfaceOrientationPortrait || (interfaceOrientation) == UIInterfaceOrientationPortraitUpsideDown  ||  (interfaceOrientation) == UIInterfaceOrientationLandscapeLeft || (interfaceOrientation) == UIInterfaceOrientationLandscapeRight);
    //return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
    
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAll;
    //return UIInterfaceOrientationMaskLandscapeRight;
#endif
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return UIInterfaceOrientationUnknown;
    //return UIInterfaceOrientationPortrait;
}

- (BOOL) shouldAutorotate
{
    return YES;
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
