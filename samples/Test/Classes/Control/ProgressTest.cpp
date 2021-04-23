
#include "ProgressTest.h"

ProgressTest::ProgressTest()
{
    this->setTitle("CAProgress");
}

ProgressTest::~ProgressTest()
{
}

void ProgressTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
    
    auto label = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(300, 50)));
    label->setTag(100);
    label->setText("");
    label->setFontSize(30);
    label->setColor(CAColor4B(51, 204, 255, 255));
    label->setTextAlignment(CATextAlignment::Center);
    label->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    this->getView()->addSubview(label);
    
    CAProgress* progress = CAProgress::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(550, 0)));
    this->getView()->addSubview(progress);
    progress->setProgress(0.0f);
    
    CAScheduler::getScheduler()->schedule([=](float dt)
    {
        static float value = 0;
        value += 0.01f;
        if (value > 1.2f)
        {
            value = 0;
        }
        progress->setProgress(value);
        label->setText(crossapp_format_string("%f", MIN(value, 1.0f)));
    }, "progressValueChange", this, 0);
    
    
}

void ProgressTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}



