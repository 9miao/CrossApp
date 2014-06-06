//
//  ViewController.cpp
//  HelloCpp
//
//  Created by liyuanfeng on 14-4-21.
//
//

#include "FirstViewController.h"

FirstViewController::FirstViewController()
{

    
    
}

FirstViewController::~FirstViewController()
{

}

void FirstViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    CCSize size = this->getView()->getBounds().size;
    
    label = CALabel::createWithCenter(CCRect(size.width/2, 60, 200, 50));
    label->setfontSize(size.width/30);
    label->setText("0.000000");
    this->getView()->addSubview(label);
    
    
    CASlider* slider = CASlider::createWithCenter(CCRect(size.width/2, size.width * 0.8f * 0.15f / 2 + 60, size.width * 0.8f, size.width * 0.8f * 0.15f));
    this->getView()->addSubview(slider);
    slider->addTarget(this, CAControl_selector(FirstViewController::updateSlider));

}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;

}

bool FirstViewController::updateSlider(cocos2d::CAControl *sender, cocos2d::CCPoint point)
{
    CASlider* slider = (CASlider*)sender;
    
    char str[10];
    sprintf(str, "%.6f", slider->getValue());
    
    label->setText(str);
    
    return false;
}

