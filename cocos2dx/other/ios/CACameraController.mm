//
//  CACameraController.m
//  iBook
//
//  Created by lh on 14-5-5.
//
//

#import "CACameraController.h"
#import "EAGLView.h"
#include "CACamera.h"
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

    UIImageView *imageView = [[UIImageView alloc] init];
    
    imageView.frame = CGRectMake(0, 0, 320, 400);
    
    
    imageView.backgroundColor = [UIColor clearColor];
    
    imageView.tag = 101;
    
    [self.view addSubview:imageView];
    
    [[EAGLView sharedEGLView] addSubview:self.view];
    
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)openCameraView
{
    BOOL isCamera = [UIImagePickerController
                isCameraDeviceAvailable:UIImagePickerControllerCameraDeviceRear];
    if(!isCamera) {
        NSLog(@"没有摄像头");
        
        return;
    }
    UIImagePickerController
    *imagePicker = [[UIImagePickerController alloc] init];
    
    
    imagePicker.sourceType
    = UIImagePickerControllerSourceTypeCamera;
    
    
    imagePicker.delegate= self;
    
    imagePicker.allowsEditing = YES;
    
    
    [self
     presentViewController:imagePicker animated:YES
     completion:^{
         
         
     }];
}
-(void)openAlbumView
{
    UIImagePickerController
    *imagePicker = [[UIImagePickerController alloc] init];
    
    
    imagePicker.sourceType
    = UIImagePickerControllerSourceTypePhotoLibrary;
    
    
    imagePicker.delegate
    = self;
    
    
    [self
     presentViewController:imagePicker animated:YES
     completion:^{
         
         
     }];
}

- (void)imagePickerController:(UIImagePickerController *)picker
didFinishPickingMediaWithInfo:(NSDictionary *)info{
    
    

    
    
    //UIImagePickerControllerOriginalImage 原始图片
    
    
    //UIImagePickerControllerEditedImage 编辑后图片
    
    
    UIImage
    *image = [info
              objectForKey:UIImagePickerControllerOriginalImage];
    
    
    
    CACamera *cam = (CACamera *)self.sender;
    
    if(cam == NULL)
    {
        return ;
    }
    NSData *data = UIImagePNGRepresentation(image);
    void* _data =malloc([data length]);
    [data getBytes:_data];
    
    NSLog(@"%f  %f ",image.size.width, image.size.height);
    CCImage *caimage = new CCImage();
    caimage->initWithImageData(_data, [data length], CCImage::kFmtPng, image.size.width, image.size.height);
    NSLog(@"%hu  %hu ",caimage->getWidth(), caimage->getHeight());
    cam->getCameraDelegate()->getSelectedImage(caimage);
   // imageView.image = image;
    
    
    
    [picker
     dismissViewControllerAnimated:YES completion:NULL];

    
    
    
    
}
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    NSLog(@"Did Cancel");
}
-(CCImage *)getSelectedImage
{
    if (_image == nil) {
        return NULL;
    }
    NSData *data = UIImagePNGRepresentation(_image);
    void* _data =malloc([data length]);
    [data getBytes:_data];
    CCImage *image = new CCImage();
    image->initWithImageData(data, [data length], CCImage::kFmtPng, _image.size.width, _image.size.height);
    
    return image;
}
@end
