
#ifndef __Test__PickerViewTest__
#define __Test__PickerViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

#define _T(x) L##x
#define CHAR    wchar_t

class PickerViewTest: public CAViewController, public CAPickerViewDelegate, public CAPickerViewDataSource, public CADatePickerViewDelegate, public CAScrollViewDelegate, public CAPageViewDelegate
{
    
public:

	PickerViewTest();
    
	virtual ~PickerViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAPickerView* p_pickerView;
    CALabel* city_value;
    CAClippingView* m_clvImage;
    CAClippingView* m_clv;
    
    int dle_ren_index;
    
public:
    
    //CAPickerView
    virtual unsigned int numberOfComponentsInPickerView(CAPickerView* pickerView);
    virtual unsigned int numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component);
    virtual float widthForComponent(CAPickerView* pickerView, unsigned int component);
    virtual float rowHeightForComponent(CAPickerView* pickerView, unsigned int component);
    virtual const char* titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component);
    
    virtual void didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component);
    virtual void didSelectRow(const struct tm& tm);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
