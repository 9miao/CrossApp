
#include "ImageViewTest.h"
#include "CDUIShowAutoCollectionView.h"

int ImageViewNum = 0;

ImageViewTest::ImageViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

ImageViewTest::~ImageViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
    ImageViewNum = 0;
}

void CDUIShowAutoCollectionView::ImageViewRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showImageViewNavigationBar >= 4)
    {
        showImageViewNavigationBar = 0;
        ImageViewNum = showImageViewNavigationBar;
    }
    else
    {
        ImageViewNum = ++showImageViewNavigationBar;
    }
    
    ImageViewTest* ViewContrllerImageViewTest = new ImageViewTest();
    ViewContrllerImageViewTest->init();
    ViewContrllerImageViewTest->setNavigationBarItem(ImageViewNavigationBar);
    ViewContrllerImageViewTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerImageViewTest, false);

}

void ImageViewTest::viewDidLoad()
{
    if (ImageViewNum == 0)
    {
        CAImageView* image1 = CAImageView::createWithImage(CAImage::create("image/h1.png"));
        image1->setLayout(DLayoutFill);
    
        CAView* view1 = CAView::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(50, 50)));
        view1->addSubview(image1);
        view1->setColor(CAColor_gray);
        this->getView()->addSubview(view1);
    }
    else if (ImageViewNum == 1)
    {
        CAImageView* image2 = CAImageView::createWithLayout(DLayoutFill);
        image2->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        image2->setImage(CAImage::create("image/h1.png"));
    
        CAView* view2 = CAView::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(50, 50)));
        view2->addSubview(image2);
        view2->setColor(CAColor_gray);
        this->getView()->addSubview(view2);
    }
    else if (ImageViewNum == 2)
    {
        CAImageView* image3 = CAImageView::createWithLayout(DLayoutFill);
        image3->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        image3->setImage(CAImage::create("image/h1.png"));
    
        CAView* view3 = CAView::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(50, 50)));
        view3->addSubview(image3);
        view3->setColor(CAColor_gray);
        this->getView()->addSubview(view3);
    }
    else if (ImageViewNum == 3)
    {
        CAImageView* image4 = CAImageView::createWithLayout(DLayoutFill);
        image4->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        image4->setImage(CAImage::create("image/h1.png"));
    
        CAView* view4 = CAView::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(50, 50)));
        view4->addSubview(image4);
        view4->setColor(CAColor_gray);
        this->getView()->addSubview(view4);
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
        CAImageView* image7 = CAImageView::createWithImage(CAImage::create("animation/npc_382-1.png"));
        image7->setLayout(DLayoutFill);
        image7->setAnimationImages(img);
        image7->setAnimationDuration(0.2);
        image7->startAnimating();
    
        CAView* view7 = CAView::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(50, 50)));
        view7->addSubview(image7);
        view7->setColor(CAColor_gray);
        this->getView()->addSubview(view7);
    }
}

void ImageViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


