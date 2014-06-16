//
//  CALabel.cpp
//  project
//
//  Created by lh on 14-5-12.
//
//

#include "CALabel.h"
#include "include/ccMacros.h"
#include "CCDirector.h"
NS_CC_BEGIN
CALabel::CALabel()
:m_nNumberOfLine(0),
m_nTextAlignment(kCCTextAlignmentLeft),
m_nText(""),
m_nfontName("Arial"),
m_nTextcolor(ccc4(0, 0, 0, 255)),
m_nVerticalTextAlignmet(kCCVerticalTextAlignmentTop),
m_pTextImage(NULL),
m_nDimensions(CCSizeZero),
m_nfontSize(18),
m_cLabelSize(CCSizeZero)
{
    
}

CALabel::~CALabel()
{
    
}

CALabel *CALabel::createWithFrame(const CCRect& rect)
{
    CALabel *label = new CALabel();
    if (label && label->initWithFrame(rect))
    {
        label->autorelease();
        return label;
    }
    CC_SAFE_DELETE(label);
    return NULL;
}

CALabel* CALabel::createWithCenter(const cocos2d::CCRect &rect)
{
    CALabel *label = new CALabel();
    if (label && label->initWithCenter(rect))
    {
        label->autorelease();
        return label;
    }
    CC_SAFE_DELETE(label);
    return NULL;
}

void CALabel::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    this->updateImage();
}

bool CALabel::initWithFrame(const CCRect& rect)
{
   
    if (!CAView::initWithFrame(rect))
    {
        return false;
    }
    
    this->setColor(CAColor_clear);
    
    m_pTextImage = CAImageView::create();
    
    m_pTextImage->setFrameOrigin(CCPointZero);
    
    this->addSubview(m_pTextImage);
    
    return true;
}

bool CALabel::initWithCenter(const CCRect& rect)
{
    
    if (!CAView::initWithCenter(rect))
    {
        return false;
    }
    
    this->setColor(CAColor_clear);
    
    m_pTextImage = CAImageView::create();
    
    m_pTextImage->setFrame(CCRectZero);
    
    this->addSubview(m_pTextImage);
    
    return true;
}

caFontDefinition CALabel::setFontDefiniton(bool flag)
{
    caFontDefinition texDef;
    
    if (flag)
        texDef.m_fontSize       =  m_nfontSize * CC_CONTENT_SCALE_FACTOR();
    else
        texDef.m_fontSize       =  m_nfontSize;
    
    texDef.m_fontName       = m_nfontName;
    texDef.m_alignment      =  m_nTextAlignment;
    texDef.m_vertAlignment  =  m_nVerticalTextAlignmet;
    
    
    if (flag)
        texDef.m_dimensions     =  CC_SIZE_POINTS_TO_PIXELS(m_nDimensions);
    else
        texDef.m_dimensions     =  m_nDimensions;
    
    // text tint
    texDef.m_fontFillColor = m_nTextcolor;
    
    return texDef;
}

bool CALabel::updateImage()
{
    if (m_bRunning == false)
    {
        return false;
    }
    
    CAImage *tex = new CAImage();
    if (!tex) {
        return false;
    }
    caFontDefinition fontDef = setFontDefiniton(true);

    CAImage *at = new CAImage();
    at->initWithString("9m", fontDef.m_fontName.c_str(), fontDef.m_fontSize, CCSizeZero, fontDef.m_alignment, fontDef.m_vertAlignment);
    float fontHeight = at->getContentSize().height+1;
    at->release();
    unsigned int linenumber = (int)this->getBounds().size.height/fontHeight;
    if (m_nNumberOfLine <= linenumber && m_nNumberOfLine != 0)
    {
        
        tex->initWithString(m_nText.c_str(), fontDef.m_fontName.c_str(), fontDef.m_fontSize, CCSizeZero, fontDef.m_alignment, fontDef.m_vertAlignment);
    }
    else
    {
        if (this->getBounds().size.width == 0 && this->getBounds().size.height == 0)
        {
            tex->initWithString(m_nText.c_str(), fontDef.m_fontName.c_str(), fontDef.m_fontSize, CCSizeZero, fontDef.m_alignment, fontDef.m_vertAlignment);
        }
        else
        {
            tex->initWithString(m_nText.c_str(), fontDef.m_fontName.c_str(), fontDef.m_fontSize, this->getBounds().size, fontDef.m_alignment, fontDef.m_vertAlignment);
        }
    }
    
    CCRect rect = CCRectZero;
    rect.size = tex->getContentSize();
    
    m_cLabelSize = tex->getContentSize();
    
    m_pTextImage->setImage(tex);

    m_pTextImage->setImageRect(rect, false, rect.size);
    
    m_pTextImage->setColor(m_nTextcolor);

    m_pTextImage->setFrameOrigin(CCPoint(0, (this->getBounds().size.height - rect.size.height)/2));

    tex->release();
    return true;
}

void CALabel::setDimensions(cocos2d::CCSize var)
{
    m_nDimensions = var;
    this->updateImage();
}

CCSize CALabel::getDimensions()
{
    return m_nDimensions;
}

void CALabel::setText(string var)
{
    m_nText = var;

    this->updateImage();
}

void CALabel::setTextAlignment(CCTextAlignment var)
{
    m_nTextAlignment = var;
    
    if (m_nText.size()>0)
    {
        this->updateImage();
    }
    
}

CCTextAlignment CALabel::getTextAlignment()
{
    return m_nTextAlignment;
}

CAColor4B CALabel::getTextcolor()
{
    return m_nTextcolor;
}

//CAColor4B CALabel::getShadowcolor()
//{
//    return m_nShadowcolor;
//}

//CCSize CALabel::getShadowOffset()
//{
//    return m_nShadowOffset;
//}

string CALabel::getText()
{
    return m_nText;
}

unsigned int CALabel::getNumberOfLine()
{
    return m_nNumberOfLine;
}

void CALabel::setTextcolor(CAColor4B var)
{
    m_nTextcolor = var;
  
    this->updateImage();
}

//void CALabel::setShadowcolor(CAColor4B var)
//{
//    m_nShadowcolor= var;
//    if (m_nText.size()>0)
//    {
//        this->updataImage();
//    }
//    
//}

//void CALabel::setShadowOffset(cocos2d::CCSize var)
//{
//    m_nShadowOffset = var;
//    this->updataImage();
//}

void CALabel::setNumberOfLine(unsigned int var)
{
    m_nNumberOfLine = var;
    this->updateImage();
}

NS_CC_END