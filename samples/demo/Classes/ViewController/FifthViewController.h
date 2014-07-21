#ifndef _Fifth_ViewController_h_
#define _Fifth_ViewController_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class FifthViewController :public CAViewController
{
public:
	FifthViewController();
	virtual ~FifthViewController();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void defaultSliderValueChange(CAControl* btn, CCPoint point);
	void hiddenViewController(CAControl* btn,CCPoint point);

private:
	CCSize size;

};

#endif