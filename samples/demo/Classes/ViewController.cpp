//
//  ViewController.cpp
//  project
//
//  Created by  on 14-4-16.
//
//

#include "ViewController.h"

ViewController::ViewController()
{
    scroll = NULL;
    v = NULL;
}

ViewController::~ViewController()
{

}

void ViewController::viewDidLoad()
{
    scroll = new CAScrollView();
    scroll->initWithFrame(this->getView()->getBounds());
    this->getView()->addSubview(scroll);
    scroll->setViewSize(scroll->getBounds().size);
    scroll->setMaximumZoomScale(3.0f);
    scroll->setMinimumZoomScale(0.5f);
    
    v = CAImageView::createWithImage(CAImage::create("bg.jpg"));
    v->setFrame(CCRect(0, 0, scroll->getViewSize().width, scroll->getViewSize().height));
    scroll->addSubview(v);
}

void ViewController::viewDidUnload()
{
    
}

void ViewController::touchUpInSide(CAControl* control, CCPoint point)
{
    CCSize size = scroll->getViewSize();
    size.height += 100;
    scroll->setViewSize(size);
    v->setFrame(CCRect(0, 0, size.width, size.height));
}
