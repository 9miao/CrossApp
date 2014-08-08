//
//  DatePickerTest.cpp
//  HelloCpp
//
//  Created by dai xinping on 14-8-5.
//
//

#include "DatePickerTest.h"

DatePickerTest::DatePickerTest()
{
    
}

DatePickerTest::~DatePickerTest()
{
    
}

void DatePickerTest::viewDidLoad()
{
    CADatePickerView* view = CADatePickerView::createWithCenter(CCRectMake(getView()->getFrame().size.width/2,
                                                                           getView()->getFrame().size.height/2, 
                                                                           getView()->getFrame().size.width/2, 
                                                                           getView()->getFrame().size.height/2));
    view->setDelegate(this);
//    view->setMode(CADatePickerModeDate);
    view->setMode(CADatePickerModeDateAndTime);
    
    getView()->addSubview(view);
}

void DatePickerTest::viewDidUnload()
{
    
}

void DatePickerTest::didSelectRow(const struct tm &tm)
{
    
}

