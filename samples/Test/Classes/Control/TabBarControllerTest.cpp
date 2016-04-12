
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

int TabBarControllerNum = 0;

void CDUIShowAutoCollectionView::TabBarRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showTabBarNavigationBar >= 1)
    {
        showTabBarNavigationBar = 0;
        TabBarControllerNum = showTabBarNavigationBar;
    }
    else
    {
        TabBarControllerNum = ++showTabBarNavigationBar;
    }
    
    TabBarControllerTest* ViewContrllerTabBarControllerTest = new TabBarControllerTest();
    ViewContrllerTabBarControllerTest->init();
    ViewContrllerTabBarControllerTest->setNavigationBarItem(TabBarNavigationBar);
    ViewContrllerTabBarControllerTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerTabBarControllerTest, false);
}

void TabBarControllerTest::viewDidLoad()
{
    if (TabBarControllerNum == 0)
    {
        CATabBarItem* item1 = CATabBarItem::create("item1", CAImage::create(""), CAImage::create(""));
        CATabBarItem* item2 = CATabBarItem::create("item2", CAImage::create(""), CAImage::create(""));
        CATabBarItem* item3 = CATabBarItem::create("item3", CAImage::create(""), CAImage::create(""));
    
        std::vector<CATabBarItem*> item;
        item.push_back(item1);
        item.push_back(item2);
        item.push_back(item3);
    
//        CATabBar* tabBar = CATabBar::createWithFrame(DRect(0, winSize.height-200, winSize.width, 98));
        CATabBar* tabBar = CATabBar::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(0, 98)));
        tabBar->setItems(item);
        tabBar->showSelectedIndicator();
        tabBar->setSelectedAtIndex(0);
//        tabBar->setBackgroundImage(CAImage::create("image/tab_news_bg.png"));
        tabBar->setTag(100);
    
        CAView* view1 = CAView::createWithLayout(DLayoutFill);
        view1->addSubview(tabBar);
        view1->setColor(CAColor_gray);
        this->getView()->addSubview(view1);
    }
    else
    {
        CATabBarItem* item4 = CATabBarItem::create("", CAImage::create("image/tab_news_btn_up.png"), CAImage::create("image/tab_news_btn_down.png"));
        CATabBarItem* item5 = CATabBarItem::create("", CAImage::create("image/tab_image_btn_up.png"), CAImage::create("image/tab_image_btn_down.png"));
        CATabBarItem* item6 = CATabBarItem::create("", CAImage::create("image/tab_mine_btn_up.png"), CAImage::create("image/tab_mine_btn_down.png"));
    
        std::vector<CATabBarItem*> it1;
        it1.push_back(item4);
        it1.push_back(item5);
        it1.push_back(item6);
    
        CATabBar* tabBar1 = CATabBar::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(0, 98)));
        tabBar1->setItems(it1);
        tabBar1->setBackgroundImage(CAImage::create("image/tab_news_bg.png"));
        tabBar1->setSelectedBackgroundColor(CAColor_clear);
        tabBar1->setTag(100);
        tabBar1->setSelectedAtIndex(0);
    
        CAView* view2 = CAView::createWithLayout(DLayoutFill);
        view2->addSubview(tabBar1);
        view2->setColor(CAColor_gray);
        this->getView()->addSubview(view2);
    }
}

void TabBarControllerTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

