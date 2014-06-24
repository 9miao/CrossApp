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
	CCRect winRect = this->getView()->getBounds();
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("HelloWorld.png"));
    imageView->setFrame(winRect);
    this->getView()->addSubview(imageView);
    
    CALabel* label = CALabel::createWithCenter(CCRect(winRect.size.width*0.5, winRect.size.height*0.5-270, winRect.size.width, 200));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(72);
    label->setFontName("CODE LIGHT");
    label->setText("Hello World!");
    label->setColor(CAColor_white);
    this->getView()->insertSubview(label, 1);
}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}