
#include "ImageViewTest.h"

ImageViewTest::ImageViewTest()
:m_iIndex(0)
{
    this->setTitle("CAImageView");
}

ImageViewTest::~ImageViewTest()
{

}

void ImageViewTest::viewDidLoad()
{
    CAView* view = CAView::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(50, 80)));
    view->setColor(CAColor4B::GRAY);
    this->getView()->addSubview(view);
    
    if (m_iIndex == 0)
    {
        CAImageView* image2 = CAImageView::createWithImage(CAImage::create("image/h1.png"));
        image2->setScaleType(CAImageView::ScaleType::FitImageXY); //default
        image2->setLayout(DLayoutFill);
        view->addSubview(image2);
    }
    else if (m_iIndex == 1)
    {
        CAImageView* image3 = CAImageView::createWithImage(CAImage::create("image/h1.png"));
        image3->setScaleType(CAImageView::ScaleType::FitImageCrop);
        image3->setLayout(DLayoutFill);
        view->addSubview(image3);
    }
    else if (m_iIndex == 2)
    {
        CAImageView* image4 = CAImageView::createWithImage(CAImage::create("image/h1.png"));
        image4->setScaleType(CAImageView::ScaleType::FitImageInside);
        image4->setLayout(DLayoutFill);
        view->addSubview(image4);
    }
    else
    {
        CAVector<CAImage* > img;
        for (int i=0; i<8; i++)
        {
            char temp[50];
            sprintf(temp, "animation/npc_382-%d.png",i+1);
            CAImage* im = CAImage::create(temp);
            img.pushBack(im);
        }
        CAImageView* image7 = CAImageView::createWithLayout(DLayout(DHorizontalLayout_W_C(96, 0.5f), DVerticalLayout_H_C(176, 0.5f)));
        image7->setAnimationImages(img);
        image7->setAnimationDuration(0.2);
        image7->startAnimating();
        view->addSubview(image7);
    }
    
    
    auto btn = CAButton::createWithLayout(DLayout(DHorizontalLayout_NW_C(0.3,0.5), DVerticalLayout_B_H(20 ,50)), CAButton::Type::RoundedRect);
    btn->setTitleForState(CAControl::State::Normal, "Switch Next");
    btn->addTarget([&] {
        
        ssize_t index = m_iIndex + 1;
        if (index > 3)
        {
            index = 0;
        }
        ImageViewTest* viewController = ImageViewTest::create();
        viewController->setIndex(index);
        RootWindow::getInstance()->getRootNavigationController()->replaceViewController(viewController, false);
        
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn);

}

void ImageViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

