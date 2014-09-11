#ifndef _Label_Test_h_
#define _Label_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class LabelTest :public CAViewController
{
public:
	LabelTest();
	virtual ~LabelTest();
    
public:
	void numberChange(float interval);

protected:
	void viewDidLoad();
	void viewDidUnload();

private:
	CADipSize size;
	CALabel* number1;
	CALabel* number2;
	CALabel* number3;
	int sum;

};

#endif