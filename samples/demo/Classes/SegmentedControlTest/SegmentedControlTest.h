#ifndef _SegmentedControl_Test_h_
#define _SegmentedControl_Test_h_

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
    void segmentCallback(CAControl* btn, const CCPoint& point);

private:
	CCSize size;
	CASegmentedControl* segment;
	CALabel* segmentText;
	CALabel* defaultPage;
	CALabel* label;
	CAView* defaultView;

};

#endif