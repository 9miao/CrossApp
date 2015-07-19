//
//  CADatePickerView.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-4.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
//

#include "CADatePickerView.h"
#include "cocoa/CACalendar.h"
#include "basics/CAApplication.h"
#include <sstream>



NS_CC_BEGIN

#pragma execution_character_set("utf-8")

CADatePickerView::CADatePickerView(const CADatePickerMode& m_mode)
: m_pPickerView(NULL)
, m_eMode(m_mode)
, m_pDelegate(NULL)
,isSetDate(false)
{

}

CADatePickerView::~CADatePickerView()
{
    CC_SAFE_RELEASE(m_pPickerView);
}

CADatePickerView* CADatePickerView::create(const CADatePickerMode& m_mode)
{
    CADatePickerView* view = new CADatePickerView(m_mode);
    if (view && view->init()) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CADatePickerView* CADatePickerView::createWithFrame(const CCRect& rect,const CADatePickerMode& m_mode)
{
    CADatePickerView* view = new CADatePickerView(m_mode);
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CADatePickerView* CADatePickerView::createWithCenter(const CCRect& rect,const CADatePickerMode& m_mode)
{
    CADatePickerView* view = new CADatePickerView(m_mode);
    if (view && view->initWithCenter(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}


bool CADatePickerView::init()
{
    if (!CAControl::init()) {
        return false;
    }
    m_pPickerView = new CAPickerView();
    m_pPickerView->initWithFrame(this->getBounds());
    m_pPickerView->setPickerViewDelegate(this);
    m_pPickerView->setPickerViewDataSource(this);
    m_pPickerView->setFontSizeNormal(_px(40));
    m_pPickerView->setFontSizeSelected(_px(40));
    addSubview(m_pPickerView);
    
    struct timeval tp = {0};
    gettimeofday(&tp,  NULL);
    time_t time = tp.tv_sec;
    m_tTM = *localtime(&time);
    return true;
}

void CADatePickerView::onEnter()
{
    CAControl::onEnter();
    if (!isSetDate) {
        setMode(m_eMode);
    }
    
}

void CADatePickerView::onExit()
{
    CAControl::onExit();
}

void CADatePickerView::visit()
{
    CAControl::visit();
}


bool CADatePickerView::initWithFrame(const CCRect& rect)
{
    if (CAControl::initWithFrame(rect) && m_pPickerView)
    {
        m_pPickerView->initWithFrame(this->getBounds());
        return true;
    }
    
    return false;
}

bool CADatePickerView::initWithCenter(const CCRect& rect)
{
    if (CAControl::initWithCenter(rect) && m_pPickerView)
    {
        m_pPickerView->initWithFrame(this->getBounds());
        return true;
    }
    
    return false;
}

void CADatePickerView::setDate(int year, int month, int day, bool animated)
{
    isSetDate = true;
    if (year>=1900 && year<=2030) {
        m_tTM.tm_year = year-1900;
    }else{
        isSetDate = false;
        return;
    }
    if (month>=1 && month<=12) {
        m_tTM.tm_mon = month;
    }else{
        isSetDate = false;
        return;
    }
    if (day>=1 && day<=31) {
        m_tTM.tm_mday = day;
    }else{
        isSetDate = false;
        return;
    }
    
    int tem_day = CACalendar::create()->_dayCountOfMonth(year,month);
    while (m_tTM.tm_mday>tem_day) {
        m_tTM.tm_mday--;
    }

    if (m_pPickerView)
    {
        m_pPickerView->reloadAllComponents();
        switch (m_eMode)
        {
            case CADatePickerModeDate:
            {
                m_pPickerView->selectRow(m_tTM.tm_year, 0, animated);
                m_pPickerView->selectRow(m_tTM.tm_mon-1, 1, animated);
                m_pPickerView->selectRow(m_tTM.tm_mday-1, 2, animated);
                break;
            }
            case CADatePickerModeDateAndTime:
                m_tTM.tm_mon--;
                m_pPickerView->selectRow(CACalendar::create(mktime(&m_tTM))->dayOfYear(), 0, animated);
                m_pPickerView->selectRow(m_tTM.tm_hour, 1);
                m_pPickerView->selectRow(m_tTM.tm_min, 2);
                break;
            default:
                break;
        }
    }
}

unsigned int CADatePickerView::numberOfComponentsInPickerView(CAPickerView* pickerView)
{
    int components = 0;
    switch (m_eMode)
    {
        case CADatePickerModeCountDownTimer:
            components = 2;
            break;
            
        case CADatePickerModeDate:
            components = 3;
            break;
            
        case CADatePickerModeDateAndTime:
            components = 3;
            break;
            
        case CADatePickerModeTime:
            components = 2;
            break;
            
        default:
            break;
    }
    
    return components;
}

unsigned int CADatePickerView::numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component)
{
    int row = 0;
    switch (m_eMode)
    {
        case CADatePickerModeCountDownTimer:
            if (component == 0) { // hour
                row = 24; // 0 ~ 23
            } else { // min
                row = 60; // 0 ~ 59
            }
            break;
            
        case CADatePickerModeDate:
            if (component == 0) { // year
                row = 2030 - 1900+1; // 1900~2030
            } else if (component == 1) { // month
                row = 12;
            } else { // day
                row = CACalendar::create()->_dayCountOfMonth(m_tTM.tm_year+1900,m_tTM.tm_mon);
            }
            break;
            
        case CADatePickerModeDateAndTime:
            if (component == 0) { // date
                row = CACalendar::create()->_isLeapYear(m_tTM.tm_year+1900) ? 366 : 365;
            } else if (component == 1) { // hour
                row = 24;
            } else { // minutes
                row = 60;
            }
            break;
            
        case CADatePickerModeTime:
            if (component == 0) {
                row = 24;
            } else {
                row = 60;
            }
            break;
            
        default:
            break;
    }
    return row;
}

float CADatePickerView::widthForComponent(CAPickerView* pickerView, unsigned int component)
{
    float width = m_pPickerView->getFrame().size.width/numberOfComponentsInPickerView(pickerView);
    switch (m_eMode)
    {
        case CADatePickerModeCountDownTimer:
            if (component == 0) { // hour                
            } else { // min
            }
            break;
            
        case CADatePickerModeDate:
            if (component == 0) { // years
            } else if (component == 1) { // month
            } else { // day
            }
            break;
            
        case CADatePickerModeDateAndTime:
            width = m_pPickerView->getFrame().size.width/4;
            if (component == 0) { // date
                width *= 2;
            } else if (component == 1) { // hour
            } else { // minutes
            }
            break;
            
        case CADatePickerModeTime:
            if (component == 0) { // hour
            } else { // min
            }
            break;
            
        default:
            break;
    }
    return width;
}

float CADatePickerView::rowHeightForComponent(CAPickerView* pickerView, unsigned int component)
{
    static float rowHeight = 80 * CAApplication::getApplication()->getAdaptationRatio();
    return rowHeight;
}

CCString* CADatePickerView::titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    char buff[256] = {0};
    switch (m_eMode)
    {
        case CADatePickerModeCountDownTimer:
            if (component == 0)
            { // hour
                sprintf(buff, "%d\u5c0f\u65f6", row);
            } else { // min
                sprintf(buff, "%d\u5206\u949f", row);
            }
            break;
            
        case CADatePickerModeDate:
            if (component == 0)
            { // years
                sprintf(buff, "%d\u5e74", row + 1900);
            } else if (component == 1) { // month
                sprintf(buff, "%d\u6708", row + 1);
            } else { // day
                sprintf(buff, "%d\u65e5", row + 1);
            }
            break;
            
        case CADatePickerModeDateAndTime:
            if (component == 0)
            { // date
                CACalendar* cal = CACalendar::create(m_tTM.tm_year+1900,m_tTM.tm_mon,m_tTM.tm_mday);
                int day = row;
                int dayOfYear = cal->dayOfYear();
                cal->addDay(day - dayOfYear);
                int week = cal->dayOfWeek();
                int month = cal->monthOfYear();
                int date = cal->dayOfMonth();
                // const char* week_s[] = {"日","一","二","三","四","五","六"};
				const char* week_s[] = {"\u65e5", "\u4e00", "\u4e8c", "\u4e09", "\u56db", "\u4e94", "\u516d"};
                sprintf(buff, "%d\u6708%d\u65e5  \u5468%s", month, date, week_s[week]);
            }
            else if (component == 1)
            { // hour
                sprintf(buff, "%02d", row);
            }
            else
            { // minutes
                sprintf(buff, "%02d", row);
            }
            break;
            
        case CADatePickerModeTime:
            if (component == 0)
            { // hour
                sprintf(buff, "%02d", row);
            }
            else
            { // minutes
                sprintf(buff, "%02d", row);
            }
            break;
            
        default:
            break;
    }
    return CCString::create(std::string(buff));
}

void CADatePickerView::didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    switch (m_eMode)
    {
        case CADatePickerModeCountDownTimer:
            if (component == 0)
            { // hour
                m_tTM.tm_hour = row;
            }
            else
            { // min
                m_tTM.tm_min = row;
            }
            break;
            
        case CADatePickerModeDate:
        {
            if (component == 0)
            { // years
                m_tTM.tm_year = row;
                int tem_day = CACalendar::create()->_dayCountOfMonth(m_tTM.tm_year+1900,m_tTM.tm_mon);
                if (m_tTM.tm_mday>tem_day) {
                    m_tTM.tm_mday = 1;
                    m_pPickerView->reloadComponent(0,2);
                }else{
                    m_pPickerView->reloadComponent(m_tTM.tm_mday-1,2);
                }
            }
            else if (component == 1)
            {
                m_tTM.tm_mon = row+1;
                int tem_day = CACalendar::create()->_dayCountOfMonth(m_tTM.tm_year+1900,m_tTM.tm_mon);
                if (m_tTM.tm_mday>tem_day) {
                    m_tTM.tm_mday = 1;
                    m_pPickerView->reloadComponent(0,2);
                }else{
                    m_pPickerView->reloadComponent(m_tTM.tm_mday-1,2);
                    
                }
            }
            else
            {
                m_tTM.tm_mday = row+1;
            }
            break;
        }
        case CADatePickerModeDateAndTime:
            if (component == 0)
            { // date
                CACalendar* cal = CACalendar::create();
                int month, date;
                cal->dateByDayOfYear(m_tTM.tm_year, row, month, date);
                m_tTM.tm_mon = month;
                m_tTM.tm_mday = date;
            }
            else if (component == 1)
            { // hour
                m_tTM.tm_hour = row;
            }
            else
            { // minutes
                m_tTM.tm_min = row;
            }
            break;
            
        case CADatePickerModeTime:
            if (component == 0)
            { // hour
                m_tTM.tm_hour = row;
            }
            else
            { // minutes
                m_tTM.tm_min = row;
            }
            break;
            
        default:
            break;
    }
    
    if (m_pDelegate)
    {
        m_pDelegate->didSelectRow(m_tTM);
    }
}

void CADatePickerView::setMode(CADatePickerMode mode)
{
    m_eMode = mode;
    CCLog("setMode m_tTM.tm_mon==%d",m_tTM.tm_mon);
    if (m_pPickerView) {
        m_pPickerView->reloadAllComponents();
        switch (m_eMode)
        {
            case CADatePickerModeDate:
                m_pPickerView->selectRow(m_tTM.tm_year, 0);
                m_pPickerView->selectRow(m_tTM.tm_mon, 1);
                m_pPickerView->selectRow(m_tTM.tm_mday-1, 2);
                break;
            case CADatePickerModeDateAndTime:
                m_pPickerView->selectRow(CACalendar::create(mktime(&m_tTM))->dayOfYear(), 0);
                m_pPickerView->selectRow(m_tTM.tm_hour, 1);
                m_pPickerView->selectRow(m_tTM.tm_min, 2);
                break;
            case CADatePickerModeTime:
                m_pPickerView->selectRow(m_tTM.tm_hour, 0);
                m_pPickerView->selectRow(m_tTM.tm_min, 1);
                break;
            default:
                break;
        }
    }
}


NS_CC_END