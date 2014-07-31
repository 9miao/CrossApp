#ifndef _NavigationBar_Test_h_
#define _NavigationBar_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class NavigationBarTest :public CAViewController
{
public:
	NavigationBarTest();
	virtual ~NavigationBarTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void nextLayer(CAControl* btn, CCPoint point);
	void returnMainMenu(CAControl* btn, CCPoint point);

private:
	CCSize size;
	CANavigationBar* p_NavigationBar;

};

#endif