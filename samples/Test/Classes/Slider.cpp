
#include "Slider.h"

SliderTestFirst::SliderTestFirst()
{

}

SliderTestFirst::~SliderTestFirst()
{
    
}

void SliderTestFirst::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);

    CAView* view = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view->setColor(CAColor_gray);
    
    sliderValue = CALabel::createWithLayout(DRectLayout(100,100,300,50,DRectLayout::L_R_T_H));
    sliderValue->setColor(ccc4(51, 204, 255, 255));
    sliderValue->setText("0");
    sliderValue->setFontSize(30);
    sliderValue->setTextAlignment(CATextAlignmentCenter);
    sliderValue->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    view->addSubview(sliderValue);
    
    CASlider* slider = CASlider::createWithLayout(DRectLayout(120,120,500,56,DRectLayout::L_R_T_H));
    slider->setTag(101);
    slider->setMaxTrackTintImage(CAImage::create("source_material/ex1.png"));
    slider->setMinTrackTintImage(CAImage::create("source_material/ex3.png"));
    slider->setThumbTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
    slider->addTarget(this, CAControl_selector(SliderTestFirst::sliderValueChange));
    view->addSubview(slider);
    
    this->getView()->addSubview(view);
    
}

void SliderTestFirst::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SliderTestFirst::sliderValueChange(CAControl* btn, DPoint point)
{
    char value[20] = "";
    CASlider* p_Slider = (CASlider*)btn;
    sprintf(value, "%.02f%%", p_Slider->getValue() * 100);
    if(p_Slider->getTag()==101)
    {
        sliderValue->setText(value);
    }
    
}
