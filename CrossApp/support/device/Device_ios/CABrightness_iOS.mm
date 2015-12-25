//
//  CABrightness_iOS.m
//  JavaScriptTest
//
//  Created by lihui on 14-8-12.
//
//

#import "CABrightness_iOS.h"
#import <UIKit/UIKit.h>

@implementation CABrightness_iOS
-(float)getBrightnessPercent
{
    float brightness = [[UIScreen mainScreen] brightness];
    return brightness;
}
-(void)setBrightnessPercent:(float)sender
{
    [[UIScreen mainScreen] setBrightness:sender];
}
@end
