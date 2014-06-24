//
//  CACameraController.h
//  iBook
//
//  Created by lh on 14-5-5.
//
//

#import <UIKit/UIKit.h>
#include "CrossApp.h"
using namespace CrossApp;
@protocol CACameraDelegate<NSObject>
@optional
-(void)setSeletedImage:(UIImage*)_image;
@end

@interface CACameraController : UIViewController<UINavigationControllerDelegate,UIImagePickerControllerDelegate>
@property (assign,nonatomic)void *sender;
-(void)openCameraView;
@end
