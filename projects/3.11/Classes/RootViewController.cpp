//
//  RootViewController.cpp
//  project
//
//  Created by 栗元峰 on 14-4-11.
//
//

#include "RootViewController.h"
#include "LTableBarController.h"

void RootViewController::viewDidLoad()
{
    LTableBarController* layer = LTableBarController::shareTableBarController();
    layer->setFrame(CCRect(0, 0, 0, 0));
    this->getView()->addChild(layer);

//    RootViewController* viewController = new RootViewController();
//    viewController->init();
//    viewController->setTabBarItem(CATabBarItem::create("111", CAImage::create("a.png"), 100));
//    
//    RootViewController* viewController2 = new RootViewController();
//    viewController2->init();
//    viewController2->setTabBarItem(CATabBarItem::create("222", CAImage::create("e.png"), 101));
//    
//    RootViewController* viewController3 = new RootViewController();
//    viewController3->init();
//    viewController3->setTabBarItem(CATabBarItem::create("333", CAImage::create("h.png"), 102));
//    
//    RootViewController* viewController4 = new RootViewController();
//    viewController4->init();
//    viewController4->setTabBarItem(CATabBarItem::create("444", CAImage::create("o.png"), 103));
//    
//    RootViewController* viewController5 = new RootViewController();
//    viewController5->init();
//    viewController5->setTabBarItem(CATabBarItem::create("555", CAImage::create("s.png"), 104));
//    
//    std::vector<CAViewController*> views;
//    views.push_back(viewController);
//    views.push_back(viewController2);
//    views.push_back(viewController3);
//    views.push_back(viewController4);
//    views.push_back(viewController5);
//    
//    CATabBarController* tabBarController = new CATabBarController();
//    tabBarController->initWithViewControllers(views, true);
//    tabBarController->getTabBar()->setMaxShowCount(5);
//    this->getView()->addChild(tabBarController->getView());
}

void RootViewController::viewDidUnload()
{
    LTableBarController::shareNull();
}