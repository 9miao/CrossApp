#ifndef _Button_Test_h_
#define _Button_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

typedef enum BTNTAG
{
	BUTTONONE = 0,
	BUTTONTWO,
	BUTTONTHREE
};

class ButtonTest :public CAViewController
{
public:
	ButtonTest();
	virtual ~ButtonTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void threeButtonType();
	void buttonBackground();
	void buttonTouchEvent();

	void buttonCallback(CAControl* btn,CCPoint point);

private:
	CCSize size;
	CALabel* descTest;

};

#endif