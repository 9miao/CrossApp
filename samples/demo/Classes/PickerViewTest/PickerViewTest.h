//
//  PickerViewTest.h
//  HelloCpp
//
//  Created by dai xinping on 14-7-24.
//
//

#ifndef __HelloCpp__PickerViewTest__
#define __HelloCpp__PickerViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class PickerViewTest :public CAViewController, public CAPickerViewDataSource, public CAPickerViewDelegate
{
public:
	PickerViewTest();
	virtual ~PickerViewTest();
    
protected:
	void viewDidLoad();
	void viewDidUnload();

protected:
    virtual void didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component);
    virtual unsigned int numberOfComponentsInPickerView(CAPickerView* pickerView);    
    virtual unsigned int numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component);
    virtual float widthForComponent(CAPickerView* pickerView, unsigned int component);
    virtual float rowHeightForComponent(CAPickerView* pickerView, unsigned int component);
    virtual CCString* titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component);
    
private:
    CAPickerView* pPickerView;
};

#endif /* defined(__HelloCpp__PickerViewTest__) */
