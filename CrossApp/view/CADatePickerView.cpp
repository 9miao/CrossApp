//
//  CADatePickerView.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-4.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CADatePickerView.h"
#include "cocoa/CACalendar.h"
#include "basics/CAApplication.h"
#include <sstream>



NS_CC_BEGIN

#pragma execution_character_set("utf-8")

CADatePickerView::CADatePickerView()
: m_pPickerView(NULL)
, m_eMode(CADatePickerModeTime)
, m_pDelegate(NULL)
{
    
}

CADatePickerView::~CADatePickerView()
{
    CC_SAFE_RELEASE(m_pPickerView);
}

CADatePickerView* CADatePickerView::create()
{
    CADatePickerView* view = new CADatePickerView();
    if (view && view->init()) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CADatePickerView* CADatePickerView::createWithFrame(const CCRect& rect)
{
    CADatePickerView* view = new CADatePickerView();
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CADatePickerView* CADatePickerView::createWithCenter(const CCRect& rect)
{
    CADatePickerView* view = new CADatePickerView();
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
    
    m_pPickerView = CAPickerView::create();
    m_pPickerView->retain();
    m_pPickerView->setDataSource(this);
    m_pPickerView->setDelegate(this);
    m_pPickerView->setFontSizeNormal(40 * CAApplication::getApplication()->getAdaptationRatio());
    m_pPickerView->setFontSizeSelected(40 * CAApplication::getApplication()->getAdaptationRatio());
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
    if (CAControl::initWithFrame(rect) && m_pPickerView) {
        CCRect frame = CCRectMake(0, 0, rect.size.width, rect.size.height);
        m_pPickerView->initWithFrame(frame);
        
        return true;
    }
    
    return false;
}

bool CADatePickerView::initWithCenter(const CCRect& rect)
{
    if (CAControl::initWithCenter(rect) && m_pPickerView) {
        CCRect center = CCRectMake(rect.size.width/2, rect.size.height/2, rect.size.width, rect.size.height);
        m_pPickerView->initWithCenter(center);
        
        return true;
    }
    
    return false;
}

void CADatePickerView::setDate(int year, int month, int day, bool animated)
{
    m_tTM.tm_year = year;
    m_tTM.tm_mon = month - 1;
    m_tTM.tm_mday = day;
    
    // fault tolerance
    time_t t = mktime(&m_tTM);
    m_tTM = *localtime(&t);
    
    if (m_pPickerView) {
        switch (m_eMode) {
            case CADatePickerModeDate: {
                m_pPickerView->selectRow(m_tTM.tm_year, 0, animated);
                m_pPickerView->selectRow(m_tTM.tm_mon, 1, animated);
                m_pPickerView->selectRow(m_tTM.tm_mday - 1, 2, animated);
                break;
            }
                
            case CADatePickerModeDateAndTime:
                m_pPickerView->selectRow(CACalendar::create(mktime(&m_tTM))->dayOfYear() - 1, 0, animated);
                break;
                                
            default:
                break;
        }
    }
}

unsigned int CADatePickerView::numberOfComponentsInPickerView(CAPickerView* pickerView)
{
    int components = 0;
    switch (m_eMode) {
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
    switch (m_eMode) {
        case CADatePickerModeCountDownTimer:
            if (component == 0) { // hour
                row = 24; // 0 ~ 23
            } else { // min
                row = 60; // 0 ~ 59
            }
            break;
            
        case CADatePickerModeDate:
            if (component == 0) { // year
                row = 2030 - 1900 + 1; // 1900~2030
            } else if (component == 1) { // month
                row = 12;
            } else { // day
                row = CACalendar::create(mktime(&m_tTM))->dayCountOfMonth();
            }
            break;
            
        case CADatePickerModeDateAndTime:
            if (component == 0) { // date
                row = CACalendar::create(mktime(&m_tTM))->isLeapYear() ? 366 : 365;
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
    switch (m_eMode) {
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
    switch (m_eMode) {
        case CADatePickerModeCountDownTimer:
            if (component == 0) { // hour
              //  sprintf(buff, "%d小时", row);
            } else { // min
              //  sprintf(buff, "%d分钟", row);
            }
            break;
            
        case CADatePickerModeDate:
            if (component == 0) { // years
              //  sprintf(buff, "%d年", row + 1900);
            } else if (component == 1) { // month
              //  sprintf(buff, "%d月", row + 1);
            } else { // day
               // sprintf(buff, "%d日", row + 1);
            }
            break;
            
        case CADatePickerModeDateAndTime:
            if (component == 0) { // date
                CACalendar* cal = CACalendar::create(mktime(&m_tTM));
                int day = row + 1;
                int dayOfYear = cal->dayOfYear();
                cal->addDay(day - dayOfYear);
                int week = cal->dayOfWeek();
                int month = cal->monthOfYear();
                int date = cal->dayOfMonth();
                // const char* week_s[] = {"日","一","二","三","四","五","六"};
				const char* week_s[] = {"\u65e5", "\u4e00", "\u4e8c", "\u4e09", "\u56db", "\u4e94", "\u516d"};
                
                sprintf(buff, "%d月%d日  周%s", month, date, week_s[week]);
            } else if (component == 1) { // hour
                sprintf(buff, "%02d", row);
            } else { // minutes
                sprintf(buff, "%02d", row);
            }
            break;
            
        case CADatePickerModeTime:
            if (component == 0) { // hour
              //  sprintf(buff, "%02d", row);
            } else { // minutes
              //  sprintf(buff, "%02d", row);
            }
            break;
            
        default:
            break;
    }
    return CCString::create(std::string(buff));
}

void CADatePickerView::didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    switch (m_eMode) {
        case CADatePickerModeCountDownTimer:
            if (component == 0) { // hour
                m_tTM.tm_hour = row;
            } else { // min
                m_tTM.tm_min = row;
            }
            break;
            
        case CADatePickerModeDate: {
            if (component == 0) { // years
                m_tTM.tm_year = row;
                m_tTM.tm_mon = 0;
                m_tTM.tm_mday = 1;
                m_pPickerView->reloadComponent(1);
                m_pPickerView->reloadComponent(2);
            } else if (component == 1) { // month
                m_tTM.tm_mon = row;
                m_tTM.tm_mday = 1;
                m_pPickerView->reloadComponent(2);
            } else { // day
                m_tTM.tm_mday = row + 1;
            }
            // fault tolerant
            time_t t = mktime(&m_tTM);
            m_tTM = *localtime(&t);
            break;
        }
            
        case CADatePickerModeDateAndTime:
            if (component == 0) { // date 
                CACalendar* cal = CACalendar::create();
                int month, date;
                cal->dateByDayOfYear(m_tTM.tm_year, row + 1, month, date);
                m_tTM.tm_mon = month;
                m_tTM.tm_mday = date;
                time_t t = mktime(&m_tTM);
                m_tTM = *localtime(&t);
            } else if (component == 1) { // hour
                m_tTM.tm_hour = row;
            } else { // minutes
                m_tTM.tm_min = row;
            }
            break;
            
        case CADatePickerModeTime:
            if (component == 0) { // hour
                m_tTM.tm_hour = row;
            } else { // minutes
                m_tTM.tm_min = row;
            }
            break;
            
        default:
            break;
    }
    
    if (m_pDelegate) {
        m_pDelegate->didSelectRow(m_tTM);
    }
}

void CADatePickerView::setMode(CADatePickerMode mode)
{
    m_eMode = mode;
    if (m_pPickerView) {
        m_pPickerView->reloadAllComponents();
        switch (m_eMode) {
            case CADatePickerModeDate:
                m_pPickerView->selectRow(m_tTM.tm_year, 0);
                m_pPickerView->selectRow(m_tTM.tm_mon, 1);
                m_pPickerView->selectRow(m_tTM.tm_mday - 1, 2);
                break;
            case CADatePickerModeDateAndTime:
                m_pPickerView->selectRow(CACalendar::create(mktime(&m_tTM))->dayOfYear() - 1, 0);
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