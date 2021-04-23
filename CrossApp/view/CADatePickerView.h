//
//  CADatePickerView.h
//  CrossApp
//
//  Created by dai xinping on 14-8-4.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
//

#ifndef __CrossApp__CADatePickerView__
#define __CrossApp__CADatePickerView__

#include <iostream>
#include "CAPickerView.h"

NS_CC_BEGIN

class CC_DLL CADatePickerView : public CAControl, public CAPickerViewDataSource, public CAPickerViewDelegate {
    
public:

    enum class Mode : int
    {
        Time = 0,           // Displays hour, minute, and optionally AM/PM designation depending on the locale setting (e.g. 6 | 53 | PM)
        Date,           // Displays month, day, and year depending on the locale setting (e.g. November | 15 | 2007)
        DateAndTime,    // Displays date, hour, minute, and optionally AM/PM designation depending on the locale setting (e.g. Wed Nov 15 | 6 | 53 | PM)
        CountDownTimer, // Displays hour and minute (e.g. 1 | 53)
    };
    
public:

    CC_LISTENING_FUNCTION(void(const tm&), SelectRow)
        
public:
    static CADatePickerView* create(CADatePickerView::Mode m_mode);
    static CADatePickerView* createWithFrame(const DRect& rect, CADatePickerView::Mode m_mode);
    static CADatePickerView* createWithCenter(const DRect& rect, CADatePickerView::Mode m_mode);
    static CADatePickerView* createWithLayout(const DLayout& layout, CADatePickerView::Mode m_mode);
    
    CADatePickerView(CADatePickerView::Mode m_mode);
    virtual ~CADatePickerView();
    
    virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
    
        
    void setBackgroundImage(CAImage* image, bool isScale9 = false);
    void setBackgroundImageForSelected(CAImage* image, bool isScale9 = false);
        
public:
    void setDate(int year, int month, int day, bool animated);
    void setTime(long time);

protected:
    virtual unsigned int numberOfComponentsInPickerView(CAPickerView* pickerView);
    virtual unsigned int numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component);
    virtual float widthForComponent(CAPickerView* pickerView, unsigned int component);
    virtual float rowHeightForComponent(CAPickerView* pickerView, unsigned int component);
    std::string titleForRow( unsigned int row, unsigned int component);
    
    virtual void didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component);
    void setMode(CADatePickerView::Mode mode);
    
private:
    
    CAImage* m_obBackgroundImageForSelected;
        
    CAPickerView* m_pPickerView;
    tm m_tTM;
    CADatePickerView::Mode m_eMode;
    bool isSetDate;
};

NS_CC_END

#endif /* defined(__CrossApp__CADatePickerView__) */
