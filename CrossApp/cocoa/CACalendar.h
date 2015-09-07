//
//  CACalendar.h
//  CrossApp
//
//  Created by dai xinping on 14-8-5.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
//

#ifndef __CrossApp__CACalendar__
#define __CrossApp__CACalendar__

#include <iostream>
#include "basics/CAObject.h"

NS_CC_BEGIN

class CC_DLL CACalendar : public CAObject {
    
public:
    CACalendar();
    virtual ~CACalendar();
    
public:
    static CACalendar* create();
    static CACalendar* create(int year, int month, int day); // year: 1 ~ 10000, month: 1 ~ 12, day: 1 ~ 31
    static CACalendar* create(time_t time); // since 1900
    
    void setCalendar(int year, int month, int day);
    void setCalendar(time_t time);
    
    // count >= 1 && count <= 10000
    void addYear(int count = 1);
    void addMonth(int count = 1);
    void addDay(int count = 1);
    
    bool isLeapYear();
    
    int dayCountOfMonth();
    int dayCountOfYear();
    
    int dayOfMonth();
    int dayOfWeek();
    int dayOfYear();
    int monthOfYear();
    int weekOfYear();
    
    void dateByDayOfYear(int year, int day, int& month, int& date);
    
public:
    bool _isLeapYear(int year);
    int _dayCountOfMonth(int year, int month);
    int _calcDayCount(tm target);
    int _compareDate(tm date1, tm date2);
    int _dayCountOfYear(int year);
private:
    tm m_tDateTime;
};

NS_CC_END

#endif /* defined(__CrossApp__CACalendar__) */
