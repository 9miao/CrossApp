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
#include "view/CAScale9ImageView.h"
#include "support/ccUTF8.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

NS_CC_BEGIN

#pragma execution_character_set("utf-8")

CADatePickerView::CADatePickerView(CADatePickerView::Mode m_mode)
: m_pPickerView(nullptr)
, m_eMode(m_mode)
, isSetDate(false)
, m_obBackgroundImageForSelected(nullptr)
{
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto tm = std::localtime(&t);
    m_tTM.tm_sec = tm->tm_sec;
    m_tTM.tm_min = tm->tm_min;
    m_tTM.tm_hour = tm->tm_hour;
    m_tTM.tm_mday = tm->tm_mday;
    m_tTM.tm_mon = tm->tm_mon;
    m_tTM.tm_year = tm->tm_year;
    m_tTM.tm_wday = tm->tm_wday;
    m_tTM.tm_yday = tm->tm_yday;
    m_tTM.tm_isdst = tm->tm_isdst;
    
    CAScheduler::getScheduler()->scheduleOnce([=](float dt){
        if (!isSetDate) this->setMode(m_eMode);
    }, "CADatePickerView::update", this, 0.1);
}

CADatePickerView::~CADatePickerView()
{
    CC_SAFE_RELEASE(m_pPickerView);
    CC_SAFE_RELEASE(m_obBackgroundImageForSelected);
}

