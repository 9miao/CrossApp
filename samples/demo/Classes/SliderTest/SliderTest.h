#ifndef _Slider_Test_h_
#define _Slider_Test_h_

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
	void defaultSliderValueChange(CAControl* btn, const CCPoint& point);
	void sliderValueChange(CAControl* btn, const CCPoint& point);

private:
	CCSize size;
	CALabel* sliderValue;

};

#endif