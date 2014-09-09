//
//  CACameraController.m
//  iBook
//
//  Created by lh on 14-5-5.
//
//

#import "CACameraController.h"
#import "EAGLView.h"
#include "CAMediaManager.h"
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
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

-(void)openCameraView
{
    BOOL isCamera = [UIImagePickerController isCameraDeviceAvailable:UIImagePickerControllerCameraDeviceRear];

    CC_RETURN_IF(!isCamera);
    
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
    imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
    imagePicker.delegate= self;
    imagePicker.allowsEditing = YES;
    
    [self presentViewController:imagePicker animated:YES completion:^{ CAApplication::getApplication()->getTouchDispatcher()->setDispatchEvents(false); }];
}
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{

    UIImage *image = [info objectForKey:@"UIImagePickerControllerEditedImage"];
    CAMediaManager *cam = (CAMediaManager *)self.sender;
    CC_RETURN_IF(!cam);

    NSData *data = UIImageJPEGRepresentation(image,0.5);
    void* _data =malloc([data length]);
    [data getBytes:_data];
        
    CCImage *caimage = new CCImage();
    caimage->initWithImageData(_data, [data length], CCImage::kFmtJpg, image.size.height, image.size.width);
    CC_RETURN_IF(!caimage);
    
    if (cam->getMediaDelegate())
    {
        cam->getMediaDelegate()->getSelectedImage(caimage);
    }
    
    [picker dismissViewControllerAnimated:YES completion:NULL];
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEvents(true);
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:NULL];
}

@end
