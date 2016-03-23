
#include "ActivityIndicatorViewTest.h"

ActivityIndicatorViewTest::ActivityIndicatorViewTest():pageViewIndex(0)
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

void ActivityIndicatorViewTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(ActivityIndicatorViewTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(ActivityIndicatorViewTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void ActivityIndicatorViewTest::buttonControlCallBack(CAControl* btn, DPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    CCLog("btn_tag===%d",button->getTag());
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
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView"));
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView1"));
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView2"));
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView3"));
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView4"));
    }
}

void ActivityIndicatorViewTest::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void ActivityIndicatorViewTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void ActivityIndicatorViewTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{

}