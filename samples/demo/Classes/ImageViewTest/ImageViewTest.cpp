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
	
	CALabel* ordinaryImage = CALabel::createWithCenter(CCRect(size.width*0.2,size.height*0.2,200,50));
	ordinaryImage->setText("Ordinary image:");
	ordinaryImage->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	ordinaryImage->setColor(ccc4(51,204,255,255));
	ordinaryImage->setTextAlignment(CATextAlignmentCenter);
	this->getView()->addSubview(ordinaryImage);

	CAImageView* firstImage = CAImageView::createWithImage(CAImage::create("btn_rounded3D_normal.png"));
	firstImage->setCenterOrigin(CCPoint(size.width*0.4, size.height*0.2));
	this->getView()->addSubview(firstImage);

	CAImageView* secondImage = CAImageView::createWithImage(CAImage::create("btn_rounded3D_selected.png"));
	secondImage->setCenterOrigin(CCPoint(size.width*0.6, size.height*0.2));
	this->getView()->addSubview(secondImage);

	CAImageView* thirdImage = CAImageView::createWithImage(CAImage::create("slider_indicator.png"));
	thirdImage->setCenterOrigin(CCPoint(size.width*0.8, size.height*0.2));
	this->getView()->addSubview(thirdImage);

	CALabel* scale9Image = CALabel::createWithCenter(CCRect(size.width*0.2, size.height*0.5, 200, 50));
	scale9Image->setText("Scale9Image:");
	scale9Image->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	scale9Image->setColor(ccc4(51, 204, 255, 255));
	scale9Image->setTextAlignment(CATextAlignmentCenter);
	this->getView()->addSubview(scale9Image);

	CAScale9ImageView* firstScale9Image = CAScale9ImageView::createWithImage(CAImage::create("btn_rounded3D_normal.png"));
	firstScale9Image->setCenter(CCRect(size.width*0.5, size.height*0.5, 200, 50));
	this->getView()->addSubview(firstScale9Image);

	CAScale9ImageView* secondScale9Image = CAScale9ImageView::createWithImage(CAImage::create("btn_rounded3D_selected.png"));
	secondScale9Image->setCenter(CCRect(size.width*0.5, size.height*0.6, 200, 50));
	this->getView()->addSubview(secondScale9Image);

	CAScale9ImageView* thirdScale9Image = CAScale9ImageView::createWithImage(CAImage::create("slider_indicator.png"));
	thirdScale9Image->setCenter(CCRect(size.width*0.5, size.height*0.7, 200, 50));
	this->getView()->addSubview(thirdScale9Image);
}

void ImageViewTest::viewDidUnload()
{

}