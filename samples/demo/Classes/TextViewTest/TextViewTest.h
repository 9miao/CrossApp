#ifndef _TextView_Test_h_
#define _TextView_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class TextViewTest :public CAViewController,CATextViewDelegate
{
public:
	TextViewTest();
	virtual ~TextViewTest();

	virtual bool onTextViewAttachWithIME(CATextView* sender);
	virtual bool onTextViewDetachWithIME(CATextView* sender);
	virtual bool onTextViewInsertText(CATextView* sender, const char * text, int nLen);
	//virtual bool onTextViewDeleteBackward(CATextView* sender, const char * delText, int nLen);
protected:
	void viewDidLoad();
	void viewDidUnload();

private:
	CADipSize size;

};

#endif