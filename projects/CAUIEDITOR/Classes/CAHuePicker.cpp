//
//  CAHuePicker.cpp
//  CrossApp
//
//  Created by QiaoXin on 15-9-7.
//  Email:qiaoxin265@126.com
//

#include "CAHuePicker.h"

USING_NS_CC;

CAHuePicker::CAHuePicker()
: m_fHue(0.0f)
, m_fHuePercentage(0.0f)
, m_pBackground(NULL)
, m_pSlider(NULL)
, m_pDelegate(NULL)
{

}

CAHuePicker::~CAHuePicker()
{
    CC_SAFE_RELEASE(m_pBackground);
    CC_SAFE_RELEASE(m_pSlider);
    m_pDelegate = NULL;
}

CAHuePicker* CAHuePicker::create(CCRect rect)
{
    CAHuePicker* obj = new CAHuePicker();
    if(obj)
    {
        obj->m_pRect = rect;
        obj->initWithFrame(rect);
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return obj;
}

bool CAHuePicker::init()
{
    if (CAControl::init())
    {
        m_pBackground = CAImageView::createWithImage(CAImage::create("huePickerBackground.png"));
        m_pBackground->setFrame(m_pRect);
        this->addSubview(m_pBackground);
        m_pSlider = CAImageView::createWithImage(CAImage::create("colourPicker.png"));
        m_pSlider->setCenter(CCRect(0, 0, 15, 15));
        this->addSubview(m_pSlider);
        setHue(m_fHue);
        return true;
    }
    return false;
}

void CAHuePicker::setHue(float hueValue)
{
    m_fHue=hueValue;
    float huePercentage	= hueValue / 360.0f;
    setHuePercentage(huePercentage);
}

void CAHuePicker::setHuePercentage(float hueValueInPercent)
{
    m_fHuePercentage=hueValueInPercent;
    m_fHue=m_fHuePercentage*360.0f;
    CCRect backgroundBox=m_pBackground->getBounds();
    float limit             = (backgroundBox.size.width) * 0.45f;
    float angleDeg          = m_fHuePercentage * 360.0f - 180.0f;
    float angle             = CC_DEGREES_TO_RADIANS(angleDeg);
    float x                 = backgroundBox.size.width*0.5 + limit * cosf(angle);
    float y                 = backgroundBox.size.height*0.5 + limit * sinf(angle);
    m_pSlider->setCenterOrigin(CCPoint(x, y));
}

void CAHuePicker::sendDelegate()
{
    if(m_pDelegate)
    {
        m_pDelegate->selectedHuePicker(this, m_fHue);
    }
}

void CAHuePicker::setHuePickerDelegate(CAHuePickerDelegate* delegate)
{
    m_pDelegate = delegate;
}

void CAHuePicker::updateSliderPosition(CCPoint location)
{
    CCRect backgroundBox=m_pBackground->getBounds();
    float dx                = location.x - backgroundBox.size.width * 0.5f;
    float dy                = location.y - backgroundBox.size.height * 0.5f;
    float angle             = atan2f(dy, dx);
    float angleDeg          = CC_RADIANS_TO_DEGREES(angle) + 180.0f;
    setHue(angleDeg);
    sendDelegate();
}

bool CAHuePicker::checkSliderPosition(CCPoint location)
{
    float distanceMax = this->getBounds().size.width*0.5f;
    double distance = sqrt(pow (location.x - distanceMax, 2) + pow(location.y - distanceMax, 2));
    if (distanceMax > distance && (distanceMax*0.76) < distance)
    {
        updateSliderPosition(location);
        return true;
    }
    return false;
}

bool CAHuePicker::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
    touchLocation = this->convertToNodeSpace(touchLocation);
    return checkSliderPosition(touchLocation);
}


void CAHuePicker::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
    touchLocation = this->convertToNodeSpace(touchLocation);
    updateSliderPosition(touchLocation);
    checkSliderPosition(touchLocation);
}

