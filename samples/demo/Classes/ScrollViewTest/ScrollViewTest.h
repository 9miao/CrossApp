#ifndef _ScrollView_Test_h_
#define _ScrollView_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ScrollViewTest :public CAViewController, public CAScrollViewDelegate
{
public:
	ScrollViewTest();
	virtual ~ScrollViewTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void zoomViewBySliderValue(CAControl* btn,CCPoint point);
	void stepperValueChange(CAControl* btn, CCPoint point);

private:
	CADipSize size;
	CAScrollView* p_ScrollView;
	CAImageView* imageView;
	CASlider* zoomView;
	CAStepper* stepper;

};

#endif