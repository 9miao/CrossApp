
#include "SliderTest.h"

SliderTest::SliderTest()
{
    this->setTitle("CASlider");
}

SliderTest::~SliderTest()
{

}

void SliderTest::viewDidLoad()
{

    auto label = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(300, 50)));
    label->setColor(ccc4(51,204,255,255));
    label->setText("0");
    label->setFontSize(30);
    label->setTextAlignment(CATextAlignment::Center);
    label->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    this->getView()->addSubview(label);
    
    CASlider* slider1 = CASlider::createWithLayout(DLayout(DHorizontalLayout_L_R(120, 120), DVerticalLayout_H_C(56, 0.4)));
    slider1->setTarget([=](float v) {
        char value[20];
        sprintf(value, "%.02f%%", v * 100);
        label->setText(value);
    });
    this->getView()->addSubview(slider1);
    
    
    CASlider* slider2 = CASlider::createWithLayout(DLayout(DHorizontalLayout_L_R(120, 120), DVerticalLayout_H_C(56, 0.75)));
    slider2->setTag(101);
    slider2->setTrackHeight(6);
    slider2->setMaxTrackTintImage(CAImage::create("image/slider_max_track.png"));
    slider2->setMinTrackTintImage(CAImage::create("image/slider_min_track.png"));
    slider2->setThumbTintImage(CAImage::create("image/slider_indicator.png"));
    slider2->setTarget([=](float v) {
        char value[20] = "";
        sprintf(value, "%.02f%%", v * 100);
        label->setText(value);
    });
    this->getView()->addSubview(slider2);
}

void SliderTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
