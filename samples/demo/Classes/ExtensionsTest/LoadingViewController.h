#ifndef _Loading_ViewController_h_
#define _Loading_ViewController_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class LoadingViewController :public CAViewController
{
public:
	LoadingViewController();
	virtual ~LoadingViewController();

public:
	void addressBookLoadProgress(float interval);

protected:
	void viewDidLoad();
	void viewDidUnload();

private:
	CCSize size;
	CAProgress* loadProgress;

};

#endif