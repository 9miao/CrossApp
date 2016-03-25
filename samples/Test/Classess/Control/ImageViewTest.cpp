
#include "ImageViewTest.h"

ImageViewTest::ImageViewTest():pageViewIndex(0)
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
    
    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(image1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CAImageView* image2 = CAImageView::createWithLayout(DRectLayout(175,400,400,400,DRectLayout::L_W_T_H));
    image2->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    image2->setImage(CAImage::create("image/h1.png"));
    
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view2->addSubview(image2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    CAImageView* image3 = CAImageView::createWithLayout(DRectLayout(175,400,400,400,DRectLayout::L_W_T_H));
    image3->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    image3->setImage(CAImage::create("image/h1.png"));
    
    CAView* view3 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view3->addSubview(image3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
//    CAImageView* image4 = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,400,400));
    CAImageView* image4 = CAImageView::createWithLayout(DRectLayout(175,400,400,400,DRectLayout::L_W_T_H));
    image4->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    image4->setImage(CAImage::create("image/h1.png"));
    
    CAView* view4 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view4->addSubview(image4);
    view4->setColor(CAColor_gray);
    VIEWLIST.pushBack(view4);
    
    CAImageView* image5 = CAImageView::createWithLayout(DRectLayout(175,400,300,400,DRectLayout::L_W_T_H));
    image5->setImageViewScaleType(CAImageViewScaleTypeFitViewByVertical);
    image5->setImage(CAImage::create("image/h1.png"));
    
    CAView* view5 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view5->addSubview(image5);
    view5->setColor(CAColor_gray);
    VIEWLIST.pushBack(view5);
    
    CAImageView* image6 = CAImageView::createWithLayout(DRectLayout(240,400,400,400,DRectLayout::L_W_T_H));
    image6->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    image6->setImage(CAImage::create("image/h1.png"));
    
    CAView* view6 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
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
    image7->setLayout(DRectLayout(320,320,500,500,DRectLayout::L_R_T_B));
    image7->setAnimationImages(img);
    image7->setAnimationDuration(0.2);
    image7->startAnimating();
    
    CAView* view7 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
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

void ImageViewTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(ImageViewTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(ImageViewTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);}

void ImageViewTest::buttonControlCallBack(CAControl* btn, DPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    //CCLog("btn_tag===%d",button->getTag());
    int temIndex = button->getTag();
    if (temIndex==1) {
        if (showIndex>0) {
            showIndex--;
            p_PageViewVec->setCurrPage(showIndex, true); //123456
        }else if(showIndex==0){
            showIndex=showNum-1;
            p_PageViewVec->setCurrPage(showIndex, false);
        }
    }else if(temIndex==2){
        if (showIndex<showNum) {
            showIndex++;
            if (showIndex==showNum) {
                showIndex=0;
                p_PageViewVec->setCurrPage(showIndex, false);
            }else{
                p_PageViewVec->setCurrPage(showIndex, true);
            }
        }
    }
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("ImageView"));
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageXY"));
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageCrop"));
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageInside"));
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitViewByHorizontal"));
    }else if(showIndex==5){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitViewByVertical"));
    }else if(showIndex==6){
        this->setNavigationBarItem(CANavigationBarItem::create("Frame animation"));
    }
}

void ImageViewTest::pageViewDidBeginTurning(CAPageView* pageView)
{

}

void ImageViewTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void ImageViewTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{

}
