#ifndef _SegmentedControl_Test_h_
#define _SegmentedControl_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class SegmentedControlTest :public CAViewController
{
public:
	SegmentedControlTest();
	virtual ~SegmentedControlTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void segmentCallback(CAControl* btn, CCPoint point);

private:
	CCSize size;
	CASegmentedControl* segment;
	CALabel* segmentText;
	CALabel* label;
	CAView *firstPage;
	CAView *secondPage;
	CAView *thirdPage;

};

#endif