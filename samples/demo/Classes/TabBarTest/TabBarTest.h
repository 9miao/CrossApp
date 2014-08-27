#ifndef _TabBar_Test_h_
#define _TabBar_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class TabBarTest :public CAViewController,CATabBarDelegate
{
public:
	TabBarTest();
	virtual ~TabBarTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

	virtual void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index);

public:
	void goPrevious(CAControl* btn, CCPoint point);

	void firstTabBar(void);
	void secondTabBar(void);
	void thirdTabBar(void);

private:
	CADipSize size;
	CAButton* returnBtn;
};

#endif