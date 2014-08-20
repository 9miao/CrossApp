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

protected:
	void viewDidLoad();
	void viewDidUnload();
	virtual void keyBackClicked();

private:
	CCSize size;
	CAActivityIndicatorView* whiteLarge;
	CAActivityIndicatorView* white;
	CAActivityIndicatorView* gray;

};

#endif