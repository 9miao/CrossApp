#ifndef _ImageView_Test_h_
#define _ImageView_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ImageViewTest :public CAViewController
{
public:
	ImageViewTest();
	virtual ~ImageViewTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

private:
	CCSize size;

};

#endif