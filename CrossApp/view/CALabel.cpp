//
//  CALabel.cpp
//  project
//
//  Created by lh on 14-5-12.
//
//

#include "CALabel.h"
#include "ccMacros.h"
#include <locale>
#include <cstdlib>

NS_CC_BEGIN


CALabel::CALabel()
:m_nNumberOfLine(0)
,m_nTextAlignment(CATextAlignmentLeft)
,m_nText("")
,m_nfontName("")
,m_nVerticalTextAlignmet(CAVerticalTextAlignmentTop)
,m_nDimensions(CCSizeZero)
,m_nfontSize(18)
,m_cLabelSize(CCSizeZero)
,m_bUpdateImage(false)
,pTextHeight(0)
,m_bFitFlag(false)
,m_iLineSpacing(0)
,m_bWordWrap(true)
,m_bBold(false)
,m_bItalics(false)
, m_bUnderLine(false)
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
	int fontHeight = CAImage::getFontHeight(m_nfontName.c_str(), m_nfontSize);
 
    unsigned int linenumber = (int)this->getBounds().size.height / fontHeight;

    CCSize size = CCSizeZero;
    if (m_bFitFlag)
    {
        float width = CAImage::getStringWidth(m_nfontName.c_str(), m_nfontSize, m_nText);
        if (width > m_obContentSize.width)
        {
            if (m_nNumberOfLine > 1)
            {
				size = CCSize(this->getBounds().size.width, (m_iLineSpacing + fontHeight) * m_nNumberOfLine);
            }
            else if (m_nNumberOfLine == 1)
            {
				size = CCSize(width, fontHeight);
            }
            else
            {
                size.width = this->getBounds().size.width;
				size.height = CAImage::getStringHeight(m_nfontName.c_str(), m_nfontSize, m_nText, size.width, m_iLineSpacing, m_bWordWrap);
            }
        }
        else
        {
            size.height = fontHeight;
			size.width = width;
        }
    }
    else
    {
        if (linenumber == 0)
		{
			size = this->getBounds().size;
		}
		else
		{
			if (m_nNumberOfLine > 0)
			{
				size = CCSize(this->getBounds().size.width, (m_iLineSpacing + fontHeight) * MIN(m_nNumberOfLine, linenumber));
			}
			else
			{
				size = CCSize(this->getBounds().size.width, (m_iLineSpacing + fontHeight) * linenumber);
			}
		}
    }
    
    
    
	CAImage* image = CAImage::createWithString(m_nText.c_str(),
                                               m_nfontName.c_str(),
                                               m_nfontSize,
                                               size,
                                               m_nTextAlignment,
											   m_nVerticalTextAlignmet, 
											   m_bWordWrap, 
											   m_iLineSpacing, 
											   m_bBold, 
											   m_bItalics,
											   m_bUnderLine);

	CC_RETURN_IF(image == NULL);

    m_cLabelSize = size;
    
    CCRect rect = CCRectZero;
    rect.size.width = this->getBounds().size.width;
    rect.size.height = size.height;
    
    float width = m_bFitFlag ? image->getContentSize().width : MIN(this->getBounds().size.width, image->getContentSize().width);
    
    rect.size.width = width;
    
    this->setImage(image);

    switch (m_nVerticalTextAlignmet)
    {
        case CAVerticalTextAlignmentTop:
            pTextHeight = 0;
            break;
            
        case CAVerticalTextAlignmentCenter:
            pTextHeight = (this->getBounds().size.height - rect.size.height) / 2;
            break;
            
        case CAVerticalTextAlignmentBottom:
            pTextHeight = this->getBounds().size.height - rect.size.height;
            break;
            
        default:
            break;
    }

    if (m_bFitFlag)
    {
        this->setImageRect(rect, false, size);
    }
    else
    {
        this->setImageRect(rect);
    }
}

void CALabel::updateImageRect()
{
    GLfloat x1,x2,y1,y2;
    x1 = 0;
    y1 = 0;
    y1 = m_obContentSize.height - m_obRect.size.height - y1;
    y1 = y1 - pTextHeight;
    x2 = x1 + m_obRect.size.width;
    y2 = y1 + m_obRect.size.height;
    m_sQuad.bl.vertices = vertex3(x1, y1, 0);
    m_sQuad.br.vertices = vertex3(x2, y1, 0);
    m_sQuad.tl.vertices = vertex3(x1, y2, 0);
    m_sQuad.tr.vertices = vertex3(x2, y2, 0);
}

void CALabel::setDimensions(const CCSize& var)
{
    m_nDimensions = var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

const CCSize& CALabel::getDimensions()
{
    return m_nDimensions;
}

void CALabel::sizeToFit()
{
    m_bFitFlag = true;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

void CALabel::setText(const string& var)
{
    CC_RETURN_IF(m_nText.compare(var) == 0);
    m_nText = var;
    m_bUpdateImage = true;
}

void CALabel::setTextAlignment(const CATextAlignment& var)
{
    m_nTextAlignment = var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

const CATextAlignment& CALabel::getTextAlignment()
{
    return m_nTextAlignment;
}

const string& CALabel::getText()
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

void CALabel::setLineSpacing(unsigned int var)
{
    CC_RETURN_IF(m_iLineSpacing == var);
	m_iLineSpacing = var;
	m_bUpdateImage = true;
}

unsigned int CALabel::getLineSpacing()
{
	return m_iLineSpacing;
}

void CALabel::setWordWrap(bool var)
{
	m_bWordWrap = var;
	m_bUpdateImage = true;
}

bool CALabel::getWordWrap()
{
	return m_bWordWrap;
}

void CALabel::setBold(bool var)
{
	m_bBold = var;
	m_bUpdateImage = true;
}

bool CALabel::getBold()
{
	return m_bBold;
}

void CALabel::setUnderLine(bool var)
{
	m_bUnderLine = var;
	m_bUpdateImage = true;
}

bool CALabel::getUnderLine()
{
	return m_bUnderLine;
}

void CALabel::setItalics(bool var)
{
	m_bItalics = var;
	m_bUpdateImage = true;
}

bool CALabel::getItalics()
{
	return m_bItalics;
}

void CALabel::setFontName(const string& var)
{
    m_nfontName=var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

const std::string& CALabel::getFontName()
{
    return m_nfontName;
}

void CALabel::setVerticalTextAlignmet(const CAVerticalTextAlignment& var)
{
    m_nVerticalTextAlignmet=var;
    if(m_nText.empty())
    {
        return;
    }
    m_bUpdateImage = true;
}

const CAVerticalTextAlignment& CALabel::getVerticalTextAlignmet()
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
    if (m_bUpdateImage)
    {
        m_bUpdateImage = false;
        this->updateImage();
    }
    CAView::visit();
}

NS_CC_END