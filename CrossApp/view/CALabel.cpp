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
#include "renderer/CCGLProgramState.h"
#include "platform/CAClipboard.h"
#include "basics/CAApplication.h"
#include "support/ccUTF8.h"
#include "platform/CAFontProcesstor.h"

NS_CC_BEGIN


CALabel::CALabel()
:m_nNumberOfLine(0)
,m_sText("")
,m_obLabelSize(DSizeZero)
,m_obPadding(DSizeZero)
,m_bFitFlag(false)
,m_bUpdateImage(false)
,m_bEnableCopy(false)
,m_eTextAlignment(CATextAlignment::Left)
,m_eVerticalTextAlignment(CAVerticalTextAlignment::Top)
{
    m_obContentSize = DSizeZero;
}

CALabel::~CALabel()
{
    
}

bool CALabel::init()
{
    if (!CAView::init())
    {
        return false;
    }
    
    this->onTouchBegan([=](CATouch *pTouch, CAEvent *pEvent)
    {
        CAScheduler::getScheduler()->scheduleOnce([&](float de)
        {
            if (m_bEnableCopy)
            {
                CATextToolBarView *pToolBar = CATextToolBarView::create();
                pToolBar->addButton(UTF8("\u590d\u5236"), std::bind(&CALabel::copySelectText, this));
                pToolBar->show();
            }
        }, "function", this, 1.0f);
        return true;
    });
    
    this->onTouchMoved([=](CATouch *pTouch, CAEvent *pEvent)
    {
        CAScheduler::getScheduler()->unschedule("function", this);
    });
    
    this->onTouchEnded([=](CATouch *pTouch, CAEvent *pEvent)
    {
        CAScheduler::getScheduler()->unschedule("function", this);
    });
    
    this->onTouchCancelled([=](CATouch *pTouch, CAEvent *pEvent)
    {
        CAScheduler::getScheduler()->unschedule("function", this);
    });
    
    
    return true;
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

CALabel* CALabel::createWithLayout(const CrossApp::DLayout &layout)
{
    CALabel *label = new CALabel();
    if (label && label->initWithLayout(layout))
    {
        label->autorelease();
        return label;
    }
    
    CC_SAFE_DELETE(label);
    return NULL;
}

void CALabel::updateImageDraw()
{
    if (!m_sText.empty() && m_bUpdateImage == false)
    {
        m_bUpdateImage = true;
        this->updateDraw();
    }
}

void CALabel::updateImage()
{
    int fontHeight = CAFontProcesstor::heightForFont(m_obFont);
    if (fontHeight == 0)
        return;
    unsigned int linenumber = (int)m_obContentSize.height / fontHeight;

    DSize size = m_obContentSize;
    if (linenumber > 0)
    {
        if (m_nNumberOfLine > 1)
        {
            linenumber = MIN(m_nNumberOfLine, linenumber);
            size.height = fontHeight * linenumber + m_obFont.lineSpacing * (linenumber - 1);
        }
        else if (m_nNumberOfLine == 1)
        {
            size.width = m_bFitFlag ? 0xffffffff : m_obContentSize.width;
            size.height = fontHeight;
        }
        else
        {
            size = m_obContentSize;
        }
    }

    CAImage* image = CAFontProcesstor::imageForText(m_sText, m_obFont, size, m_eTextAlignment);
    
    this->setImage(image);

    if (image)
    {
        m_obLabelSize = size;
        
        switch (m_eTextAlignment)
        {
            case CATextAlignment::Left:
                m_obPadding.width = 0;
                break;

            case CATextAlignment::Center:
                m_obPadding.width = (m_obContentSize.width - m_obLabelSize.width) / 2;
                break;
                
            case CATextAlignment::Right:
                m_obPadding.width = m_obContentSize.width - m_obLabelSize.width;
                break;
                
            default:
                break;
        }
        
        switch (m_eVerticalTextAlignment)
        {
            case CAVerticalTextAlignment::Top:
                m_obPadding.height = 0;
                break;
                
            case CAVerticalTextAlignment::Center:
                m_obPadding.height = (m_obContentSize.height - m_obLabelSize.height) / 2;
                break;
                
            case CAVerticalTextAlignment::Bottom:
                m_obPadding.height = m_obContentSize.height - m_obLabelSize.height;
                break;
                
            default:
                break;
        }
        
        if (m_bFitFlag && !m_obLabelSize.equals(m_obContentSize))
        {
            if (m_eLayoutType == 0)
            {
                DRect rect = this->getFrame();
                rect.size = m_obLabelSize;
                this->setFrame(rect);
            }
            else if (m_eLayoutType == 1)
            {
                DRect rect = this->getCenter();
                rect.size = m_obLabelSize;
                this->setCenter(rect);
            }
        }
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        this->setImageRect(DRect(DPointZero, size));
#else
        this->setImageRect(DRect(DPointZero, image->getContentSize()));
#endif
        
    }
    else
    {
        this->setImageRect(DRectZero);
    }
}

void CALabel::updateImageRect()
{
    GLfloat x1,x2,y1,y2;
    x1 = m_obPadding.width;
    y1 = 0;
    y1 = m_obContentSize.height - m_obLabelSize.height - y1;
    y1 = y1 - m_obPadding.height;
    x2 = x1 + m_obLabelSize.width;
    x2 = MAX(x1, x2);
    y2 = y1 + m_obLabelSize.height;
    y2 = MAX(y1, y2);
    m_sQuad.bl.vertices = DPoint3D(x1, y1, m_fPointZ);
    m_sQuad.br.vertices = DPoint3D(x2, y1, m_fPointZ);
    m_sQuad.tl.vertices = DPoint3D(x1, y2, m_fPointZ);
    m_sQuad.tr.vertices = DPoint3D(x2, y2, m_fPointZ);
}

void CALabel::copySelectText()
{
	CAClipboard::setText(m_sText);
}

void CALabel::sizeToFit()
{
    m_bFitFlag = true;
    this->updateImageDraw();
}

void CALabel::unsizeToFit()
{
    m_bFitFlag = false;
}

void CALabel::setText(const string& var)
{
    CC_RETURN_IF(m_sText.compare(var) == 0);
    m_sText = var;
    this->updateImageDraw();
}

void CALabel::setTextAlignment(const CATextAlignment& var)
{
    CC_RETURN_IF(m_eTextAlignment == var);
    m_eTextAlignment = var;
    this->updateImageDraw();
}

void CALabel::setVerticalTextAlignmet(const CAVerticalTextAlignment& var)
{
    CC_RETURN_IF(m_eVerticalTextAlignment == var);
    m_eVerticalTextAlignment = var;
    this->updateImageDraw();
}

const string& CALabel::getText()
{
    return m_sText;
}

unsigned int CALabel::getNumberOfLine()
{
    return m_nNumberOfLine;
}

void CALabel::setNumberOfLine(unsigned int var)
{
    m_nNumberOfLine = var;
    this->updateImageDraw();
}

void CALabel::setFont(const CrossApp::CAFont &var)
{
    m_obFont = var;
    this->updateImageDraw();
}

const CAFont& CALabel::getFont()
{
    return m_obFont;
}

void CALabel::setFontSize(float var)
{
    CC_RETURN_IF(m_obFont.fontSize == var);
    m_obFont.fontSize = var;
    this->updateImageDraw();
}

void CALabel::setFontName(const string& var)
{
    CC_RETURN_IF(m_obFont.fontName == var);
    m_obFont.fontName = var;
    this->updateImageDraw();
}

void CALabel::setBold(bool var)
{
    CC_RETURN_IF(m_obFont.bold == var);
    m_obFont.bold = var;
    this->updateImageDraw();
}

void CALabel::setUnderLine(bool var)
{
    CC_RETURN_IF(m_obFont.underLine == var);
    m_obFont.underLine = var;
    this->updateImageDraw();
}

void CALabel::setDeleteLine(bool var)
{
    CC_RETURN_IF(m_obFont.deleteLine == var);
    m_obFont.deleteLine = var;
    this->updateImageDraw();
}

void CALabel::setItalics(bool var)
{
    CC_RETURN_IF(m_obFont.italics == var);
    m_obFont.italics = var;
    this->updateImageDraw();
}

const CAColor4B& CALabel::getColor(void)
{
    return m_obFont.color;
}

void CALabel::setColor(const CAColor4B& color)
{
    m_obFont.color = color;
    this->updateImageDraw();
    CAView::setColor(CAColor4B(255, 255, 255, color.a));
}

void CALabel::setLineSpacing(float var)
{
    CC_RETURN_IF(m_obFont.lineSpacing == var);
	m_obFont.lineSpacing = var;
	this->updateImageDraw();
}

void CALabel::setWordWrap(bool var)
{
	m_obFont.wordWrap = var;
	this->updateImageDraw();
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
    CAView::visitEve();
    if (m_bUpdateImage)
    {
        m_bUpdateImage = false;
        this->updateImage();
    }
}

NS_CC_END
