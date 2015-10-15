//
//  UIRotateViewController.m
//  Smobiler
//
//  Created by 惜风幻 on 15/8/19.
//
//

#import "UIRotateViewController.h"
#import <QuartzCore/CALayer.h>
#define TOAST_MAX_WIDTH 200

@interface UIRotateViewController ()

@end

@implementation UIRotateViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self initLabel];
    [self.view setBackgroundColor:[UIColor clearColor]];
    
    //Register device orientation notifications.
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(orientationChanged:)
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];
}

- (void)dealloc
{
    [_label removeFromSuperview];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIDeviceOrientationDidChangeNotification object:nil];
}

- (void)orientationChanged:(NSNotification *)notification
{
    [self fitScreenSize];
}

- (void)initLabel
{
    _label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 200, 30)];
    [_label setBackgroundColor:[UIColor blackColor]];
    [_label setOpaque:NO];
    [_label setTextColor:[UIColor whiteColor]];
    [_label setTextAlignment:UITextAlignmentCenter];
    [_label setShadowOffset:CGSizeMake(0, 2.0)];
    [_label setFont:[UIFont systemFontOfSize:18.0]];
    _label.layer.cornerRadius = 6;
    _label.layer.masksToBounds = YES;
}

- (void)setShowText:(NSString*)text andBgResPath:(NSString*)bgResPath
{
    _text = text.copy;
    _bgRes = bgResPath.copy;
    
    [self fitScreenSize];
}

- (void)fitScreenSize
{
    if (nil == _text)
        return;
    
    //Cacluate screen width and screen height.
    float screenWidth = [UIScreen mainScreen].bounds.size.width;
    float screenHeight = [UIScreen mainScreen].bounds.size.height;
    int max = MAX(screenWidth, screenHeight);
    int min = MIN(screenWidth, screenHeight);
    if (UIDeviceOrientationIsLandscape([UIDevice currentDevice].orientation))
    {
        screenHeight = min;
        screenWidth = max;
    }else if (UIDeviceOrientationIsPortrait([UIDevice currentDevice].orientation))
    {
        screenHeight = max;
        screenWidth = min;
    }else
    {
        return;
    }
    
    float width = screenWidth * 0.8;
    //Caculate text size.
    CGSize size = [_text sizeWithFont:[UIFont systemFontOfSize:18.0]];
    int lineNumber = size.width / width + 1;
    float viewWidth = 1 == lineNumber ? (size.width + 20) : width;
    float viewHeight = lineNumber * size.height + 18;
    [self.view setFrame:CGRectMake((screenWidth - viewWidth) / 2, (screenHeight / 4) * 3.3 - viewHeight / 2, viewWidth, viewHeight)];
    [_label setFrame:CGRectMake(0, 0, viewWidth, viewHeight)];
    [_label setText:_text];
    _label.numberOfLines = lineNumber;
    
    //Set toast's background image if needed.
    if (nil != _bgRes)
    {
        [_label setBackgroundColor:[UIColor colorWithPatternImage:[UIImage imageNamed:_bgRes]]];
    }
    [self.view addSubview:_label];
}
@end
