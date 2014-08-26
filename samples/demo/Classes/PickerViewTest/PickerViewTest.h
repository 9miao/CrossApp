#ifndef _PickerView_Test_h_
#define _PickerView_Test_h_

#include "CrossApp.h"

USING_NS_CC;

class PickerViewTest :public CAViewController, public CAPickerViewDelegate,public CAPickerViewDataSource
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
	CCSize size;
	std::string year;
	std::string month;
	std::string day;
	bool yearFlag;
	int daysOfMonth;
    CAPickerView* pickerView;

};

#endif
