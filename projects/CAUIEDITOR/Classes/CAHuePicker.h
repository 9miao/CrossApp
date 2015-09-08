//
//  CAHuePicker.h
//  CrossApp
//
//  Created by QiaoXin on 15-9-7.
//  Email:qiaoxin265@126.com
//

#ifndef __CAHUE_PICKER_H__
#define __CAHUE_PICKER_H__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

typedef struct HSV{
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} HSV;

class CAHuePickerDelegate
{
public:
    virtual void selectedHuePicker(CAObject* sender, float hue) = 0;
};

class CC_DLL CAHuePicker : public CAControl
{
public:
    static CAHuePicker* create(CCRect rect);

    CAHuePicker();
    virtual ~CAHuePicker();
    bool init();
    void setHuePickerDelegate(CAHuePickerDelegate* delegate);
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);

    void sendDelegate();
protected:
    void updateSliderPosition(CCPoint location);
    bool checkSliderPosition(CCPoint location);

    CC_SYNTHESIZE_READONLY(float, m_fHue, Hue);
    virtual void setHue(float val);
    CC_SYNTHESIZE_READONLY(float, m_fHuePercentage, HuePercentage);
    virtual void setHuePercentage(float val);

    CC_SYNTHESIZE_RETAIN(CAImageView*, m_pBackground, Background);
    CC_SYNTHESIZE_RETAIN(CAImageView*, m_pSlider, Slider);
    CAHuePickerDelegate* m_pDelegate;
    CCRect m_pRect;
};


static CAColor4F  RGBfromHSV(HSV value)
{
    double      hh, p, q, t, ff;
    long        i;
    CAColor4F        out;
    out.a        = 1;
    
    if (value.s <= 0.0) // < is bogus, just shuts up warnings
    {
        if (isnan(value.h)) // value.h == NAN
        {
            out.r = value.v;
            out.g = value.v;
            out.b = value.v;
            return out;
        }
        
        // error - should never happen
        out.r = 0.0;
        out.g = 0.0;
        out.b = 0.0;
        return out;
    }
    
    hh = 360.0 - value.h;
    if(hh >= 360.0 || hh <0.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = value.v * (1.0 - value.s);
    q = value.v * (1.0 - (value.s * ff));
    t = value.v * (1.0 - (value.s * (1.0 - ff)));
    
    switch(i)
    {
        case 0:
            out.r = value.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = value.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = value.v;
            out.b = t;
            break;
            
        case 3:
            out.r = p;
            out.g = q;
            out.b = value.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = value.v;
            break;
        case 5:
        default:
            out.r = value.v;
            out.g = p;
            out.b = q;
            break;
    }
    return out;
}

static HSV HSVfromRGB(CAColor4F value)
{
    HSV         out;
    double      min, max, delta;
    
    min = value.r < value.g ? value.r : value.g;
    min = min  < value.b ? min  : value.b;
    
    max = value.r > value.g ? value.r : value.g;
    max = max  > value.b ? max  : value.b;
    
    out.v = max;                                // v
    delta = max - min;
    if(delta == 0)
        delta = 0.01;
    if( max > 0.0 )
    {
        out.s = (delta / max);                  // s
    } else
    {
        // r = g = b = 0                        // s = 0, v is undefined
        out.s = 0.0;
        out.h = -1;                            // its now undefined (don't know if setting to NAN is a good idea)
        return out;
    }
    if( value.r >= max )                        // > is bogus, just keeps compilor happy
    {
        out.h = ( value.g - value.b ) / delta;        // between yellow & magenta
    } else
    {
        if( value.g >= max )
            out.h = 2.0 + ( value.b - value.r ) / delta;  // between cyan & yellow
        else
            out.h = 4.0 + ( value.r - value.g ) / delta;  // between magenta & cyan
    }
    
    out.h *= 60.0;                              // degrees
    CCLog("%f", out.h);
    out.h = 360 - out.h;
    CCLog("%f", out.h);

    if( out.h < 0.0 )
        out.h += 360.0;
    CCLog("%f", out.h);

    return out;
}

#endif//__CAHUE_PICKER_H__
