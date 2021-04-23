
#include "RenderImageTest.h"
#include "RootWindow.h"

RenderImageTest::RenderImageTest()
{
    this->setTitle("CARenderImage");
}

RenderImageTest::~RenderImageTest()
{
}

void RenderImageTest::viewDidLoad()
{
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("image/h1.png"));
    imageView->setLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(50, 50)));
    imageView->setScaleType(CAImageView::ScaleType::FitImageInside);
    this->getView()->addSubview(imageView);
    imageView->setDisplayRange(false);
    

    CAButton* button = CAButton::createWithLayout(DLayout(DHorizontalLayout_W_C(200, 0.5), DVerticalLayout_B_H(20, 60)) , CAButton::Type::RoundedRect);
    button->setTitleForState(CAControl::State::Normal,"截屏");
    this->getView()->addSubview(button);
    button->addTarget([=]
    {
        imageView->setImage(CARenderImage::printscreen()->getImageView()->getImage()) ;

    }, CAButton::Event::TouchUpInSide);
}

void RenderImageTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


