#include "SliderTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

SliderTest::SliderTest()
{

}

SliderTest::~SliderTest()
{

}

void SliderTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	
	CASlider* defaultSlider = CASlider::createWithCenter(CADipRect(size.width*0.5,size.height*0.2,size.width*0.7,20));
	defaultSlider->addTarget(this, CAControl_selector(SliderTest::defaultSliderValueChange));
	this->getView()->addSubview(defaultSlider);

	CASlider* slider = CASlider::createWithCenter(CADipRect(size.width*0.5, size.height*0.4, size.width*0.7, 20));
	slider->setTrackHeight(20);
	slider->setMaxTrackTintImage(CAImage::create("source_material/ex2.png"));
	slider->setMinTrackTintImage(CAImage::create("source_material/ex3.png"));
	slider->setThumbTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
	slider->addTarget(this, CAControl_selector(SliderTest::sliderValueChange));
	this->getView()->addSubview(slider);

	sliderValue = CALabel::createWithCenter(CADipRect(size.width*0.5, size.height*0.6, size.width*0.3, 50));
	sliderValue->setColor(CAColor_blueStyle);
	sliderValue->setText("");
	sliderValue->setFontSize(_px(30));
	sliderValue->setTextAlignment(CATextAlignmentCenter);
	sliderValue->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(sliderValue);
}

void SliderTest::defaultSliderValueChange(CAControl* btn, CCPoint point)
{
	char value[20] = "";
	CASlider* p_Slider = (CASlider*)btn;
	sprintf(value, "%.02f%%", p_Slider->getValue() * 100);
	sliderValue->setText(value);
}

void SliderTest::sliderValueChange(CAControl* btn, CCPoint point)
{
	char value[20] = "";
	CASlider* p_Slider = (CASlider*)btn;
	sprintf(value, "%.02f%%", p_Slider->getValue() * 100);
	sliderValue->setText(value);
}

void SliderTest::viewDidUnload()
{

}