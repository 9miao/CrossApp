//
//  SecondViewController.cpp
//  HelloCpp
//
//  Created by MAC on 14-6-14.
//
//

#include "SecondViewController.h"
#include "FirstViewController.h"
SecondViewController::SecondViewController()
{

}

SecondViewController::~SecondViewController()
{

}

void SecondViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    CCSize size = this->getView()->getBounds().size;
    
    CAButton* btn = CAButton::createWithCenter(CCRect(size.width/2, 600, 150, 60), CAButtonTypeRoundedRect);
    btn->setTitleForState(CAControlStateAll, "pressenView");
    this->getView()->addSubview(btn);
    btn->addTarget(this, CAControl_selector(SecondViewController::touchUpInSide), CAControlEventTouchUpInSide);
}

void SecondViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    
    
}

void SecondViewController::touchUpInSide(CAControl* sender, CCPoint point)
{
    //imageView->setImage(CAImage::create("2.jpg"));
    
    FirstViewController* viewController = new FirstViewController();
    viewController->init();
    
    this->presentModalViewController(viewController, true);
}