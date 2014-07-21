//
//  CALabel.cpp
//  project
//
//  Created by lh on 14-5-12.
//
//

#include "CALabel.h"
#include "ccMacros.h"
#include "CCApplication.h"
#include "images/CAImageCache.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include <locale>
#include <cstdlib>
NS_CC_BEGIN


CALabel::CALabel()
:m_nNumberOfLine(0),
m_nTextAlignment(CATextAlignmentLeft),
m_nText(""),
m_nfontName(""),
m_nVerticalTextAlignmet(CAVerticalTextAlignmentTop),
m_nDimensions(CCSizeZero),
m_nfontSize(18),
m_cLabelSize(CCSizeZero),
m_bUpdateImage(false),
pTextHeight(0)
{
    m_obContentSize = CCSizeZero;
    
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

CALabel* CALabel::createWithCenter(const CCRect &rect)
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
    if (m_bUpdateImage)
    {
        m_bUpdateImage = false;
        this->updateImage();
    }
}

bool CALabel::initWithFrame(const CCRect& rect)
{
   
    if (!CAView::initWithFrame(rect,CAColor_black))
    {
        return false;
    }
    return true;
}

bool CALabel::initWithCenter(const CCRect& rect)
{
    
    if (!CAView::initWithCenter(rect,CAColor_black))
    {
        return false;
    }
    return true;
}


void CALabel::updateImage()
{
	float fontHeight = CAImage::getFontHeight(m_nfontName.c_str(), m_nfontSize);
    unsigned int linenumber = 0;
    if (this->getBounds().size.height == 0)
    {
        linenumber = m_nNumberOfLine;
    }
    else
    {
        linenumber = (int)this->getBounds().size.height / fontHeight;
    }
    
    CCSize size = CCSizeZero;
    if ((m_nNumberOfLine <= linenumber && m_nNumberOfLine != 0))
    {
        size = CCSizeMake(this->getBounds().size.width, fontHeight*m_nNumberOfLine);
    }
    else
    {
        
        size = this->getBounds().size;
        size.height = linenumber *fontHeight;
        
    }

	CAImage* image = CAImage::createWithString(m_nText.c_str(),
                                               m_nfontName.c_str(),
                                               m_nfontSize,
                                               size,
                                               m_nTextAlignment,
                                               m_nVerticalTextAlignmet);

	CC_RETURN_IF(image == NULL);

    m_cLabelSize = image->getContentSize();
    
    CCRect rect = CCRectZero;
    rect.size.width = this->getBounds().size.width;
    rect.size.height = image->getContentSize().height;
    float width = MIN(this->getBounds().size.width,image->getContentSize().width);
    rect.size.width = width;
    
    this->setImage(image);
    
    switch (m_nVerticalTextAlignmet)
    {
        case CAVerticalTextAlignmentTop:
            pTextHeight =0;
            break;
        case CAVerticalTextAlignmentCenter:
            pTextHeight =(this->getBounds().size.height-rect.size.height)/2;
            break;
        case CAVerticalTextAlignmentBottom:
            pTextHeight =this->getBounds().size.height - rect.size.height;
            break;
        default:
            break;
    }
    
    this->setImageRect(rect);
}

void CALabel::updateImageRect()
{
    GLfloat x1,x2,y1,y2;
    x1=0;
    y1=0;
    y1=m_obContentSize.height - m_obRect.size.height - y1;
    y1=y1-pTextHeight;
    x2=x1+m_obRect.size.width;
    y2=y1+m_obRect.size.height;
    m_sQuad.bl.vertices = vertex3(x1, y1, 0);
    m_sQuad.br.vertices = vertex3(x2, y1, 0);
    m_sQuad.tl.vertices = vertex3(x1, y2, 0);
    m_sQuad.tr.vertices = vertex3(x2, y2, 0);
}

void CALabel::setDimensions(CCSize var)
{
    m_nDimensions = var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

CCSize CALabel::getDimensions()
{
    return m_nDimensions;
}

void CALabel::setText(string var)
{
    m_nText = var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

void CALabel::setTextAlignment(CATextAlignment var)
{
    m_nTextAlignment = var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

CATextAlignment CALabel::getTextAlignment()
{
    return m_nTextAlignment;
}

string CALabel::getText()
{
    return m_nText;
}

unsigned int CALabel::getNumberOfLine()
{
    return m_nNumberOfLine;
}

void CALabel::setNumberOfLine(unsigned int var)
{
    m_nNumberOfLine = var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

void CALabel::setFontSize(unsigned int var)
{
    m_nfontSize=var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

unsigned int CALabel::getFontSize()
{
    return m_nfontSize;
}

void CALabel::setFontName(string var)
{
    m_nfontName=var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

std::string CALabel::getFontName()
{
    return m_nfontName;
}

void CALabel::setVerticalTextAlignmet(CAVerticalTextAlignment var)
{
    m_nVerticalTextAlignmet=var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

CAVerticalTextAlignment CALabel::getVerticalTextAlignmet()
{
    return m_nVerticalTextAlignmet;
}

void CALabel::setContentSize(const CrossApp::CCSize &var)
{
    CAView::setContentSize(var);
    m_bUpdateImage = true;
}

void CALabel::visit()
{
    CAView::visit();
    if (m_bUpdateImage)
    {
        m_bUpdateImage = false;
        this->updateImage();
    }
}

NS_CC_END