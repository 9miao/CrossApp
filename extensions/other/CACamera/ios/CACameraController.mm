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
    if(!isCamera)
    {
        
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
- (void)imagePickerController:(UIImagePickerController *)picker
didFinishPickingMediaWithInfo:(NSDictionary *)info{

    
    UIImage *image = [info objectForKey:@"UIImagePickerControllerEditedImage"];

    
    CAMediaManager *cam = (CAMediaManager *)self.sender;
        
    if(cam == NULL)
    {
        return ;
    }
    NSData *data = UIImageJPEGRepresentation(image,0.5);
    void* _data =malloc([data length]);
    [data getBytes:_data];
        
    CCImage *caimage = new CCImage();
    caimage->initWithImageData(_data, [data length], CCImage::kFmtJpg, image.size.height, image.size.width);
    if (caimage == NULL||cam==NULL) {
        return;
    }
    cam->getMediaDelegate()->getSelectedImage(caimage);
    [picker
     dismissViewControllerAnimated:YES completion:NULL];

    
    
    
    
}
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker
     dismissViewControllerAnimated:YES completion:NULL];
}

@end
