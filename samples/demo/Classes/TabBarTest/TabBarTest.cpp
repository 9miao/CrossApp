#include "TabBarTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

TabBarTest::TabBarTest()
{

}

TabBarTest::~TabBarTest()
{

}

void TabBarTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	this->getNavigationBarItem()->setShowGoBackButton(false);

	CCSize navigationBarSize = this->getNavigationController()->getNavigationBar()->getBounds().size;
	returnBtn = CAButton::createWithCenter(CCRect(navigationBarSize.width*0.1, navigationBarSize.height*0.5, 100, 40), CAButtonTypeCustom);
	returnBtn->setImageForState(CAControlStateAll,CAImage::create("source_material/btn_left_white.png"));
	returnBtn->setImageColorForState(CAControlStateHighlighted, ccc4(0, 255, 200, 255));
	returnBtn->addTarget(this, CAControl_selector(TabBarTest::goPrevious), CAControlEventTouchUpInSide);
	this->getNavigationController()->getNavigationBar()->addSubview(returnBtn);

	firstTabBar();
	secondTabBar();
	thirdTabBar();
}

void TabBarTest::tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index)
{

}

void TabBarTest::goPrevious(CAControl* btn, CCPoint point)
{
	this->getNavigationController()->getTabBarController()->setTabBarHidden(false, false);
	this->getNavigationController()->popViewControllerAnimated(true);
}

void TabBarTest::firstTabBar()
{
	CATabBarItem* item1 = CATabBarItem::create("Edit", CAImage::create("tabbar_function/first_1.png"), CAImage::create("tabbar_function/first_2.png"));
	CATabBarItem* item2 = CATabBarItem::create("Friends", CAImage::create("tabbar_function/second_1.png"), CAImage::create("tabbar_function/second_2.png"));
	CATabBarItem* item3 = CATabBarItem::create("Message", CAImage::create("tabbar_function/third_1.png"), CAImage::create("tabbar_function/third_2.png"));
	CATabBarItem* item4 = CATabBarItem::create("Search", CAImage::create("tabbar_function/fourth_1.png"), CAImage::create("tabbar_function/fourth_2.png"));
	CATabBarItem* item5 = CATabBarItem::create("Setting", CAImage::create("tabbar_function/fifth_1.png"), CAImage::create("tabbar_function/fifth_2.png"));

	std::vector<CATabBarItem*> item;
	item.push_back(item1);
	item.push_back(item2);
	item.push_back(item3);
	item.push_back(item4);
	item.push_back(item5);

	CATabBar* tabBar = CATabBar::create(item);
	tabBar->setCenter(CCRect(size.width*0.5, size.height*0.5 + 200, size.width, 100));
	tabBar->showSelectedIndicator();
	tabBar->setDelegate(this);
	this->getView()->addSubview(tabBar);
    tabBar->setSelectedAtIndex(2);
}

void TabBarTest::secondTabBar()
{
	CATabBarItem* item1 = CATabBarItem::create("Edit", CAImage::create("tabbar_function/first_1.png"), CAImage::create("tabbar_function/first_2.png"));
	CATabBarItem* item2 = CATabBarItem::create("Friends", CAImage::create("tabbar_function/second_1.png"), CAImage::create("tabbar_function/second_2.png"));
	CATabBarItem* item3 = CATabBarItem::create("Message", CAImage::create("tabbar_function/third_1.png"), CAImage::create("tabbar_function/third_2.png"));
	CATabBarItem* item4 = CATabBarItem::create("Search", CAImage::create("tabbar_function/fourth_1.png"), CAImage::create("tabbar_function/fourth_2.png"));
	CATabBarItem* item5 = CATabBarItem::create("Setting", CAImage::create("tabbar_function/fifth_1.png"), CAImage::create("tabbar_function/fifth_2.png"));

	std::vector<CATabBarItem*> item;
	item.push_back(item1);
	item.push_back(item2);
	item.push_back(item3);
	item.push_back(item4);
	item.push_back(item5);

	CATabBar* tabBar = CATabBar::create(item);
	tabBar->setCenter(CCRect(size.width*0.5, size.height*0.5 , size.width, 100));
	tabBar->setSelectedIndicatorImage(CAImage::create("source_material/indicator.png"));
	tabBar->showSelectedIndicator();
	tabBar->setBackGroundImage(CAImage::create("source_material/ex1.png"));
	tabBar->setSelectedBackGroundImage(CAImage::create("source_material/btn_rounded3D_selected.png"));
	tabBar->setDelegate(this);
	this->getView()->addSubview(tabBar);
    tabBar->setSelectedAtIndex(3);
}

void TabBarTest::thirdTabBar()
{
	CATabBarItem* item1 = CATabBarItem::create("Edit", CAImage::create("tabbar_function/first_1.png"), CAImage::create("tabbar_function/first_2.png"));
	CATabBarItem* item2 = CATabBarItem::create("Friends", CAImage::create("tabbar_function/second_1.png"), CAImage::create("tabbar_function/second_2.png"));
	CATabBarItem* item3 = CATabBarItem::create("Message", CAImage::create("tabbar_function/third_1.png"), CAImage::create("tabbar_function/third_2.png"));
	CATabBarItem* item4 = CATabBarItem::create("Search", CAImage::create("tabbar_function/fourth_1.png"), CAImage::create("tabbar_function/fourth_2.png"));
	CATabBarItem* item5 = CATabBarItem::create("Setting", CAImage::create("tabbar_function/fifth_1.png"), CAImage::create("tabbar_function/fifth_2.png"));

	std::vector<CATabBarItem*> item;
	item.push_back(item1);
	item.push_back(item2);
	item.push_back(item3);
	item.push_back(item4);
	item.push_back(item5);

	CATabBar* tabBar = CATabBar::create(item);
	tabBar->setCenter(CCRect(size.width*0.5, size.height*0.5 - 200, size.width, 100));
	tabBar->showSelectedIndicator();
	tabBar->setTitleColorForNormal(CAColor_yellow);
	tabBar->setTitleColorForSelected(CAColor_orange);
	tabBar->setSelectedBackGroundImage(CAImage::create("source_material/alert_btn.png"));
	tabBar->setMaxShowCount(3);
	tabBar->setDelegate(this);
	this->getView()->addSubview(tabBar);
    tabBar->setSelectedAtIndex(1);
}

void TabBarTest::viewDidUnload()
{
	returnBtn->removeFromSuperview();
}