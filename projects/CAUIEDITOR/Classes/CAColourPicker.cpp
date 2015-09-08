//
//  CAColourPicker.cpp
//  CrossApp
//
//  Created by QiaoXin on 15-9-6.
//  Email:qiaoxin265@126.com
//

#include "CrossApp.h"
#include "CAColourPicker.h"

USING_NS_CC;

CAColourPicker::CAColourPicker()
: m_pBackground(NULL)
, m_pSaturationBrightnessPicker(NULL)
, m_pHuePicker(NULL)
, m_pDelegate(NULL)
{
    m_pHSV.h = 1;
    m_pHSV.v = 1;
    m_pHSV.s = 1;
}

CAColourPicker::~CAColourPicker()
{
    CC_SAFE_RELEASE(m_pBackground);
    CC_SAFE_RELEASE(m_pHuePicker);
}

CAColourPicker* CAColourPicker::createWithFrame(CCRect rect)
{
    CAColourPicker* obj = new CAColourPicker();
    obj->m_pRect = rect;
    if(obj && obj->initWithFrame(rect))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return obj;
}

CAColourPicker* CAColourPicker::createWithCenter(CCRect rect)
{
    CAColourPicker* obj = new CAColourPicker();
    obj->m_pRect = rect;
    if(obj && obj->initWithCenter(rect))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return obj;
}

bool CAColourPicker::init()
{
    if (CAControl::init())
    {
        m_pBackground = CAImageView::createWithFrame(CCRect(0, 0, m_pRect.size.width, m_pRect.size.height));
        m_pBackground->setImage(CAImage::create("source_material/bg_badge.png"));
        this->addSubview(m_pBackground);
        m_pHuePicker = CAHuePicker::create(CCRect(0,0,m_pRect.size.width,m_pRect.size.height));
        m_pHuePicker->setHuePickerDelegate(this);
        this->addSubview(m_pHuePicker);
        m_pSaturationBrightnessPicker = CASaturationBrightnessPicker::create(CCRect(m_pRect.size.width*0.5f,m_pRect.size.height*0.5f,m_pRect.size.width*0.66,m_pRect.size.height*0.66));
        m_pSaturationBrightnessPicker->setDelegate(this);
        this->addSubview(m_pSaturationBrightnessPicker);
        
        // Set defaults
        updateHueAndSaturationBrightnessPicker();
        m_pHuePicker->setHue(180);
        return true;
    }
    return false;
}

void CAColourPicker::setDelegate(CAColourPickerDelegate* delegate)
{
    m_pDelegate = delegate;
}

void CAColourPicker::setColor(const CAColor4B &color)
{
    CAColor4F rgb    = ccc4FFromccc4B(color);
    m_pHSV = HSVfromRGB(rgb);
    updateHueAndSaturationBrightnessPicker();
}

void CAColourPicker::updateHueAndSaturationBrightnessPicker()
{
    m_pHuePicker->setHue(m_pHSV.h);
    m_pSaturationBrightnessPicker->updateWithHSV(m_pHSV);
    m_pSaturationBrightnessPicker->updateDraggerWithHSV(m_pHSV);
}

bool CAColourPicker::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return false;
}

void CAColourPicker::selectedHuePicker(CrossApp::CAObject *sender, float hue)
{
    m_pHSV.h = hue;
    m_pHSV.s = m_pSaturationBrightnessPicker->getSaturation();
    m_pHSV.v = m_pSaturationBrightnessPicker->getBrightness();
    if(m_pSaturationBrightnessPicker)
    {
        m_pSaturationBrightnessPicker->updateWithHSV(m_pHSV);
    }
    if(m_pDelegate)
    {
        CAColor4B rgb    = ccc4BFromccc4F(RGBfromHSV(m_pHSV));
        m_pDelegate->selectedPickerDelegate(this, rgb.r, rgb.g, rgb.b);
    }
}

void CAColourPicker::selectedSaturationBrightnessPicker(CAObject* sender, float r, float g, float b)
{
    if(m_pDelegate)
    {
        m_pDelegate->selectedPickerDelegate(this, r, g, b);
    }
}