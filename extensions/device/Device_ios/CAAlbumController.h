//
//  CAAlbumController.h
//  LocationTest
//
//  Created by lh on 14-5-29.
//
//

#ifndef __LocationTest__CAAlbumController__
#define __LocationTest__CAAlbumController__

#include "CrossApp.h"
using namespace CrossApp;
@protocol CAAlbumDelegate<NSObject>
@optional
-(void)setSeletedImage:(UIImage*)_image;
@end

@interface CAAlbumController : UIViewController<UINavigationControllerDelegate,UIImagePickerControllerDelegate>

@property (assign,nonatomic)void *sender;
-(void)openAlbumView;
@end
#endif /* defined(__LocationTest__CAAlbumController__) */
