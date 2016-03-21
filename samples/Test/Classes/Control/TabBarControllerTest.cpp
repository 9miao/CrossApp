
#include "TabBarControllerTest.h"

TabBarControllerTest::TabBarControllerTest():pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

TabBarControllerTest::~TabBarControllerTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void TabBarControllerTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    showNum = 2;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    CATabBarItem* item1 = CATabBarItem::create("item1", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item2 = CATabBarItem::create("item2", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item3 = CATabBarItem::create("item3", CAImage::create(""), CAImage::create(""));
    
    std::vector<CATabBarItem*> item;
    item.push_back(item1);
    item.push_back(item2);
    item.push_back(item3);
    
//    CATabBar* tabBar = CATabBar::createWithFrame(DRect(0, winSize.height-200, winSize.width, 98));
    CATabBar* tabBar = CATabBar::createWithLayout(DRectLayout(0,0,0,98,DRectLayout::L_R_B_H));
    tabBar->setItems(item);
    tabBar->showSelectedIndicator();
    tabBar->setSelectedAtIndex(0);
//    tabBar->setBackgroundImage(CAImage::create("image/tab_news_bg.png"));
    tabBar->setTag(100);
    
    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(tabBar);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CATabBarItem* item4 = CATabBarItem::create("", CAImage::create("image/tab_news_btn_up.png"), CAImage::create("image/tab_news_btn_down.png"));
    CATabBarItem* item5 = CATabBarItem::create("", CAImage::create("image/tab_image_btn_up.png"), CAImage::create("image/tab_image_btn_down.png"));
    CATabBarItem* item6 = CATabBarItem::create("", CAImage::create("image/tab_mine_btn_up.png"), CAImage::create("image/tab_mine_btn_down.png"));
    
    std::vector<CATabBarItem*> it1;
    it1.push_back(item4);
    it1.push_back(item5);
    it1.push_back(item6);
    
    CATabBar* tabBar1 = CATabBar::createWithLayout(DRectLayout(0,0,0,98,DRectLayout::L_R_B_H));
    tabBar1->setItems(it1);
    tabBar1->setBackgroundImage(CAImage::create("image/tab_news_bg.png"));
    tabBar1->setSelectedBackgroundColor(CAColor_clear);
    tabBar1->setTag(100);
    tabBar1->setSelectedAtIndex(0);
    
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view2->addSubview(tabBar1);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void TabBarControllerTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void TabBarControllerTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(TabBarControllerTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(TabBarControllerTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void TabBarControllerTest::buttonControlCallBack(CAControl* btn, DPoint point)
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
    
}

void TabBarControllerTest::pageViewDidBeginTurning(CAPageView* pageView)
{

}

void TabBarControllerTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void TabBarControllerTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{

}