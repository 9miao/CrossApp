//
//  CheckboxTest.cpp
//  Test
//
//  Created by ly≤     on 2017/3/1.
//
//

#include "CheckboxTest.h"

CheckboxTest::CheckboxTest()
{
    this->setTitle("CACheckbox");
}

CheckboxTest::~CheckboxTest()
{

}

void CheckboxTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
    
    CACheckbox* box = CACheckbox::createWithLayout(DLayout(DHorizontalLayout_W_C(54, 0.5), DVerticalLayout_H_C(54, 0.25f)), CACheckbox::Type::RoundedRect);
    this->getView()->addSubview(box);
    box->setTarget([](bool var)
    {
        CCLog("box: %d", var);
    });
    
    CACheckbox* box1 = CACheckbox::createWithLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.5f)), CACheckbox::Type::SquareRect);
    this->getView()->addSubview(box1);
    box1->setTarget([](bool var)
    {
        CCLog("box: %d", var);
    });
    
    CACheckbox* box2 = CACheckbox::createWithLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.75f)), CACheckbox::Type::RoundedRect);
    box2->setTitleStateNormal("未选中");
    box2->setTitleStateSelected("选中");
    this->getView()->addSubview(box2);
    box2->setTarget([](bool var)
                    {
                        CCLog("box: %d", var);
                    });
}

void CheckboxTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
