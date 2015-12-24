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
#include "CALabelStyle.h"
#include "shaders/CAShaderCache.h"
#include "platform/CAClipboard.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN


CALabel::CALabel()
:m_nNumberOfLine(0)
,m_nTextAlignment(CATextAlignmentLeft)
,m_nText("")
,m_nfontName("")
,m_nVerticalTextAlignmet(CAVerticalTextAlignmentTop)
,m_nDimensions(DSizeZero)
,m_nfontSize(24)
,m_cLabelSize(DSizeZero)
,m_bUpdateImage(false)
,pTextHeight(0)
,m_bFitFlag(false)
,m_iLineSpacing(0)
,m_bWordWrap(false)
,m_bBold(false)
,m_bItalics(false)
,m_bUnderLine(false)
,m_bEnableCopy(false)
, m_cFontColor(CAColor_black)
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
    if (m_bUpdateImage)
    {
        m_bUpdateImage = false;
        this->updateImage();
    }
}

bool CALabel::initWithFrame(const DRect& rect)
{
    if (!CAView::initWithFrame(rect,CAColor_black))
    {
        return false;
    }
    return true;
}

bool CALabel::initWithCenter(const DRect& rect)
{
    if (!CAView::initWithCenter(rect,CAColor_black))
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
	int fontHeight = CAImage::getFontHeight(m_nfontName.c_str(), m_nfontSize);
	int defaultLineSpace = fontHeight / 4;
 
    unsigned int linenumber = (int)this->getBounds().size.height / fontHeight;

    DSize size = DSizeZero;
    if (m_bFitFlag)
    {
        float width = CAImage::getStringWidth(m_nfontName.c_str(), m_nfontSize, m_nText);
        if (width > m_obContentSize.width)
        {
            if (m_nNumberOfLine > 1)
            {
				size = DSize(this->getBounds().size.width, (defaultLineSpace + m_iLineSpacing + fontHeight) * m_nNumberOfLine);
            }
            else if (m_nNumberOfLine == 1)
            {
				size = DSize(width, fontHeight);
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
				size = DSize(this->getBounds().size.width, (defaultLineSpace + m_iLineSpacing + fontHeight) * MIN(m_nNumberOfLine, linenumber));
			}
			else
			{
				size = DSize(this->getBounds().size.width, (defaultLineSpace + m_iLineSpacing + fontHeight) * linenumber);
			}
		}
    }
    
    
    
	CAImage* image = CAImage::createWithString(m_nText.c_str(),
											   m_cFontColor,
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

    this->setImage(image);
	CC_RETURN_IF(image == NULL);
    m_cLabelSize = size;
    
    DRect rect = DRectZero;
    rect.size.width = this->getBounds().size.width;
    rect.size.height = size.height;
    
    float width = m_bFitFlag ? image->getContentSize().width : MIN(this->getBounds().size.width, image->getContentSize().width);
    
    rect.size.width = width;

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
        if (!size.equals(m_obContentSize))
        {
            if (m_bFrame)
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
    x2 = x1 + m_obRect.size.width - 1;
    x2 = MAX(x1, x2) + 0.5f;
    y2 = y1 + m_obRect.size.height - 1;
    y2 = MAX(y1, y2) + 0.5f;
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

void CALabel::setFontSize(unsigned int var)
{
    m_nfontSize = var;
    if(m_nText.empty())
    {
        return;
    }
    this->updateImageDraw();
}

unsigned int CALabel::getFontSize()
{
    return m_nfontSize;
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

bool CALabel::getWordWrap()
{
	return m_bWordWrap;
}

void CALabel::setBold(bool var)
{
	m_bBold = var;
	this->updateImageDraw();
}

bool CALabel::getBold()
{
	return m_bBold;
}

void CALabel::setUnderLine(bool var)
{
	m_bUnderLine = var;
	this->updateImageDraw();
}

bool CALabel::getUnderLine()
{
	return m_bUnderLine;
}

void CALabel::setItalics(bool var)
{
	m_bItalics = var;
	this->updateImageDraw();
}

bool CALabel::getItalics()
{
	return m_bItalics;
}

void CALabel::setFontName(const string& var)
{
    m_nfontName = var;
    if(m_nText.empty())
    {
        return;
    }
    this->updateImageDraw();
}

const std::string& CALabel::getFontName()
{
    return m_nfontName;
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
    size.width = MAX(size.width, m_nfontSize);
    size.height = MAX(size.height, m_nfontSize);
    
    if (!m_obContentSize.equals(size))
    {
        CAView::setContentSize(size);
        this->updateImageDraw();
    }
}

const CAColor4B& CALabel::getColor(void)
{
	return m_cFontColor;
}

void CALabel::setColor(const CAColor4B& color)
{
	m_cFontColor = color;
	updateImage();
	CAView::setColor(CAColor_white);
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

void CALabel::applyStyle(const string& sStyleName)
{
	const CALabelStyle* pStyle = CALabelStyleCache::sharedStyleCache()->getStyle(sStyleName);
	applyStyle(pStyle);
}

void CALabel::applyStyle(const CALabelStyle* pLabelStyle)
{
	CC_RETURN_IF(!pLabelStyle);

	setFontName(pLabelStyle->getFontName());
	setFontSize(pLabelStyle->getFontSize());
	
//	setColor(pLabelStyle->getFontColor());
	setLineSpacing(pLabelStyle->getLineSpace());
	setBold(pLabelStyle->isBold());
	setItalics(pLabelStyle->isItalics());
	setWordWrap(pLabelStyle->isWordWrap());
}

NS_CC_END