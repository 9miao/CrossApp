#ifndef _Progress_Test_h_
#define _Progress_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ProgressTest :public CAViewController
{
public:
	ProgressTest();
	virtual ~ProgressTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void firstProgressValueChange(float intevalTime);
	void secondProgressValueChange(float intevalTime);

private:
	CCSize size;
	CAProgress* firstProgress;
	CAProgress* secondProgress;
	CALabel* progressValue;

};

#endif