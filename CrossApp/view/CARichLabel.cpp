#include "CARichLabel.h"
#include "ccMacros.h"
#include "platform/CAFontProcesstor.h"
#include "device/CADevice.h"
NS_CC_BEGIN

CARichLabel::Element::Element(const std::string& text, const CAFont& font)
: text(text)
, font(font)
{

}

///////////////////////////////////////////////////
CARichLabel::CARichLabel()
: m_bUpdateImage(false)
, m_eTextAlignment(CATextAlignment::Left)
, m_eVerticalTextAlignment(CAVerticalTextAlignment::Top)
, m_iNumberOfLine(0)
, m_obPadding(DSizeZero)
, m_obLabelSize(DSizeZero)
, m_bFitFlag(false)
{
}

CARichLabel::~CARichLabel()
{
}

CARichLabel* CARichLabel::create()
{
	CARichLabel *label = new CARichLabel();
	if (label && label->init())
	{
		label->autorelease();
		return label;
	}
	CC_SAFE_DELETE(label);
	return NULL;
}

CARichLabel* CARichLabel::createWithFrame(const DRect& rect)
{
	CARichLabel *label = new CARichLabel();
	if (label && label->initWithFrame(rect))
	{
		label->autorelease();
		return label;
	}
	CC_SAFE_DELETE(label);
	return NULL;
}

CARichLabel* CARichLabel::createWithCenter(const DRect& rect)
{
	CARichLabel *label = new CARichLabel();
	if (label && label->initWithCenter(rect))
	{
		label->autorelease();
		return label;
	}

	CC_SAFE_DELETE(label);
	return NULL;
}

CARichLabel* CARichLabel::createWithLayout(const DLayout& layout)
{
    CARichLabel *label = new CARichLabel();
    if (label && label->initWithLayout(layout))
    {
        label->autorelease();
        return label;
    }
    
    CC_SAFE_DELETE(label);
    return NULL;
}

void CARichLabel::setContentSize(const CrossApp::DSize &var)
{
	if (!m_obContentSize.equals(var))
	{
		CAView::setContentSize(var);
		this->updateImageDraw();
	}
}

void CARichLabel::updateImageRect()
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

void CARichLabel::updateImageDraw()
{
	m_bUpdateImage = true;
	this->updateDraw();
}

void CARichLabel::updateImage()
{
    int fontHeight = CAFontProcesstor::heightForFont(m_vElements.front().font);
    
    unsigned int linenumber = (int)m_obContentSize.height / fontHeight;
    
    DSize size = m_obContentSize;
    if (linenumber > 0)
    {
        if (m_iNumberOfLine > 1)
        {
            linenumber = MIN(m_iNumberOfLine, linenumber);
            size.height = fontHeight * linenumber + m_vElements.front().font.lineSpacing * (linenumber - 1);
        }
        else if (m_iNumberOfLine == 1)
        {
            size.width = m_bFitFlag ? 0xffffffff : m_obContentSize.width;
            size.height = fontHeight;
        }
        else
        {
            size = m_obContentSize;
        }
    }
    CAImage* image = CAFontProcesstor::imageForRichText(m_vElements, size, m_eTextAlignment);
    this->setImage(image);
    
    if (image)
    {
        this->setImage(image);
        
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

void CARichLabel::visitEve()
{
    CAView::visitEve();
	if (m_bUpdateImage)
	{
		m_bUpdateImage = false;
		this->updateImage();
	}
}

void CARichLabel::appendText(const std::string& text, const CAFont& font)
{
    m_vElements.push_back(Element(text, font));
    this->updateImageDraw();
}

void CARichLabel::clear()
{
    m_vElements.clear();
    updateImageDraw();
}

void CARichLabel::setVerticalTextAlignmet(const CAVerticalTextAlignment& var)
{
    m_eVerticalTextAlignment = var;
}

void CARichLabel::setTextAlignment(const CATextAlignment& var)
{
    m_eTextAlignment = var;
}

void CARichLabel::setNumberOfLine(unsigned int var)
{
    m_iNumberOfLine = var;
}

const DSize& CARichLabel::getLabelSize()
{
    return m_obLabelSize;
}

NS_CC_END
