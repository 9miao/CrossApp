
#include "ImagePickerControllerTest.h"

ImagePickerControllerTest::ImagePickerControllerTest()
{
    this->setTitle("ImagePickerControllerTest");
}

ImagePickerControllerTest::~ImagePickerControllerTest()
{
    
}

void ImagePickerControllerTest::viewDidLoad()
{
    
    CAButton* takePicture = CAButton::create(CAButton::Type::SquareRect);
    takePicture->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.07)));
    takePicture->setTitleForState(CAControl::State::Normal,"拍照");
    takePicture->addTarget([=]()
    {
       // 打开相机
       CAImagePickerController* imgPicker = CAImagePickerController::create(CrossApp::CAImagePickerController::SourceType::CameraDeviceRear);
       imgPicker->open([=](CAImage* img)
       {
           CAImageView* image2 = CAImageView::createWithImage(img);
           image2->setLayout(DLayout(DHorizontalLayout_W_C(448, 0.5),DVerticalLayout_H_C(700, 0.6)));
           this->getView()->addSubview(image2);
       });
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(takePicture);
    
    CAButton* photoAlbum = CAButton::create(CAButton::Type::SquareRect);
    photoAlbum->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.15)));
    photoAlbum->setTitleForState(CAControl::State::Normal,"相册");
    photoAlbum->addTarget([=]()
    {
      // 打开相册
      CAImagePickerController* imgPicker = CAImagePickerController::create(CrossApp::CAImagePickerController::SourceType::PhotoLibrary);
      imgPicker->open([=](CAImage* img)
      {
          CAImageView* image2 = CAImageView::createWithImage(img);
          image2->setLayout(DLayout(DHorizontalLayout_W_C(448, 0.5), DVerticalLayout_H_C(700, 0.6)));
          this->getView()->addSubview(image2);
      });
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(photoAlbum);
    
}

void ImagePickerControllerTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
