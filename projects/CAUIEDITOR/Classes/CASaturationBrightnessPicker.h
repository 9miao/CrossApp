//
//  CASaturationBrightnessPicker.h
//  CrossApp
//
//  Created by QiaoXin on 15-9-7.
//  Email:qiaoxin265@126.com
//

#ifndef __CASATURATION_PICKER_H__
#define __CASATURATION_PICKER_H__

#include <iostream>
#include "CAHuePicker.h"

USING_NS_CC;

NS_CC_BEGIN

class CASaturationBrightnessPickerDelegate
{
public:
    virtual void selectedSaturationBrightnessPicker(CAObject* sender, float r, float g, float b) = 0;
};


class CC_DLL CASaturationBrightnessPicker : public CAControl
{
public:
    static CASaturationBrightnessPicker* create(CCRect rect);
    CASaturationBrightnessPicker();
    virtual ~CASaturationBrightnessPicker();
    bool init();
    virtual void updateWithHSV(HSV hsv);
    virtual void updateDraggerWithHSV(HSV hsv);
    void setDelegate(CASaturationBrightnessPickerDelegate* delegate);
protected:
    void updateSliderPosition(CCPoint location);
    bool checkSliderPosition(CCPoint location);
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
protected:
    int         m_iBoxPos;
    int         m_iBoxSize;
    CCRect      m_pRect;
    HSV         m_pHsv;
    CASaturationBrightnessPickerDelegate* m_pDelegate;
    CC_SYNTHESIZE_READONLY(float, m_fSaturation, Saturation);
    CC_SYNTHESIZE_READONLY(float, m_fBrightness, Brightness);
    CC_SYNTHESIZE_READONLY(CAImageView*, m_pBackground, Background);
    CC_SYNTHESIZE_READONLY(CAImageView*, m_pOverlay, Overlay);
    CC_SYNTHESIZE_READONLY(CAImageView*, m_pShadow, Shadow);
    CC_SYNTHESIZE_READONLY(CAImageView*, m_pSlider, Slider);
    CC_SYNTHESIZE_READONLY(CCPoint, m_pStartPos, StartPos);
};
NS_CC_END
#endif//__CASATURATION_PICKER_H__
