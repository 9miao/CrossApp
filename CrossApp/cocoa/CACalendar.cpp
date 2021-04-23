//
//  CACalendar.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-5.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
//

#include "CACalendar.h"
#include <cstdlib>  
#include <iostream>  
#include <stdio.h> 

#include <iomanip>
#include <ctime>
#include <chrono>

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
    tm.tm_mon = month-1;
    tm.tm_mday = day;
    
	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	m_tDateTime = *std::localtime(&t);
}

void CACalendar::setCalendar(time_t time)
{
	m_tDateTime = *std::localtime(&time);
}

int CACalendar::_compareDate(tm date1, tm date2)
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

int CACalendar::_calcDayCount(tm target)
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
    m_tDateTime.tm_year += count;
    time_t t = std::mktime(&m_tDateTime);
	m_tDateTime = *std::localtime(&t);
}

void CACalendar::addMonth(int count)
{
    m_tDateTime.tm_mon += count;
	time_t t = std::mktime(&m_tDateTime);
	m_tDateTime = *std::localtime(&t);

}

void CACalendar::addDay(int count)
{
    m_tDateTime.tm_mday += count;
	time_t t = std::mktime(&m_tDateTime);
	m_tDateTime = *std::localtime(&t);
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

int CACalendar::_dayCountOfYear(int year)
{
    return _isLeapYear(year) ? 366 : 365;
}

int CACalendar::dayCountOfMonth()
{
    return _dayCountOfMonth(m_tDateTime.tm_year + 1900, m_tDateTime.tm_mon);
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
    return m_tDateTime.tm_mon+1;
}

void CACalendar::dateByDayOfYear(int year, int day, int& month, int& date)
{
    int num = _isLeapYear(year+1900)? 366 : 365;
    if (day<num) {
        month = 0;
        while (day > 0) {
            day -= _dayCountOfMonth(year, month++);
        }
        
        if (day == 0) {
            date = 1;
        } else if(day < 0){
            month--;
            date = _dayCountOfMonth(year, month) + day + 1;
        }
    }else{
        month = 0;
        date = 1;
    }
}

NS_CC_END