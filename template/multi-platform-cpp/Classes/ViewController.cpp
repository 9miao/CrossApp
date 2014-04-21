//
//  ViewController.cpp
//  HelloCpp
//
//  Created by 栗元峰 on 14-4-21.
//
//

#include "ViewController.h"

CAWindow* ViewController::createWindow()
{
    CAWindow* _window = CAWindow::create();
    ViewController* _viewController = new ViewController();
    _viewController->init();
    _window->setRootViewController(_viewController);
    _viewController->release();
    return _window;
}

ViewController::ViewController()
{

}

ViewController::~ViewController()
{

}

void ViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    
    CAImageView* imageView = CAImageView::createWithTexture(CAImage::create("9m.jpg"));
    imageView->setFrame(CCRect(200, 300, 165, 107));
    this->getView()->addChild(imageView);
    
    CCLabelTTF* ttf = CCLabelTTF::create("Hello World!", "Arial", 30);
    ttf->setColor(ccBLACK);
    ttf->setFrame(CCRect(200, 450, 0, 0));
    this->getView()->addChild(ttf);
}

void ViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    
    
}