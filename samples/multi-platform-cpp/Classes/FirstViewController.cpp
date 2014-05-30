//
//  ViewController.cpp
//  HelloCpp
//
//  Created by liyuanfeng on 14-4-21.
//
//

#include "FirstViewController.h"

CAWindow* FirstViewController::createWindow()
{
    CAWindow* _window = CAWindow::create();
	FirstViewController* _viewController = new FirstViewController();
    _viewController->init();
    _window->setRootViewController(_viewController);
    _viewController->release();
    return _window;
}

FirstViewController::FirstViewController()
{

}

FirstViewController::~FirstViewController()
{

}

void FirstViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("9m.jpg"));
    imageView->setFrame(CCRect(200, 300, 330, 214));
    this->getView()->addSubview(imageView);
    
    CCLabelTTF* ttf = CCLabelTTF::create("Hello World!", "Arial", 50);
    ttf->setColor(ccBLACK);
    ttf->setFrame(CCRect(200, 450, 0, 0));
    this->getView()->addSubview(ttf);

}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;

    
}