CADatePickerView* CADatePickerView::create(CADatePickerView::Mode m_mode)
{
    CADatePickerView* view = new CADatePickerView(m_mode);
    if (view && view->init()) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CADatePickerView* CADatePickerView::createWithFrame(const DRect& rect,CADatePickerView::Mode m_mode)
{
    CADatePickerView* view = new CADatePickerView(m_mode);
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CADatePickerView* CADatePickerView::createWithCenter(const DRect& rect,CADatePickerView::Mode m_mode)
{
    CADatePickerView* view = new CADatePickerView(m_mode);
    if (view && view->initWithCenter(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CADatePickerView* CADatePickerView::createWithLayout(const CrossApp::DLayout &layout, CADatePickerView::Mode m_mode)
{
    CADatePickerView* view = new CADatePickerView(m_mode);
    if (view && view->initWithLayout(layout)) {
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
    m_pPickerView->initWithLayout(DLayoutFill);
    m_pPickerView->setFontSizeNormal(40);
    m_pPickerView->setFontSizeSelected(40);
    this->addSubview(m_pPickerView);

	m_pPickerView->onTitleForRow(CALLBACK_BIND_2(CADatePickerView::titleForRow, this));
    
	
    return true;
}

void CADatePickerView::onEnterTransitionDidFinish()
{
	CAControl::onEnterTransitionDidFinish();
    if (!isSetDate)
    {
        setMode(m_eMode);
    }
}

void CADatePickerView::onExitTransitionDidStart()
{
	CAControl::onExitTransitionDidStart();
}

void CADatePickerView::setBackgroundImage(CAImage* image, bool isScale9)
{
    m_pPickerView->setBackgroundImage(image, isScale9);
}

void CADatePickerView::setBackgroundImageForSelected(CAImage* image, bool isScale9)
{
    CC_SAFE_RETAIN(image);
    CC_SAFE_RELEASE(m_obBackgroundImageForSelected);
    m_obBackgroundImageForSelected = image;
    m_pPickerView->onViewForSelected([&](unsigned int component, DSize size){
        
        CAView* backgroundView = nullptr;
        
        if (isScale9)
        {
            backgroundView = CAScale9ImageView::createWithImage(m_obBackgroundImageForSelected);
            backgroundView->setLayout(DLayoutFill);
            return backgroundView;
        }
        else
        {
            backgroundView = CAImageView::createWithImage(m_obBackgroundImageForSelected);
            backgroundView->setLayout(DLayoutFill);
            
        }
        return backgroundView;
    });
}


void CADatePickerView::setDate(int year, int month, int day, bool animated)
{
    isSetDate = false;
    if (year>=1900 && year<=2100) {
        m_tTM.tm_year = year-1900;
    }
    
    if (month>=1 && month<=12) {
        m_tTM.tm_mon = month-1;
    }
    
    if (day>=1 && day<=31) {
        m_tTM.tm_mday = day;
    }
    
    CAScheduler::getScheduler()->scheduleOnce([=](float dt){
        if (!isSetDate) this->setMode(m_eMode);
    }, "CADatePickerView::update", this, 0.1);
}

void CADatePickerView::setTime(long time)
{
    isSetDate = false;

    std::time_t t = (std::time_t)time;
    auto tm = std::localtime(&t);
    m_tTM.tm_sec = tm->tm_sec;
    m_tTM.tm_min = tm->tm_min;
    m_tTM.tm_hour = tm->tm_hour;
    m_tTM.tm_mday = tm->tm_mday;
    m_tTM.tm_mon = tm->tm_mon;
    m_tTM.tm_year = tm->tm_year;
    m_tTM.tm_wday = tm->tm_wday;
    m_tTM.tm_yday = tm->tm_yday;
    m_tTM.tm_isdst = tm->tm_isdst;
    
    CAScheduler::getScheduler()->scheduleOnce([=](float dt){
        if (!isSetDate) this->setMode(m_eMode);
    }, "CADatePickerView::update", this, 0.1);
}

unsigned int CADatePickerView::numberOfComponentsInPickerView(CAPickerView* pickerView)
{
    int components = 0;
    switch (m_eMode)
    {
        case CADatePickerView::Mode::CountDownTimer:
            components = 2;
            break;
            
        case CADatePickerView::Mode::Date:
            components = 3;
            break;
            
        case CADatePickerView::Mode::DateAndTime:
            components = 3;
            break;
            
        case CADatePickerView::Mode::Time:
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
        case CADatePickerView::Mode::CountDownTimer:
            if (component == 0) { // hour
                row = 24; // 0 ~ 23
            } else { // min
                row = 60; // 0 ~ 59
            }
            break;
            
        case CADatePickerView::Mode::Date:
            if (component == 0) { // year
                row = 2100 - 1900+1; // 1900~2100
            } else if (component == 1) { // month
                row = 12;
            } else { // day
                row = CACalendar::create()->_dayCountOfMonth(m_tTM.tm_year+1900,m_tTM.tm_mon+1);
            }
            break;
            
        case CADatePickerView::Mode::DateAndTime:
            if (component == 0) { // date
                row = CACalendar::create()->_isLeapYear(m_tTM.tm_year+1900) ? 366 : 365;
            } else if (component == 1) { // hour
                row = 24;
            } else { // minutes
                row = 60;
            }
            break;
            
        case CADatePickerView::Mode::Time:
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
        case CADatePickerView::Mode::CountDownTimer:
            if (component == 0) { // hour                
            } else { // min
            }
            break;
            
        case CADatePickerView::Mode::Date:
            if (component == 0) { // years
            } else if (component == 1) { // month
            } else { // day
            }
            break;
            
        case CADatePickerView::Mode::DateAndTime:
            width = m_pPickerView->getFrame().size.width/4;
            if (component == 0) { // date
                width *= 2;
            } else if (component == 1) { // hour
            } else { // minutes
            }
            break;
            
        case CADatePickerView::Mode::Time:
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
    return 80;
}

std::string CADatePickerView::titleForRow(unsigned int row, unsigned int component)
{
    char buff[256] = {0};
    switch (m_eMode)
    {
        case CADatePickerView::Mode::CountDownTimer:
            if (component == 0)
            { // hour
                sprintf(buff, "%d%s", row, UTF8("\u5c0f\u65f6"));
            } else { // min
				sprintf(buff, "%d%s", row, UTF8("\u5206\u949f"));
            }
            break;
            
        case CADatePickerView::Mode::Date:
            if (component == 0)
            { // years
				sprintf(buff, "%d%s", row + 1900, UTF8("\u5e74"));
            } else if (component == 1) { // month
				sprintf(buff, "%d%s", row + 1, UTF8("\u6708"));
            } else { // day
				sprintf(buff, "%d%s", row + 1, UTF8("\u65e5"));
            }
            break;
            
        case CADatePickerView::Mode::DateAndTime:
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
				const std::string week_s[7] = {

					UTF8("\u5468\u65e5"), 
					UTF8("\u5468\u4e00"), 
					UTF8("\u5468\u4e8c"), 
					UTF8("\u5468\u4e09"),
					UTF8("\u5468\u56db"), 
					UTF8("\u5468\u4e94"), 
					UTF8("\u5468\u516d") 
				};

				sprintf(buff, "%d%s%d%s  %s", month, UTF8("\u6708"), date, UTF8("\u65e5"), week_s[week].c_str());
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
            
        case CADatePickerView::Mode::Time:
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
	return buff;
}

void CADatePickerView::didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    switch (m_eMode)
    {
        case CADatePickerView::Mode::CountDownTimer:
            if (component == 0)
            { // hour
                m_tTM.tm_hour = row;
            }
            else
            { // min
                m_tTM.tm_min = row;
            }
            break;
            
        case CADatePickerView::Mode::Date:
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
                m_tTM.tm_mon = row;
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
        case CADatePickerView::Mode::DateAndTime:
            if (component == 0)
            { // date
                CACalendar* cal = CACalendar::create();
                int month, date;
                cal->dateByDayOfYear(m_tTM.tm_year, row, month, date);
                m_tTM.tm_mon = month;
                m_tTM.tm_mday = date;
                m_tTM.tm_mon--;
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
            
        case CADatePickerView::Mode::Time:
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
    
    if (m_obSelectRow)
    {
        m_obSelectRow(m_tTM);
    }
}

void CADatePickerView::setMode(CADatePickerView::Mode mode)
{
    m_eMode = mode;
    
    if (m_pPickerView)
    {
        m_pPickerView->setPickerViewDelegate(this);
        m_pPickerView->setPickerViewDataSource(this);
        m_pPickerView->reloadAllComponents();

        switch (m_eMode)
        {
            case CADatePickerView::Mode::Date:
                m_pPickerView->selectRow(m_tTM.tm_year, 0);
                m_pPickerView->selectRow(m_tTM.tm_mon, 1);
                m_pPickerView->selectRow(m_tTM.tm_mday-1, 2);
                break;
            case CADatePickerView::Mode::DateAndTime:
                m_pPickerView->selectRow(CACalendar::create(mktime(&m_tTM))->dayOfYear(), 0);
                m_pPickerView->selectRow(m_tTM.tm_hour, 1);
                m_pPickerView->selectRow(m_tTM.tm_min, 2);
                break;
            case CADatePickerView::Mode::Time:
                m_pPickerView->selectRow(m_tTM.tm_hour, 0);
                m_pPickerView->selectRow(m_tTM.tm_min, 1);
                break;
            default:
                break;
        }
    }
}


NS_CC_END
