#ifndef _Slider_Test_h_
#define _Slider_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class SliderTest :public CAViewController
{
public:
	SliderTest();
	virtual ~SliderTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void defaultSliderValueChange(CAControl* btn,CCPoint point);
	void sliderValueChange(CAControl* btn, CCPoint point);

private:
	CCSize size;
	CALabel* sliderValue;

};

#endif