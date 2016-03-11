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
#include "shaders/CAShaderCache.h"
#include "platform/CAClipboard.h"
#include "basics/CAApplication.h"
#include "support/ccUTF8.h"
NS_CC_BEGIN


CALabel::CALabel()
:m_nNumberOfLine(0)
,m_nTextAlignment(CATextAlignmentLeft)
,m_nText("")
,m_nVerticalTextAlignmet(CAVerticalTextAlignmentTop)
,m_nDimensions(DSizeZero)
,m_cLabelSize(DSizeZero)
,pTextHeight(0)
,m_bFitFlag(false)
,m_bUpdateImage(false)
,m_iLineSpacing(0)
,m_bWordWrap(false)
,m_bEnableCopy(false)
{
    m_obContentSize = DSizeZero;
}

CALabel::~CALabel()
{
    
}

CALabel *CALabel::create()
{
    CALabel *label = new CALabel();
    if (label && label->init())
    {
        label->autorelease();
        return label;
    }
    CC_SAFE_DELETE(label);
    return NULL;
}

CALabel *CALabel::createWithFrame(const DRect& rect)
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

CALabel* CALabel::createWithCenter(const DRect &rect)
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
}

bool CALabel::initWithFrame(const DRect& rect)
{
    if (!CAView::initWithFrame(rect))
    {
        return false;
    }
    return true;
}

bool CALabel::initWithCenter(const DRect& rect)
{
    if (!CAView::initWithCenter(rect))
    {
        return false;
    }
    return true;
}

void CALabel::updateImageDraw()
{
    m_bUpdateImage = true;
    this->updateDraw();
}

void CALabel::updateImage()
{
	int fontHeight = CAImage::getFontHeight(m_obFont.fontName.c_str(), m_obFont.fontSize);
	int defaultLineSpace = fontHeight / 4;
 
    unsigned int linenumber = (int)m_obContentSize.height / fontHeight;

    DSize size = DSizeZero;
    if (m_bFitFlag)
    {
        float width = CAImage::getStringWidth(m_obFont.fontName.c_str(), m_obFont.fontSize, m_nText);
        if (width > m_obContentSize.width)
        {
            if (m_nNumberOfLine > 1)
            {
				size = DSize(m_obContentSize.width, (defaultLineSpace + m_iLineSpacing + fontHeight) * m_nNumberOfLine);
            }
            else if (m_nNumberOfLine == 1)
            {
				size = DSize(width, fontHeight);
            }
            else
            {
                size.width = m_obContentSize.width;
				size.height = CAImage::getStringHeight(m_obFont.fontName.c_str(), m_obFont.fontSize, m_nText, size.width, m_iLineSpacing, m_bWordWrap);
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
			size = m_obContentSize;
		}
		else
		{
			if (m_nNumberOfLine > 0)
			{
				size = DSize(m_obContentSize.width, (defaultLineSpace + m_iLineSpacing + fontHeight) * MIN(m_nNumberOfLine, linenumber));
			}
			else
			{
				size = DSize(m_obContentSize.width, (defaultLineSpace + m_iLineSpacing + fontHeight) * linenumber);
			}
		}
    }
    
    
    
	CAImage* image = CAImage::createWithString(m_nText.c_str(),
											   m_obFont.color,
                                               m_obFont.fontName.c_str(),
                                               m_obFont.fontSize,
                                               size,
                                               m_nTextAlignment,
											   m_nVerticalTextAlignmet, 
											   m_bWordWrap, 
											   m_iLineSpacing, 
											   m_obFont.bold,
											   m_obFont.italics,
											   m_obFont.underLine,
											   m_obFont.deleteLine);

    this->setImage(image);
	CC_RETURN_IF(image == NULL);
    m_cLabelSize = size;

    DRect rect = DRectZero;
    rect.size.width = m_obContentSize.width;
    rect.size.height = size.height;
    
    float width = m_bFitFlag ? image->getContentSize().width : MIN(m_obContentSize.width, image->getContentSize().width);
    
    rect.size.width = width;

    switch (m_nVerticalTextAlignmet)
    {
        case CAVerticalTextAlignmentTop:
            pTextHeight = 0;
            break;
            
        case CAVerticalTextAlignmentCenter:
            pTextHeight = (m_obContentSize.height - rect.size.height) / 2;
            break;
            
        case CAVerticalTextAlignmentBottom:
            pTextHeight = m_obContentSize.height - rect.size.height;
            break;
            
        default:
            break;
    }

    if (m_bFitFlag)
    {
        if (!size.equals(m_obContentSize))
        {
            if (m_eLayoutType == 0)
            {
                DRect rect = this->getFrame();
                rect.size = size;
                this->setFrame(rect);
            }
            else
            {
                DRect rect = this->getCenter();
                rect.size = size;
                this->setCenter(rect);
            }
        }
    }
    this->setImageRect(rect);
}

void CALabel::updateImageRect()
{
    GLfloat x1,x2,y1,y2;
    x1 = 0;
    y1 = 0;
    y1 = m_obContentSize.height - m_obRect.size.height - y1;
    y1 = y1 - pTextHeight;
    x2 = x1 + m_obRect.size.width;
    x2 = MAX(x1, x2);
    y2 = y1 + m_obRect.size.height;
    y2 = MAX(y1, y2);
    m_sQuad.bl.vertices = vertex3(x1, y1, m_fVertexZ);
    m_sQuad.br.vertices = vertex3(x2, y1, m_fVertexZ);
    m_sQuad.tl.vertices = vertex3(x1, y2, m_fVertexZ);
    m_sQuad.tr.vertices = vertex3(x2, y2, m_fVertexZ);
}

void CALabel::copySelectText()
{
	CAClipboard::setText(m_nText);
}

void CALabel::ccTouchPress(CATouch *pTouch, CAEvent *pEvent)
{
	if (m_bEnableCopy)
	{
		CATextToolBarView *pToolBar = CATextToolBarView::create();
		pToolBar->addButton(UTF8("\u590d\u5236"), this, callfunc_selector(CALabel::copySelectText));
		pToolBar->show();
	}
}


void CALabel::setDimensions(const DSize& var)
{
    m_nDimensions = var;
    if(m_nText.empty())
    {
        return;
    }
    this->updateImageDraw();
}

const DSize& CALabel::getDimensions()
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
    this->updateImageDraw();
}

