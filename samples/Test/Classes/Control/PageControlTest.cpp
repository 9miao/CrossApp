//
//  PageControlTest.cpp
//  Test
//
//  Created by ly on 2017/3/2.
//
//

#include "PageControlTest.h"

PageControlTest::PageControlTest()
{
    this->setTitle("CAPageControl");
}

PageControlTest::~PageControlTest()
{
}

void PageControlTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
    
    CAPageControl* pageControl = CAPageControl::createWithLayout(DLayout(DHorizontalLayout_W_C(150, 0.5), DVerticalLayout_H_C(54, 0.25f)));
    pageControl->setNumberOfPages(5);
    pageControl->setStyle(CAPageControl::Style::Dot);
    pageControl->setPageIndicatorTintColor(CAColor4B::WHITE);
    pageControl->setCurrentPageIndicatorTintColor(CAColor4B::GREEN);
    pageControl->setTarget([=](int index)
    {
        CCLog("btn_tag===%d",pageControl->getCurrentPage());
    });
    this->getView()->addSubview(pageControl);
    
    CAPageControl* pageControl2 = CAPageControl::createWithLayout(DLayout(DHorizontalLayout_W_C(150, 0.5), DVerticalLayout_H_C(54, 0.4f)));
    pageControl2->setNumberOfPages(5);
    pageControl2->setStyle(CAPageControl::Style::Round);
    pageControl2->setPageIndicatorTintColor(CAColor4B::WHITE);
    pageControl2->setCurrentPageIndicatorTintColor(CAColor4B::GREEN);
    pageControl2->setTarget([=](int index)
    {
        CCLog("btn_tag===%d",pageControl2->getCurrentPage());
    });
    this->getView()->addSubview(pageControl2);
    
    CAPageControl* pageControl3 = CAPageControl::createWithLayout(DLayout(DHorizontalLayout_W_C(150, 0.5), DVerticalLayout_H_C(54, 0.55f)));
    pageControl3->setNumberOfPages(5);
    pageControl3->setStyle(CAPageControl::Style::Rectangle);
    pageControl3->setPageIndicatorTintColor(CAColor4B::WHITE);
    pageControl3->setCurrentPageIndicatorTintColor(CAColor4B::GREEN);
    pageControl3->setTarget([=](int index)
    {
        CCLog("btn_tag===%d",pageControl3->getCurrentPage());
    });
    this->getView()->addSubview(pageControl3);
}

void PageControlTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
