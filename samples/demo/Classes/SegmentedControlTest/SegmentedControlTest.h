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
	CADipSize size;
	CASegmentedControl* segment;
	CALabel* segmentText;
	CALabel* defaultPage;
	CALabel* label;
	CAView* defaultView;

};

#endif