void CALabel::unsizeToFit()
{
    m_bFitFlag = false;
}

void CALabel::setText(const string& var)
{
    CC_RETURN_IF(m_nText.compare(var) == 0);
    m_nText = var;
    this->updateImageDraw();
}

void CALabel::setTextAlignment(const CATextAlignment& var)
{
    m_nTextAlignment = var;
    if(m_nText.empty())
    {
        return;
    }
   this->updateImageDraw();
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
    this->updateImageDraw();
}

void CALabel::setFont(const CrossApp::CAFont &var)
{
    m_obFont = var;
    if(m_nText.empty())
    {
        return;
    }
    this->updateImageDraw();
}

const CAFont& CALabel::getFont()
{
    return m_obFont;
}

void CALabel::setFontSize(unsigned int var)
{
    m_obFont.fontSize = var;
    if(m_nText.empty())
    {
        return;
    }
    this->updateImageDraw();
}

unsigned int CALabel::getFontSize()
{
    return m_obFont.fontSize;
}

void CALabel::setFontName(const string& var)
{
    m_obFont.fontName = var;
    if(m_nText.empty())
    {
        return;
    }
    this->updateImageDraw();
}

const std::string& CALabel::getFontName()
{
    return m_obFont.fontName;
}

void CALabel::setBold(bool var)
{
    m_obFont.bold = var;
    this->updateImageDraw();
}

bool CALabel::isBold()
{
    return m_obFont.bold;
}

void CALabel::setUnderLine(bool var)
{
    m_obFont.underLine = var;
    this->updateImageDraw();
}

bool CALabel::isUnderLine()
{
    return m_obFont.underLine;
}

void CALabel::setDeleteLine(bool var)
{
    m_obFont.deleteLine = var;
    this->updateImageDraw();
}

bool CALabel::isDeleteLine()
{
    return m_obFont.deleteLine;
}


void CALabel::setItalics(bool var)
{
    m_obFont.italics = var;
    this->updateImageDraw();
}

bool CALabel::isItalics()
{
    return m_obFont.italics;
}

const CAColor4B& CALabel::getColor(void)
{
    return m_obFont.color;
}

void CALabel::setColor(const CAColor4B& color)
{
    m_obFont.color = color;
    updateImage();
    CAView::setColor(ccc4(255, 255, 255, color.a));
}

void CALabel::setLineSpacing(int var)
{
    CC_RETURN_IF(m_iLineSpacing == var);
	m_iLineSpacing = var;
	this->updateImageDraw();
}

int CALabel::getLineSpacing()
{
	return m_iLineSpacing;
}

void CALabel::setWordWrap(bool var)
{
	m_bWordWrap = var;
	this->updateImageDraw();
}

bool CALabel::isWordWrap()
{
	return m_bWordWrap;
}

void CALabel::setVerticalTextAlignmet(const CAVerticalTextAlignment& var)
{
    m_nVerticalTextAlignmet = var;
    if(m_nText.empty())
    {
        return;
    }
    this->updateImageDraw();
}

const CAVerticalTextAlignment& CALabel::getVerticalTextAlignmet()
{
    return m_nVerticalTextAlignmet;
}

void CALabel::setContentSize(const CrossApp::DSize &var)
{
    DSize size = var;
    size.width = MAX(size.width, m_obFont.fontSize);
    size.height = MAX(size.height, m_obFont.fontSize);
    
    if (!m_obContentSize.equals(size))
    {
        CAView::setContentSize(size);
        this->updateImageDraw();
    }
}

void CALabel::visitEve()
{
    if (m_bUpdateImage)
    {
        m_bUpdateImage = false;
        this->updateImage();
    }
    CAView::visitEve();
}

NS_CC_END