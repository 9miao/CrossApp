
#include "FirstViewController.h"

FirstViewController::FirstViewController()
{

}

FirstViewController::~FirstViewController()
{

}

void FirstViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
	DRect winRect = this->getView()->getBounds();
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("r/HelloWorld.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    imageView->setFrame(winRect);
    this->getView()->addSubview(imageView);

    DRect rect;
    rect.size = DSize(600, 200);
    rect.origin = winRect.size/2;
    rect.origin.y -= 360;
    CALabel* label = CALabel::createWithCenter(rect);
    label->setTextAlignment(CATextAlignmentCenter);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(72);
    label->setText("Hello World!");
    label->setColor(CAColor_white);
    this->getView()->addSubview(label);
}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}