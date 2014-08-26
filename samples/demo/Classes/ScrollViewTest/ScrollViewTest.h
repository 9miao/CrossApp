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
    void zoomViewBySliderValue(CAControl* btn, const CCPoint& point);

private:
	CCSize size;
	CAScrollView* p_ScrollView;
	CAImageView* imageView;
	CASlider* zoomView;

};

#endif