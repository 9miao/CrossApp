
#include "TabBarTest.h"
#include "FirstViewController.h"

TabBarTest::TabBarTest()
{
    this->setTitle("CATabBar");
}

TabBarTest::~TabBarTest()
{
}

void TabBarTest::viewDidLoad()
{
    CATabBarItem* item1 = CATabBarItem::create("item1", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item2 = CATabBarItem::create("item2", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item3 = CATabBarItem::create("item3", CAImage::create(""), CAImage::create(""));

    CAVector<CATabBarItem*> item;
    item.pushBack(item1);
    item.pushBack(item2);
    item.pushBack(item3);
    
    CATabBar* tabBar = CATabBar::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_H_C(98, 0.33)));
    tabBar->setItems(item);
    tabBar->showSelectedIndicator();
    tabBar->setSelectedAtIndex(0);
    tabBar->setTag(100);
    
    this->getView()->addSubview(tabBar);
}

void TabBarTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

