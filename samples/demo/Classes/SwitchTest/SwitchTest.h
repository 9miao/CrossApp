#ifndef _Switch_Test_h_
#define _Switch_Test_h_

#include "CrossApp.h"

USING_NS_CC;

class SwitchTest :public CAViewController
{
public:
	SwitchTest();
	virtual ~SwitchTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void switchStateChange(CAControl* btn, const CCPoint& point);

private:
	CCSize size;
	CALabel* switchState;

};

#endif