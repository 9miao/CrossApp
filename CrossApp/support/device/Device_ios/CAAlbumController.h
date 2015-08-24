//
//  CAAlbumController.h
//  LocationTest
//
//  Created by lh on 14-5-29.
//
//

#ifndef __LocationTest__CAAlbumController__
#define __LocationTest__CAAlbumController__

#import <UIKit/UIKit.h>
#include <iostream>
#include "CrossApp.h"

@protocol CAAlbumDelegate<NSObject>
@optional
-(void)setSeletedImage:(UIImage*)_image;
@end

@interface CAAlbumController : UIViewController<UINavigationControllerDelegate,UIImagePickerControllerDelegate>

@property (assign,nonatomic)void *sender;
-(void)openAlbumView:(BOOL)allowEdit;
-(void)writeImageToPhoto:(const std::string&)sender;
@end
#endif /* defined(__LocationTest__CAAlbumController__) */
