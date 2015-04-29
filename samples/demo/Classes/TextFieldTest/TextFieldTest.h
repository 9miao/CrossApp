#ifndef _TextField_Test_h_
#define _TextField_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class TextFieldTest :public CAViewController, public CATextFieldDelegate
{
public:
	TextFieldTest();
	virtual ~TextFieldTest();

protected:
	void viewDidLoad();
	void viewDidUnload();
    
    virtual bool getKeyBoardHeight(int height);
    virtual bool onTextFieldAttachWithIME(CATextField * sender);
    virtual bool onTextFieldDetachWithIME(CATextField * sender);
    
    //animation
    void willStartAction(const string& animationID, void* context);
    void didStopAction(const string& animationID, void* context);
    
public:
	void defaultSliderValueChange(CAControl* btn, CCPoint point);

private:
	CADipSize size;
    CAView *m_pTextField;

};

#endif