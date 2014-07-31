#ifndef _AlertView_Test_h_
#define _AlertView_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class AlertViewTest :public CAViewController
{
public:
	AlertViewTest();
	virtual ~AlertViewTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void respondTouch(CAControl* btn, CCPoint point);
	void alertViewCallback(int btnIndex);

private:
	CCSize size;
	CAButton* imageBtn;

};

#endif