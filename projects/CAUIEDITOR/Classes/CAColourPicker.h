//
//  CAColourPicker.h
//  CrossApp
//
//  Created by QiaoXin on 15-9-6.
//  Email:qiaoxin265@126.com
//

#ifndef __CACOLOUR_PICKER_H__
#define __CACOLOUR_PICKER_H__


#include <iostream>
#include "CrossApp.h"
#include "CASaturationBrightnessPicker.h"

USING_NS_CC;

NS_CC_BEGIN

class CASaturationBrightnessPicker;

class CAColourPickerDelegate
{
public:
    virtual void selectedPickerDelegate(CAObject* obj, float r, float g, float b) = 0;
};

class  CC_DLL CAColourPicker: public CAControl,public CASaturationBrightnessPickerDelegate, public CAHuePickerDelegate
{
public:
    static CAColourPicker* createWithFrame(CCRect rect);
    static CAColourPicker* createWithCenter(CCRect rect);
    CAColourPicker();
    virtual ~CAColourPicker();
    bool init();

    void setDelegate(CAColourPickerDelegate* delegate);
    void setColor(const CAColor4B &colorValue);
protected:
    void updateHueAndSaturationBrightnessPicker();
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void selectedHuePicker(CAObject* sender, float hue);
    virtual void selectedSaturationBrightnessPicker(CAObject* sender, float r, float g, float b);
protected:
    HSV m_pHSV;
    CASaturationBrightnessPicker* m_pSaturationBrightnessPicker;
    CAColourPickerDelegate* m_pDelegate;
    CAHuePicker*            m_pHuePicker;
    CAImageView*            m_pBackground;
    CCRect                  m_pRect;
};

NS_CC_END
#endif//__CACOLOUR_PICKER_H__
