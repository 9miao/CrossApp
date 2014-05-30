//
//  CAAlbumController.h
//  LocationTest
//
//  Created by lh on 14-5-29.
//
//

#ifndef __LocationTest__CAAlbumController__
#define __LocationTest__CAAlbumController__

#include "cocos2d.h"
using namespace cocos2d;
@protocol CAAlbumDelegate<NSObject>
@optional
-(void)setSeletedImage:(UIImage*)_image;
@end

@interface CAAlbumController : UIViewController<UINavigationControllerDelegate,UIImagePickerControllerDelegate>

@property (assign,nonatomic)void *sender;
-(void)openCameraView;
@end
#endif /* defined(__LocationTest__CAAlbumController__) */
