
#ifndef _Third_ViewController_h_
#define _Third_ViewController_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ThirdViewController :public CAViewController, public CAScrollViewDelegate
{
public:
	ThirdViewController();
	virtual ~ThirdViewController();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void stateControl(CAControl* btn, const CCPoint& point);

private:
	CCSize size;
	CAScrollView* scrollView;
	CASegmentedControl* scrollControl;

};

#endif
