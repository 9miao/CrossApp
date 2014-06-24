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
    label->setFontSize(size.width/30);
    label->setText("0.000000");
    this->getView()->addSubview(label);
    
    float height = size.width * 0.8f * 0.15f;
    float off_Y = size.width * 0.8f * 0.15f + 10;
    
    CCRect progress_rect = CCRect(size.width / 2, off_Y + 60, size.width * 0.8f, 10);
	progress = CAProgress::create();
	progress->setCenter(progress_rect);
	this->getView()->addSubview(progress);
    
    CCRect slider_rect = CCRect(size.width/2, off_Y * 2 + 60, size.width * 0.8f, height);
    CASlider* slider = CASlider::createWithCenter(slider_rect);
    this->getView()->addSubview(slider);
    slider->addTarget(this, CAControl_selector(FirstViewController::updateSlider));
    
    CCRect segmentedControl_rect = CCRect(size.width/2, off_Y * 3 + 60, size.width * 0.8f, height);
    CASegmentedControl* segmentedControl = CASegmentedControl::createWithCenter(segmentedControl_rect, 2);
    segmentedControl->insertSegmentWithTitle("2", 2, CAControlStateAll);
    segmentedControl->insertSegmentWithTitle("3", 3, CAControlStateAll);
    this->getView()->addSubview(segmentedControl);
    
    CAButton* btn = CAButton::createWithCenter(CCRect(size.width/2, 600, 150, 60), CAButtonTypeRoundedRect);
    btn->setTitleForState(CAControlStateAll, "OK");
    this->getView()->addSubview(btn);
    btn->addTarget(this, CAControl_selector(FirstViewController::diss), CAControlEventTouchUpInSide);
    
    
}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;

}

void FirstViewController::updateSlider(cocos2d::CAControl *sender, cocos2d::CCPoint point)
{
    CASlider* slider = (CASlider*)sender;
    
    char str[10];
    sprintf(str, "%.6f", slider->getValue());
    
    label->setText(str);
    progress->setProgress(slider->getValue());
}

void FirstViewController::diss(CAControl* sender, CCPoint point)
{
    this->dismissModalViewController(true);
}
