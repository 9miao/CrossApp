
#include "ActivityIndicatorViewTest.h"

ActivityIndicatorViewTest::ActivityIndicatorViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

ActivityIndicatorViewTest::~ActivityIndicatorViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void ActivityIndicatorViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    showNum = 5;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    CAActivityIndicatorView* idc1 = CAActivityIndicatorView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    idc1->setStyle(CAActivityIndicatorViewStyleWhiteLarge);
    idc1->startAnimating();
    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(idc1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CAActivityIndicatorView* idc2 = CAActivityIndicatorView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    idc2->setStyle(CAActivityIndicatorViewStyleGrayLarge);
    idc2->startAnimating();
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view2->addSubview(idc2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    CAActivityIndicatorView* idc3 = CAActivityIndicatorView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    idc3->setStyle(CAActivityIndicatorViewStyleWhite);
    idc2->setCycleTime(1.0f);
    idc2->setTimesOneCycle(12);
    idc2->startAnimating();
    CAView* view3 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view3->addSubview(idc3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
    CAActivityIndicatorView* idc4 = CAActivityIndicatorView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    idc4->setStyle(CAActivityIndicatorViewStyleGray);
    idc4->setCycleTime(1.0f);
    idc4->setTimesOneCycle(12);
    idc4->startAnimating();
    CAView* view4 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view4->addSubview(idc4);
    view4->setColor(CAColor_gray);
    VIEWLIST.pushBack(view4);
    
    CAImage* image = CAImage::create("image/indicator2.png");
//    CAImageView* imageView = CAImageView::createWithCenter(DRect(0, 0, 100, 100));
    CAImageView* imageView = CAImageView::createWithLayout(DRectLayout(100,100,100,100,DRectLayout::L_W_T_H));
    imageView->setFrame(DRect(0, 0, 100, 100));
    imageView->setImage(image);
    CCLog("imageeView.w = ");
    CAActivityIndicatorView* idc5 = CAActivityIndicatorView::createWithLayout(DRectLayout(100,100,100,100,DRectLayout::L_R_T_B));
    idc5->setStyle(CAActivityIndicatorViewStyleImage);
    idc5->setActivityIndicatorView(imageView);
    idc5->startAnimating();
    CAView* view5 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view5->addSubview(idc5);
    view5->setColor(CAColor_gray);
    VIEWLIST.pushBack(view5);
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void ActivityIndicatorViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

