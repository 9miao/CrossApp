#ifndef _Switch_Test_h_
#define _Switch_Test_h_

#include <iostream>
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
	void switchStateChange(CAControl* btn, CCPoint point);

private:
	CCSize size;
	CALabel* switchState;

};

#endif