#include "ScrollViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

ScrollViewTest::ScrollViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

ScrollViewTest::~ScrollViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void ScrollViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	
	p_ScrollView = CAScrollView::createWithCenter(CADipRect(size.width*0.5,size.height*0.5,size.width,size.height));
	p_ScrollView->setMinimumZoomScale(0.2f);
	p_ScrollView->setMaximumZoomScale(5.0f);
	this->getView()->addSubview(p_ScrollView);

	CADipSize _size = p_ScrollView->getBounds().size;
	imageView = CAImageView::createWithImage(CAImage::create("bg.jpg"));
	imageView->setCenter(CADipRect(_size.width*0.5, _size.height*0.5,_px(800),_px(1200)));
	p_ScrollView->addSubview(imageView);

	zoomView = CASlider::createWithCenter(CADipRect(size.width*0.5, size.height*0.9, size.width*0.8, 10));
	zoomView->addTarget(this,CAControl_selector(ScrollViewTest::zoomViewBySliderValue));
	//this->getView()->addSubview(zoomView);

	stepper = CAStepper::createWithCenter(CADipRect(size.width*0.5, size.height*0.9, 210, 80));
	stepper->setMinValue(-0.9);
	stepper->setMaxValue(1);
	stepper->setStepValue(0.1);
	stepper->setTouchEffect(true);
	stepper->addTarget(this, CAControl_selector(ScrollViewTest::stepperValueChange));
	this->getView()->addSubview(stepper);
	
}

void ScrollViewTest::zoomViewBySliderValue(CAControl* btn, CCPoint point)
{
	float multiple = zoomView->getValue()+1.0;
	imageView->setScale(multiple);
}

void ScrollViewTest::stepperValueChange(CAControl* btn, CCPoint point)
{
	imageView->setScale(stepper->getValue() + 1);
}

void ScrollViewTest::viewDidUnload()
{

}