
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
    imageView->setLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
    this->getView()->addSubview(imageView);

    DRectLayout labelLayout;
    labelLayout.left = 20;
    labelLayout.right = 20;
    labelLayout.top = 200;
    labelLayout.height = 100;
    
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