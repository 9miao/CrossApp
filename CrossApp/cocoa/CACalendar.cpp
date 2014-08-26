//
//  CACalendar.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-5.
//  Copyright (c) 2014 cocos2d-x. All rights reserved.
//

#include "CACalendar.h"
#include <cstdlib>  
#include <iostream>  
#include <time.h>  
#include <stdio.h> 

using namespace std;
NS_CC_BEGIN

CACalendar::CACalendar()
{
    
}

CACalendar::~CACalendar()
{
    
}

CACalendar* CACalendar::create()
{
    CACalendar* cal = new CACalendar();
    if (cal) {
        cal->autorelease();
    }
    return cal;
}

CACalendar* CACalendar::create(int year, int month, int day)
{
    CACalendar* cal = CACalendar::create();
    if (cal) {
        cal->setCalendar(year, month, day);
    }
    return cal;        
}

CACalendar* CACalendar::create(time_t time)
{
    CACalendar* cal = CACalendar::create();
    if (cal) {
        cal->setCalendar(time);
    }
    return cal;    
}

void CACalendar::setCalendar(int year, int month, int day)
{
    struct tm tm = {0};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    
    time_t time = mktime(&tm);
    m_tDateTime = *localtime(&time);
}

void CACalendar::setCalendar(time_t time)
{
    m_tDateTime = *localtime(&time);
}

int CACalendar::_compareDate(const tm& date1, const tm& date2)
{
    if (date1.tm_year == date2.tm_year) {
        if (date1.tm_mon == date2.tm_mon) {
            if (date1.tm_mday == date2.tm_mday) {
                return 0;
            } else {
                return date1.tm_mday - date2.tm_mday;
            }
        } else {
            return date1.tm_mon - date2.tm_mon;
        }
    } else {
        return date1.tm_year - date2.tm_year;
    }
    return 0;
}

int CACalendar::_calcDayCount(const tm& target)
{
    int bInc = _compareDate(target, m_tDateTime);
    if (bInc == 0) {
        return 0;
    }
    int day = 0;
    int curr_month = bInc > 0 ? m_tDateTime.tm_mon : target.tm_mon;
    int curr_year = bInc > 0 ? m_tDateTime.tm_year : target.tm_year;
    int curr_date = bInc > 0 ? m_tDateTime.tm_mday : target.tm_mday;
    int dest_month = bInc < 0 ? m_tDateTime.tm_mon : target.tm_mon;
    int dest_year = bInc < 0 ? m_tDateTime.tm_year : target.tm_year;
    int dest_date = bInc < 0 ? m_tDateTime.tm_mday : target.tm_mday;

    while (curr_year != dest_year && curr_month != dest_month) {
        day += _dayCountOfMonth(curr_year, curr_month+1);
        curr_month++;
        if (curr_month == 12) {
            curr_month = 0;
            curr_year++;
        }
    }
    day += dest_date - curr_date;
    return day;
}

void CACalendar::addYear(int count)
{
//    struct tm tm = m_tDateTime;
//    tm.tm_year += count;
//    if (tm.tm_year < 1) {
//        tm.tm_year = 1;
//    } else if (tm.tm_year > 10000) {
//        tm.tm_year = 10000;
//    }
//    
//    if (tm.tm_mon == 1 && tm.tm_mday == 29) { // 2.29
//        if (!_isLeapYear(tm.tm_year)) {
//            tm.tm_mday = 28;
//        }
//    }
//    
//    int day = _calcDayCount(tm);
//    time_t time = mktime(&m_tDateTime) + day * 24 * 60 * 60;
//    m_tDateTime = *localtime(&time);
    m_tDateTime.tm_year += count;
    time_t time = mktime(&m_tDateTime);
    m_tDateTime = *localtime(&time);
}

void CACalendar::addMonth(int count)
{
//    int year = (m_tDateTime.tm_mon + count)/12;
//    int month = (m_tDateTime.tm_mon + count)%12;
//    if (month < 0) {
//        year--;
//        month += 12;
//    }
//    struct tm tm = m_tDateTime;
//    tm.tm_year += year;
//    tm.tm_mom = month;
//    
//    if (tm.tm_mon == 1 && tm.tm_mday > 28) { // 2.29
//        if (!_isLeapYear(tm.tm_year)) {
//            tm.tm_mday = 28;
//        } else {
//            tm.tm_mday = 29;
//        }
//    } else if (tm.tm_mday == 31) {
//        switch (tm.tm_mon) {
//            case 4:
//            case 6:
//            case 9:
//            case 11:
//                tm.tm_mday = 30;
//                break;
//            default:
//                break;
//        }
//    }
//    
//    int day = _calcDayCount(tm);

    m_tDateTime.tm_mon += count;
    time_t time = mktime(&m_tDateTime);
    m_tDateTime = *localtime(&time);

}

void CACalendar::addDay(int count)
{
    m_tDateTime.tm_mday += count;
    time_t time = mktime(&m_tDateTime);
    m_tDateTime = *localtime(&time);
}

bool CACalendar::_isLeapYear(int y)
{
    return ( (y%4==0 && y%100!=0)|| y%400==0 ) ? true : false;    
}

bool CACalendar::isLeapYear()
{
    return _isLeapYear(m_tDateTime.tm_year);
}

int CACalendar::_dayCountOfMonth(int year, int month)
{
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            return _isLeapYear(year) ? 29 : 28;
        default:
            return 0;
    }
}

int CACalendar::dayCountOfMonth()
{
    return _dayCountOfMonth(m_tDateTime.tm_year + 1900, m_tDateTime.tm_mon + 1);
}

int CACalendar::dayCountOfYear()
{
    return isLeapYear() ? 366 : 365;
}

int CACalendar::dayOfMonth()
{
    return m_tDateTime.tm_mday;
}

int CACalendar::dayOfYear()
{
    return m_tDateTime.tm_yday;
}

int CACalendar::dayOfWeek()
{
    return m_tDateTime.tm_wday;
}

int CACalendar::monthOfYear()
{
    return m_tDateTime.tm_mon + 1;
}

void CACalendar::dateByDayOfYear(int year, int day, int& month, int& date)
{
    month = 1;
    
    while (day > 0) {
        day -= _dayCountOfMonth(year, month++);
    }
    if (day == 0) {
        month--;
        date = _dayCountOfMonth(year, month);
    } else {
        date = _dayCountOfMonth(year, month) + day;
    }
}

NS_CC_END