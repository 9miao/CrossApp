//
//  SecondViewController.cpp
//  HelloCpp
//
//  Created by MAC on 14-6-14.
//
//

#include "SecondViewController.h"

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

}

void SecondViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    
}

void SecondViewController::touchUpInSide(CAControl* sender, CCPoint point)
{
    imageView->setImage(CAImage::create("2.jpg"));
}