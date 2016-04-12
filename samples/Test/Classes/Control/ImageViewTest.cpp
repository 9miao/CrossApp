
#include "ImageViewTest.h"

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

void ImageViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    showNum = 7;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    CAImageView* image1 = CAImageView::createWithImage(CAImage::create("image/h1.png"));
    image1->setFrame(this->getView()->getBounds());
    
    CAView* view1 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view1->addSubview(image1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CAImageView* image2 = CAImageView::createWithLayout(DLayout(200,200,100,400,DLayout::L_R_T_H));
    image2->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    image2->setImage(CAImage::create("image/h1.png"));
    
    CAView* view2 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view2->addSubview(image2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    CAImageView* image3 = CAImageView::createWithLayout(DLayout(100,400,100,400,DLayout::L_W_T_H));
    image3->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    image3->setImage(CAImage::create("image/h1.png"));
    
    CAView* view3 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view3->addSubview(image3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
//    CAImageView* image4 = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,400,400));
    CAImageView* image4 = CAImageView::createWithLayout(DLayout(200,200,100,400,DLayout::L_R_T_H));
    image4->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    image4->setImage(CAImage::create("image/h1.png"));
    
    CAView* view4 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view4->addSubview(image4);
    view4->setColor(CAColor_gray);
    VIEWLIST.pushBack(view4);
    
    CAImageView* image5 = CAImageView::createWithLayout(DLayout(100,400,100,400,DLayout::L_W_T_H));
    image5->setImageViewScaleType(CAImageViewScaleTypeFitViewByVertical);
    image5->setImage(CAImage::create("image/h1.png"));
    
    CAView* view5 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view5->addSubview(image5);
    view5->setColor(CAColor_gray);
    VIEWLIST.pushBack(view5);
    
    CAImageView* image6 = CAImageView::createWithLayout(DLayout(100,400,100,400,DLayout::L_W_T_H));
    image6->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    image6->setImage(CAImage::create("image/h1.png"));
    
    CAView* view6 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view6->addSubview(image6);
    view6->setColor(CAColor_gray);
    VIEWLIST.pushBack(view6);
    
    CAVector<CAImage* > img;
    for (int i=0; i<8; i++) {
        char temp[50];
        sprintf(temp, "animation/npc_382-%d.png",i+1);
        CAImage* im = CAImage::create(temp);
        img.pushBack(im);
    }
    CAImageView* image7 = CAImageView::createWithImage(CAImage::create("animation/npc_382-1.png"));
    image7->setLayout(DLayout(240,240,100,280,DLayout::L_R_T_H));
    image7->setAnimationImages(img);
    image7->setAnimationDuration(0.2);
    image7->startAnimating();
    
    CAView* view7 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view7->addSubview(image7);
    view7->setColor(CAColor_gray);
    VIEWLIST.pushBack(view7);
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void ImageViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


