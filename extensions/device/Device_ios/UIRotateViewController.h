//
//  UIRotateViewController.h
//  Smobiler
//
//  Created by 惜风幻 on 15/8/10.
//
//

#import <UIKit/UIKit.h>

@interface UIRotateViewController : UIViewController
{
    UILabel* _label;
    NSString* _text;
    NSString* _bgRes;
}

- (void)setShowText:(NSString*)text andBgResPath:(NSString*)bgResPath;

@end
