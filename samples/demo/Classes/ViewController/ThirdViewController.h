
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
	virtual void viewDidAppear();

public:
	void stateControl(CASegmentedControl* btn, int index);

private:
	CADipSize size;
	CAScrollView* scrollView;
	CASegmentedControl* scrollControl;

};

#endif
