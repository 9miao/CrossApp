#ifndef _ActivityIndicatorView_Test_h_
#define _ActivityIndicatorView_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ActivityIndicatorViewTest :public CAViewController
{
public:
	ActivityIndicatorViewTest();
	virtual ~ActivityIndicatorViewTest();
	
	void cancelCallback();
	void stopAllActivity(CAControl* btn,CCPoint point);

protected:
	void viewDidLoad();
	void viewDidUnload();

private:
	CADipSize size;
	CAActivityIndicatorView* whiteLarge;
	CAActivityIndicatorView* white;
	CAActivityIndicatorView* gray;
};

#endif