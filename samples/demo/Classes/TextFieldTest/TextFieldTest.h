#ifndef _TextField_Test_h_
#define _TextField_Test_h_

#include "CrossApp.h"

USING_NS_CC;

class TextFieldTest :public CAViewController
{
public:
	TextFieldTest();
	virtual ~TextFieldTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void defaultSliderValueChange(CAControl* btn, CCPoint point);

private:
	CCSize size;

};

#endif