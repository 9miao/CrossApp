//
//  ViewController.cpp
//  project
//
//  Created by 栗元峰 on 14-4-16.
//
//

#include "ViewController.h"
#include "LMenu.h"

static int count = 0;

void ViewController::viewDidLoad()
{
    ccColor3B color = ccRED;
    if (this->getTitle().compare("view1") == 0)
    {
        color = ccBLACK;
    }
    else if (this->getTitle().compare("view2") == 0)
    {
        color = ccBLUE;
        LMenu* menu = LMenu::createWithSprite(CCSprite::create("9m.jpg"));
        menu->setFrame(CCRectMake(100, 100, 0, 0));
        this->getView()->addChild(menu);
        menu->addTarget(this, LMenu_selector(ViewController::touchUpInSide), TouchUpInSide);
        
        LMenu* menu2 = LMenu::createWithSprite(CCSprite::create("9m.jpg"));
        menu2->setFrame(CCRectMake(400, 100, 0, 0));
        this->getView()->addChild(menu2);
        menu2->addTarget(this, LMenu_selector(ViewController::touchUpInSide2), TouchUpInSide);
        
        ttf = CCLabelTTF::create(CCString::createWithFormat("VIEW %d", count)->getCString(), "Arial", 50);
        ttf->setFrame(CCRectMake(100, 400, 0, 0));
        this->getView()->addChild(ttf);
        
    }
    else if (this->getTitle().compare("view3") == 0)
    {
        color = ccYELLOW;
    }
    else if (this->getTitle().compare("view4") == 0)
    {
        color = ccGRAY;
    }
    else if (this->getTitle().compare("view5") == 0)
    {
        color = ccRED;
    }

    this->getView()->setColor(color);
}

void ViewController::viewDidUnload()
{
    this->getView()->removeAllChildren();
}

void ViewController::touchUpInSide()
{
    ++count;
    
    ViewController* viewController2 = new ViewController();
    viewController2->init();
    viewController2->setTabBarItem(CATabBarItem::create("222", CAImage::create("e.png"), 101));
    viewController2->setNavigationBarItem(CANavigationBarItem::create(CCString::createWithFormat("ViewController %d", count)->getCString()));
    viewController2->setTitle("view2");

    if (this->getNavigationController())
    {
        this->getNavigationController()->pushViewController(viewController2, true);
        --count;
        return;
    }
    
    if (this->getTabBarController())
    {
        this->getTabBarController()->getNavigationController()->pushViewController(viewController2, true);
        --count;
        return;
    }
}

void ViewController::touchUpInSide2()
{
    if (this->getNavigationController() && this->getNavigationController()->popViewControllerAnimated(true))
    {
        --count;
        return;
    }
    
    if (this->getTabBarController() && this->getTabBarController()->getNavigationController()->popViewControllerAnimated(true))
    {
        --count;
        return;
    }
    
}