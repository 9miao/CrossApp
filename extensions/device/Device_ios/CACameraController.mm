//
//  CACameraController.m
//  iBook
//
//  Created by lh on 14-5-5.
//
//

#import "CACameraController.h"
#import "EAGLView.h"
#include "../CADevice.h"
@interface CACameraController ()
{
    UIImage *_image;
}
@end

@implementation CACameraController

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
    
    [[EAGLView sharedEGLView] addSubview:self.view];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}
-(void)writeImageToPhoto:(std::string)sender
{
    
    UIImage *newImage = [[UIImage alloc] initWithContentsOfFile:[NSString stringWithFormat:@"%s",sender.c_str()]];
    if (newImage == nil)
    {
        NSLog(@"Save image have some error");
    }
    UIImageWriteToSavedPhotosAlbum(newImage, self, nil, nil);
    [[NSFileManager defaultManager] removeItemAtPath:[NSString stringWithFormat:@"%s",sender.c_str()] error:nil];
}
-(void)openCameraView:(BOOL)allowEdit
{

    BOOL isCamera = [UIImagePickerController isCameraDeviceAvailable:UIImagePickerControllerCameraDeviceRear];

    CC_RETURN_IF(!isCamera);
    
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
    imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
    imagePicker.delegate= self;
    imagePicker.allowsEditing = allowEdit;
   // imagePicker.allowsEditing=NO;
   // imagePicker.cameraOverlayView = NO;
    [self presentViewController:imagePicker animated:YES completion:^{ CAApplication::getApplication()->getTouchDispatcher()->setDispatchEvents(false);
    }];
    
}
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    
    NSString *imageType;
    if (picker.allowsEditing)
    {
        imageType = [NSString stringWithFormat:@"UIImagePickerControllerEditedImage"];
    }
    else
    {
        imageType = [NSString stringWithFormat:@"UIImagePickerControllerOriginalImage"];
    }
    
    UIImage *image = [info objectForKey:imageType];
    UIImage *newfixImage = [self fixOrientation:image];
    UIImage *fiximage = [self scaleFromImage:newfixImage toSize:CGSizeMake(640, [newfixImage size].height/([newfixImage size].width/640))];
    CAMediaDelegate *cam = (CAMediaDelegate *)self.sender;
    CC_RETURN_IF(!cam);

    NSData *data = UIImageJPEGRepresentation(fiximage,0.5);
    void* _data = malloc([data length]);
    [data getBytes:_data];
    
    CAImage *__image = new CAImage();
    __image->initWithImageData((unsigned char*)_data, data.length);
    if (cam)
    {
        cam->getSelectedImage(__image);
    }
    __image->release();
    free(_data);

    [picker dismissViewControllerAnimated:YES completion:^
        {
            [self.view removeFromSuperview];
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEvents(true);
        }
    ];
}
- (UIImage *) scaleFromImage: (UIImage *) image toSize: (CGSize) size
{
    UIGraphicsBeginImageContext(size);
    [image drawInRect:CGRectMake(0, 0, size.width, size.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:^
        {
            [self.view removeFromSuperview];
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEvents(true);
        }
    ];
    
}
- (UIImage *)fixOrientation:(UIImage *)srcImg {
    if (srcImg.imageOrientation == UIImageOrientationUp) return srcImg;
    CGAffineTransform transform = CGAffineTransformIdentity;
    switch (srcImg.imageOrientation) {
        case UIImageOrientationDown:
        case UIImageOrientationDownMirrored:
            transform = CGAffineTransformTranslate(transform, srcImg.size.width, srcImg.size.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
            
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
            transform = CGAffineTransformTranslate(transform, srcImg.size.width, 0);
            transform = CGAffineTransformRotate(transform, M_PI_2);
            break;
            
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformTranslate(transform, 0, srcImg.size.height);
            transform = CGAffineTransformRotate(transform, -M_PI_2);
            break;
        case UIImageOrientationUp:
        case UIImageOrientationUpMirrored:
            break;
    }
    
    switch (srcImg.imageOrientation) {
        case UIImageOrientationUpMirrored:
        case UIImageOrientationDownMirrored:
            transform = CGAffineTransformTranslate(transform, srcImg.size.width, 0);
            transform = CGAffineTransformScale(transform, -1, 1);
            break;
            
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformTranslate(transform, srcImg.size.height, 0);
            transform = CGAffineTransformScale(transform, -1, 1);
            break;
        case UIImageOrientationUp:
        case UIImageOrientationDown:
        case UIImageOrientationLeft:
        case UIImageOrientationRight:
            break;
    }
    
    CGContextRef ctx = CGBitmapContextCreate(NULL, srcImg.size.width, srcImg.size.height,
                                             CGImageGetBitsPerComponent(srcImg.CGImage), 0,
                                             CGImageGetColorSpace(srcImg.CGImage),
                                             CGImageGetBitmapInfo(srcImg.CGImage));
    CGContextConcatCTM(ctx, transform);
    switch (srcImg.imageOrientation) {
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            CGContextDrawImage(ctx, CGRectMake(0,0,srcImg.size.height,srcImg.size.width), srcImg.CGImage);
            break;
            
        default:
            CGContextDrawImage(ctx, CGRectMake(0,0,srcImg.size.width,srcImg.size.height), srcImg.CGImage);
            break;
    }
    
    CGImageRef cgimg = CGBitmapContextCreateImage(ctx);
    UIImage *img = [UIImage imageWithCGImage:cgimg];
    CGContextRelease(ctx);
    CGImageRelease(cgimg);
    return img;
}
@end
