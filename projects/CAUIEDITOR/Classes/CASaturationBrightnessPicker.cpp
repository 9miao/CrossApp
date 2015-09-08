//
//  CASaturationBrightnessPicker.cpp
//  CrossApp
//
//  Created by QiaoXin on 15-9-7.
//  Email:qiaoxin265@126.com
//

#include "CASaturationBrightnessPicker.h"


CASaturationBrightnessPicker::CASaturationBrightnessPicker()
: m_fSaturation(0.0f)
, m_fBrightness(0.0f)
, m_pBackground(NULL)
, m_pOverlay(NULL)
, m_pShadow(NULL)
, m_pSlider(NULL)
, m_iBoxPos(0)
, m_iBoxSize(0)
, m_pDelegate(NULL)
{
    m_pHsv.h = 1;
    m_pHsv.v = 1;
    m_pHsv.s = 1;
}

CASaturationBrightnessPicker::~CASaturationBrightnessPicker()
{
    m_pDelegate   = NULL;
    m_pBackground = NULL;
    m_pOverlay    = NULL;
    m_pShadow     = NULL;
    m_pSlider     = NULL;
}

CASaturationBrightnessPicker* CASaturationBrightnessPicker::create(CCRect rect)
{
    CASaturationBrightnessPicker* obj = new CASaturationBrightnessPicker();
    if(obj)
    {
        obj->m_pRect = rect;
        obj->initWithCenter(rect);
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return obj;
}

bool CASaturationBrightnessPicker::init()
{
    if (CAControl::init())
    {
        m_pBackground=CAImageView::createWithImage(CAImage::create("colourPickerBackground.png"));
        m_pBackground->setFrame(CCRect(0,0,m_pRect .size.width,m_pRect .size.height));
        this->addSubview(m_pBackground);
        
        m_pOverlay=CAImageView::createWithImage(CAImage::create("colourPickerOverlay.png"));
        m_pOverlay->setFrame(CCRect(0,0,m_pRect .size.width,m_pRect .size.height));
        this->addSubview(m_pOverlay);
        
        m_pShadow=CAImageView::createWithImage(CAImage::create("colourPickerShadow.png"));
        m_pShadow->setFrame(CCRect(0,0,m_pRect .size.width,m_pRect .size.height));
        this->addSubview(m_pShadow);
        
        m_pSlider=CAImageView::createWithImage(CAImage::create("colourPicker.png"));
        m_pSlider->setCenter(CCRect(m_pRect .size.width*0.5f,m_pRect .size.height*0.5f,15,15));
        this->addSubview(m_pSlider);

        m_iBoxPos          = m_pRect .size.width * 0.25;    // starting position of the virtual box area for picking a colour
        m_iBoxSize         = m_pBackground->getBounds().size.width / 2;    // the size (width and height) of the virtual box for picking a colour from
        return true;
    }
    return false;
}

void CASaturationBrightnessPicker::setDelegate(CASaturationBrightnessPickerDelegate* delegate)
{
    m_pDelegate = delegate;
}

void CASaturationBrightnessPicker::updateWithHSV(HSV hsv)
{
    m_pHsv = hsv;
    
    HSV tmpHSV;
    tmpHSV.s = 1;
    tmpHSV.h = hsv.h;
    tmpHSV.v = 1;
    
    CAColor4B rgb = ccc4BFromccc4F(RGBfromHSV(tmpHSV));
    m_pBackground->setColor(ccc4( (GLubyte)(rgb.r ), (GLubyte)(rgb.g * 1), (GLubyte)(rgb.b * 1), (GLubyte)(rgb.a * 1) ));
}

void CASaturationBrightnessPicker::updateDraggerWithHSV(HSV hsv)
{
    m_pHsv = hsv;
    CCPoint pos = CCPoint(m_pStartPos.x + m_iBoxPos + m_iBoxSize*(1-hsv.s),m_pStartPos.y + m_iBoxPos + m_iBoxSize*hsv.v);
    updateSliderPosition(pos);
}

void CASaturationBrightnessPicker::updateSliderPosition(CCPoint sliderPosition)
{
    float centerX           = m_pStartPos.x + m_pBackground->getBounds().size.width*0.5f;
    float centerY           = m_pStartPos.y + m_pBackground->getBounds().size.height*0.5f;
    
    float dx                = sliderPosition.x - centerX;
    float dy                = sliderPosition.y - centerY;
    float dist              = sqrtf(dx * dx + dy * dy);
    
    float angle             = atan2f(dy, dx);
    
    float limit             = m_pBackground->getBounds().size.width*0.5f;
    
    if (dist > limit)
    {
        sliderPosition.x    = centerX + limit * cosf(angle);
        sliderPosition.y    = centerY + limit * sinf(angle);
    }
    
    m_pSlider->setCenterOrigin(sliderPosition);
    
    if (sliderPosition.x < m_pStartPos.x + m_iBoxPos)                        sliderPosition.x = m_pStartPos.x + m_iBoxPos;
    else if (sliderPosition.x > m_pStartPos.x + m_iBoxPos + m_iBoxSize - 1)    sliderPosition.x = m_pStartPos.x + m_iBoxPos + m_iBoxSize - 1;
    if (sliderPosition.y < m_pStartPos.y + m_iBoxPos)                        sliderPosition.y = m_pStartPos.y + m_iBoxPos;
    else if (sliderPosition.y > m_pStartPos.y + m_iBoxPos + m_iBoxSize)        sliderPosition.y = m_pStartPos.y + m_iBoxPos + m_iBoxSize;
    
    m_fSaturation = 1.0f - fabs((m_pStartPos.x + (float)m_iBoxPos - sliderPosition.x)/(float)m_iBoxSize);
    m_fBrightness = fabs((m_pStartPos.y + (float)m_iBoxPos - sliderPosition.y)/(float)m_iBoxSize);
    
    m_pHsv.s=m_fSaturation;
    m_pHsv.v=m_fBrightness;
    if(m_pDelegate)
    {
        CAColor4B rgb    = ccc4BFromccc4F(RGBfromHSV(m_pHsv));
        m_pDelegate->selectedSaturationBrightnessPicker(this, rgb.r, rgb.g, rgb.b);
    }
}

bool CASaturationBrightnessPicker::checkSliderPosition(CCPoint location)
{
    float centerX           = m_pStartPos.x + m_pBackground->getBounds().size.width*0.5f;
    float centerY           = m_pStartPos.y + m_pBackground->getBounds().size.height*0.5f;
    
    float dx                = location.x - centerX;
    float dy                = location.y - centerY;
    float dist              = sqrtf(dx*dx+dy*dy);
    
    if (dist <= m_pBackground->getBounds().size.width*0.5f)
    {
        updateSliderPosition(location);
        return true;
    }
    return false;
}

bool CASaturationBrightnessPicker::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
    touchLocation = this->convertToNodeSpace(touchLocation);
    return checkSliderPosition(touchLocation);
}

void CASaturationBrightnessPicker::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
    touchLocation = this->convertToNodeSpace(touchLocation);
    checkSliderPosition(touchLocation);
}


