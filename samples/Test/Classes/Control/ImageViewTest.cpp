
#include "ImageViewTest.h"
#include "CDUIShowAutoCollectionView.h"

ImageViewTest::ImageViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

ImageViewTest::~ImageViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

int ImageViewNum = 0;

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
        image1->setFrame(this->getView()->getBounds());
    
//        CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
        CAView* view1 = CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(0, 100)));
        view1->addSubview(image1);
        view1->setColor(CAColor_gray);
        this->getView()->addSubview(view1);
    }
    else if (ImageViewNum == 1)
    {
//        CAImageView* image2 = CAImageView::createWithLayout(DRectLayout(200,200,100,400,DRectLayout::L_R_T_H));
        CAImageView* image2 = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(200, 200), DVerticalLayout_T_H(100, 400)));
        image2->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        image2->setImage(CAImage::create("image/h1.png"));
    
        CAView* view2 = CAView::createWithLayout(DLayoutFill);
        view2->addSubview(image2);
        view2->setColor(CAColor_gray);
        this->getView()->addSubview(view2);
    }
    else if (ImageViewNum == 2)
    {
//        CAImageView* image3 = CAImageView::createWithLayout(DRectLayout(100,400,100,400,DRectLayout::L_W_T_H));
        CAImageView* image3 = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_W(100, 400), DVerticalLayout_T_H(100, 400)));
        image3->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        image3->setImage(CAImage::create("image/h1.png"));
    
        CAView* view3 = CAView::createWithLayout(DLayoutFill);
        view3->addSubview(image3);
        view3->setColor(CAColor_gray);
        this->getView()->addSubview(view3);
    }
    else if (ImageViewNum == 3)
    {
//    CAImageView* image4 = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,400,400));
//        CAImageView* image4 = CAImageView::createWithLayout(DRectLayout(200,200,100,400,DRectLayout::L_R_T_H));
        CAImageView* image4 = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(200, 200), DVerticalLayout_T_H(100, 400)));
        image4->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        image4->setImage(CAImage::create("image/h1.png"));
    
        CAView* view4 = CAView::createWithLayout(DLayoutFill);
        view4->addSubview(image4);
        view4->setColor(CAColor_gray);
        this->getView()->addSubview(view4);
    }
//    else if (ImageViewNum == 4)
//    {
//        CAImageView* image5 = CAImageView::createWithLayout(DRectLayout(100,400,100,400,DRectLayout::L_W_T_H));
//        image5->setImageViewScaleType(CAImageViewScaleTypeFitViewByVertical);
//        image5->setImage(CAImage::create("image/h1.png"));
//    
//        CAView* view5 = CAView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
//        view5->addSubview(image5);
//        view5->setColor(CAColor_gray);
//        this->getView()->addSubview(view5);
//    }
//    else if (ImageViewNum == 5)
//    {
//        CAImageView* image6 = CAImageView::createWithLayout(DRectLayout(200,200,100,400,DRectLayout::L_R_T_H));
//        image6->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
//        image6->setImage(CAImage::create("image/h1.png"));
//    
//        CAView* view6 = CAView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
//        view6->addSubview(image6);
//        view6->setColor(CAColor_gray);
//        this->getView()->addSubview(view6);
//    }
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
//        image7->setLayout(DRectLayout(200,80,100,196,DRectLayout::L_W_T_H));
        image7->setLayout(DLayout(DHorizontalLayout_L_W(200, 80), DVerticalLayout_T_H(100, 196)));
        image7->setAnimationImages(img);
        image7->setAnimationDuration(0.2);
        image7->startAnimating();
    
        CAView* view7 = CAView::createWithLayout(DLayoutFill);
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


