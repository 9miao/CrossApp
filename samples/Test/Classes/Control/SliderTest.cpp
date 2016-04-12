
#include "SliderTest.h"
#include "CDUIShowAutoCollectionView.h"

SliderTest::SliderTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

SliderTest::~SliderTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

int SliderNum = 0;

void CDUIShowAutoCollectionView::SliderRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showSliderNavigationBar >= 1)
    {
        showSliderNavigationBar = 0;
        SliderNum = showSliderNavigationBar;
    }
    else
    {
        SliderNum = ++showSliderNavigationBar;
    }
    SliderTest* ViewContrllerSliderTest = new SliderTest();
    ViewContrllerSliderTest->init();
    ViewContrllerSliderTest->setNavigationBarItem(SliderNavigationBar);
    ViewContrllerSliderTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerSliderTest, false);
}

void SliderTest::viewDidLoad()
{
    if (SliderNum == 0)
    {
        CAView* view1 = CAView::createWithLayout(DLayoutFill);
        view1->setColor(CAColor_gray);
    
//        sliderValue1 = CALabel::createWithLayout(DRectLayout(100,100,300,50,DRectLayout::L_R_T_H));
        sliderValue1 = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(300, 50)));
        sliderValue1->setColor(ccc4(51,204,255,255));
        sliderValue1->setText("0");
        sliderValue1->setFontSize(30);
        sliderValue1->setTextAlignment(CATextAlignmentCenter);
        sliderValue1->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        view1->addSubview(sliderValue1);
    
//        CASlider* slider1 = CASlider::createWithLayout(DRectLayout(120,120,500,56,DRectLayout::L_R_T_H));
        CASlider* slider1 = CASlider::createWithLayout(DLayout(DHorizontalLayout_L_R(120, 120), DVerticalLayout_T_H(500, 56)));
        slider1->addTarget(this, CAControl_selector(SliderTest::sliderValueChange));
        slider1->setTag(100);
        view1->addSubview(slider1);
        this->getView()->addSubview(view1);
    }
    else
    {
        CAView* view2 = CAView::createWithLayout(DLayoutFill);
        view2->setColor(CAColor_gray);
    
//        sliderValue2 = CALabel::createWithLayout(DRectLayout(100,100,300,50,DRectLayout::L_R_T_H));
        sliderValue2 = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(300, 50)));
        sliderValue2->setColor(ccc4(51, 204, 255, 255));
        sliderValue2->setText("0");
        sliderValue2->setFontSize(30);
        sliderValue2->setTextAlignment(CATextAlignmentCenter);
        sliderValue2->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        view2->addSubview(sliderValue2);
    
//        CASlider* slider2 = CASlider::createWithLayout(DRectLayout(120,120,500,56,DRectLayout::L_R_T_H));
        CASlider* slider2 = CASlider::createWithLayout(DLayout(DHorizontalLayout_L_R(120, 120), DVerticalLayout_T_H(500, 56)));
        slider2->setTag(101);
        slider2->setMaxTrackTintImage(CAImage::create("source_material/ex1.png"));
        slider2->setMinTrackTintImage(CAImage::create("source_material/ex3.png"));
        slider2->setThumbTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
        slider2->addTarget(this, CAControl_selector(SliderTest::sliderValueChange));
        view2->addSubview(slider2);
    
        this->getView()->addSubview(view2);
    }
}

void SliderTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SliderTest::sliderValueChange(CAControl* btn, DPoint point)
{
    char value[20] = "";
    CASlider* p_Slider = (CASlider*)btn;
    sprintf(value, "%.02f%%", p_Slider->getValue() * 100);
    if (p_Slider->getTag()==100) {
        sliderValue1->setText(value);
    }else if(p_Slider->getTag()==101)
    {
        sliderValue2->setText(value);
    }
    
}
