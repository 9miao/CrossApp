
#include "TabBarControllerTest.h"
#include "CDUIShowAutoCollectionView.h"

TabBarControllerTest::TabBarControllerTest()
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
    CATabBarItem* item1 = CATabBarItem::create("item1", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item2 = CATabBarItem::create("item2", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item3 = CATabBarItem::create("item3", CAImage::create(""), CAImage::create(""));

    std::vector<CATabBarItem*> item;
    item.push_back(item1);
    item.push_back(item2);
    item.push_back(item3);
    
    CATabBar* tabBar = CATabBar::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_H_C(98, 0.33)));
    tabBar->setItems(item);
    tabBar->showSelectedIndicator();
    tabBar->setSelectedAtIndex(0);
    tabBar->setTag(100);
    
    CATabBarItem* item4 = CATabBarItem::create("", CAImage::create("image/tab_news_btn_up.png"), CAImage::create("image/tab_news_btn_down.png"));
    CATabBarItem* item5 = CATabBarItem::create("", CAImage::create("image/tab_image_btn_up.png"), CAImage::create("image/tab_image_btn_down.png"));
    CATabBarItem* item6 = CATabBarItem::create("", CAImage::create("image/tab_mine_btn_up.png"), CAImage::create("image/tab_mine_btn_down.png"));
    
    std::vector<CATabBarItem*> it1;
    it1.push_back(item4);
    it1.push_back(item5);
    it1.push_back(item6);
    
    CATabBar* tabBar1 = CATabBar::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_H_C(98, 0.66)));
    tabBar1->setItems(it1);
    tabBar1->setBackgroundImage(CAImage::create("image/tab_news_bg.png"));
    tabBar1->setSelectedBackgroundColor(CAColor_clear);
    tabBar1->setTag(100);
    tabBar1->setSelectedAtIndex(0);
    
    CAView* view1 = CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_C(0, 0.5)));
    view1->addSubview(tabBar);
    view1->addSubview(tabBar1);
    view1->setColor(CAColor_gray);
    this->getView()->addSubview(view1);
    

}

void TabBarControllerTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

