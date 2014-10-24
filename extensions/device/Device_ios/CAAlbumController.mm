//
//  CAAlbumController.cpp
//  LocationTest
//
//  Created by lh on 14-5-29.
//
//

#import "CAAlbumController.h"
#import "EAGLView.h"
#include "../CADevice.h"

@interface CAAlbumController ()
{
    UIImage *_image;
}

@end

@implementation CAAlbumController

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
    
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)openAlbumView
{
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
    
    
    imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    
    
    imagePicker.delegate = self;
    
    [self presentViewController:imagePicker animated:YES completion:^
        {
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEvents(false);
        }
    ];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];

    CAMediaDelegate *cam = (CAMediaDelegate *)self.sender;
    
    if(cam == NULL)
    {
        return ;
    }
    
    NSData *data = UIImageJPEGRepresentation(image,0.5);
    void* _data =malloc([data length]);
    [data getBytes:_data];
    
    CCImage *caimage = new CCImage();
   
    caimage->initWithImageData(_data, [data length], CCImage::kFmtPng, image.size.height, image.size.width);
    
    cam->getSelectedImage(caimage);
    
    [picker dismissViewControllerAnimated:YES completion:^
        {
            [self.view removeFromSuperview];
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEvents(true);
        }
    ];
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

@end