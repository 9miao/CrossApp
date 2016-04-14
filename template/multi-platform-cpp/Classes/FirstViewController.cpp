
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
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("r/HelloWorld.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	imageView->setLayout(DLayoutFill);
    this->getView()->addSubview(imageView);

    DLayout labelLayout;
	labelLayout.horizontal = DHorizontalLayout_L_R(20, 20);
	labelLayout.vertical = DVerticalLayout_T_H(200, 100);
    
    CAFont font;
    font.color = CAColor_white;
    font.fontSize = 72;
    
    CALabel* label = CALabel::createWithLayout(labelLayout);
    label->setTextAlignment(CATextAlignmentCenter);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFont(font);
    label->setText("Hello World!");
    this->getView()->addSubview(label);
}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}