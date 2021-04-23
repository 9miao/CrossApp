
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
    imageView->setScaleType(CAImageView::ScaleType::FitImageCrop);
	imageView->setLayout(DLayoutFill);
    this->getView()->addSubview(imageView);

    DLayout labelLayout;
	labelLayout.horizontal = DHorizontalLayout_L_R(20, 20);
	labelLayout.vertical = DVerticalLayout_T_H(200, 100);
    
    CAFont font;
    font.color = CAColor4B::WHITE;
    font.fontSize = 72;
    
    CALabel* label = CALabel::createWithLayout(labelLayout);
    label->setFont(font);
    label->setText("CrossApp");
    label->setTextAlignment(CATextAlignment::Center);
    label->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    this->getView()->addSubview(label);

}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
