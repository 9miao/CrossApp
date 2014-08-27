#include "ImageViewTest.h"

ImageViewTest::ImageViewTest()
{

}

ImageViewTest::~ImageViewTest()
{

}

void ImageViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	
	CALabel* originalImage = CALabel::createWithCenter(CADipRect(size.width*0.5, size.height*0.1, size.width*0.4, 50 * CROSSAPP_ADPTATION_RATIO));
	originalImage->setText("Original image");
	originalImage->setFontSize(_px(30));
	originalImage->setColor(ccc4(51, 204, 255, 255));
	originalImage->setTextAlignment(CATextAlignmentCenter);
	originalImage->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(originalImage);

	CAImageView* firstImage = CAImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png"));
	firstImage->setCenterOrigin(CADipPoint(size.width*0.2, size.height*0.15));
	this->getView()->addSubview(firstImage);
	CAImageView* secondImage = CAImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png"));
	secondImage->setCenterOrigin(CADipPoint(size.width*0.5, size.height*0.15));
	this->getView()->addSubview(secondImage);
	CAImageView* thirdImage = CAImageView::createWithImage(CAImage::create("source_material/slider_indicator.png"));
	thirdImage->setCenterOrigin(CADipPoint(size.width*0.8, size.height*0.15));
	this->getView()->addSubview(thirdImage);

	CALabel* imageView = CALabel::createWithCenter(CADipRect(size.width*0.25, size.height*0.4, size.width*0.4, 50*CROSSAPP_ADPTATION_RATIO));
	imageView->setText("ImageView");
	imageView->setFontSize(_px(30));
	imageView->setColor(ccc4(51, 204, 255, 255));
	imageView->setTextAlignment(CATextAlignmentCenter);
	imageView->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(imageView);

	CAImageView* firstImageView = CAImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png"));
	firstImageView->setCenter(CADipRect(size.width*0.25, size.height*0.5, size.width*0.4, size.height*0.1));
	this->getView()->addSubview(firstImageView);
	CAImageView* secondImageView = CAImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png"));
	secondImageView->setCenter(CADipRect(size.width*0.25, size.height*0.65, size.width*0.4, size.height*0.1));
	this->getView()->addSubview(secondImageView);
	CAImageView* thirdImageView = CAImageView::createWithImage(CAImage::create("source_material/slider_indicator.png"));
	thirdImageView->setCenter(CADipRect(size.width*0.25, size.height*0.8, size.width*0.4, size.height*0.1));
	this->getView()->addSubview(thirdImageView);

	CALabel* scale9Image = CALabel::createWithCenter(CADipRect(size.width*0.75, size.height*0.4, size.width*0.4, 50 * CROSSAPP_ADPTATION_RATIO));
	scale9Image->setText("Scale9Image");
	scale9Image->setFontSize(_px(30));
	scale9Image->setColor(ccc4(51, 204, 255, 255));
	scale9Image->setTextAlignment(CATextAlignmentCenter);
	scale9Image->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(scale9Image);

	CAScale9ImageView* firstScale9Image = CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png"));
	firstScale9Image->setCenter(CADipRect(size.width*0.75, size.height*0.5, size.width*0.4, size.height*0.1));
	this->getView()->addSubview(firstScale9Image);
	CAScale9ImageView* secondScale9Image = CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png"));
	secondScale9Image->setCenter(CADipRect(size.width*0.75, size.height*0.65, size.width*0.4, size.height*0.1));
	this->getView()->addSubview(secondScale9Image);
	CAScale9ImageView* thirdScale9Image = CAScale9ImageView::createWithImage(CAImage::create("source_material/slider_indicator.png"));
	thirdScale9Image->setCenter(CADipRect(size.width*0.75, size.height*0.8, size.width*0.4, size.height*0.1));
	this->getView()->addSubview(thirdScale9Image);
}

void ImageViewTest::viewDidUnload()
{

}