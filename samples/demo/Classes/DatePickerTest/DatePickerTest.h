//
//  DatePickerTest.h
//  HelloCpp
//
//  Created by dai xinping on 14-8-5.
//
//

#ifndef __HelloCpp__DatePickerTest__
#define __HelloCpp__DatePickerTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class DatePickerTest : public CAViewController, public CADatePickerViewDelegate {
    
public:
    DatePickerTest();
    virtual ~DatePickerTest();
    
protected:
	void viewDidLoad();
	void viewDidUnload();
    
    virtual void didSelectRow(const struct tm& tm);
};


#endif /* defined(__HelloCpp__DatePickerTest__) */
