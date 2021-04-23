
#include "ActivityIndicatorViewTest.h"

ActivityIndicatorViewTest::ActivityIndicatorViewTest()
{
    this->setTitle("CAActivityIndicatorView");
}

ActivityIndicatorViewTest::~ActivityIndicatorViewTest()
{

}

void ActivityIndicatorViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
    
    auto idc1 = CAActivityIndicatorView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_NH_C(0.2f, 1/6.f)));
    idc1->setStyle(CAActivityIndicatorView::Style::WhiteLarge);
    this->getView()->addSubview(idc1);
    
    auto idc2 = CAActivityIndicatorView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_NH_C(0.2f, 2/6.f)));
    idc2->setStyle(CAActivityIndicatorView::Style::GrayLarge);
    this->getView()->addSubview(idc2);
    
    auto idc3 = CAActivityIndicatorView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_NH_C(0.2f, 3/6.f)));
    idc3->setStyle(CAActivityIndicatorView::Style::White);
    this->getView()->addSubview(idc3);
    
    auto idc4 = CAActivityIndicatorView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_NH_C(0.2f, 4/6.f)));
    idc4->setStyle(CAActivityIndicatorView::Style::Gray);
    this->getView()->addSubview(idc4);
    
    auto idc5 = CAActivityIndicatorView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_NH_C(0.2f, 5/6.f)));
    idc5->setStyle(CAActivityIndicatorView::Style::Image);
    this->getView()->addSubview(idc5);
    
    CAImage* image = CAImage::create("image/indicator2.png");
    CAImageView* imageView = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_B(100,100)));
    imageView->setFrame(DRect(0, 0, 100, 100));
    imageView->setImage(image);
    
    idc5->setActivityIndicatorView(imageView);
}

void ActivityIndicatorViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

