#include "CARichLabel.h"
#include "ccMacros.h"
#include "platform/CAFTRichFont.h"

NS_CC_BEGIN

CAFont::CAFont()
	: m_nfontSize(24)
	, m_cTextColor(CAColor_black)
	, m_bItalics(false)
	, m_bBold(false)
	, m_bUnderLine(false)
	, m_bDeleteLine(false)
{

}

///////////////////////////////////////////////////
CARichLabel::CARichLabel()
	: m_fTextHeight(0)
	, m_bUpdateImage(false)
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

bool CARichLabel::initWithFrame(const DRect& rect)
{
	if (!CAView::initWithFrame(rect))
	{
		return false;
	}
	return true;
}

bool CARichLabel::initWithCenter(const DRect& rect)
{
	if (!CAView::initWithCenter(rect))
	{
		return false;
	}
	return true;
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
	GLfloat x1, x2, y1, y2;
	x1 = 0;
	y1 = 0;
	y1 = m_obContentSize.height - m_obRect.size.height - y1;
	y1 = y1 - m_fTextHeight;
	x2 = x1 + m_obRect.size.width;
	x2 = MAX(x1, x2);
	y2 = y1 + m_obRect.size.height;
	y2 = MAX(y1, y2);
	m_sQuad.bl.vertices = vertex3(x1, y1, m_fVertexZ);
	m_sQuad.br.vertices = vertex3(x2, y1, m_fVertexZ);
	m_sQuad.tl.vertices = vertex3(x1, y2, m_fVertexZ);
	m_sQuad.tr.vertices = vertex3(x2, y2, m_fVertexZ);
}

void CARichLabel::updateImageDraw()
{
	m_bUpdateImage = true;
	this->updateDraw();
}

float CARichLabel::getMaxFontHeight()
{
	std::string cszFontName;
	int iFontSize = 0;

	for (int i = 0; i < m_vLabelFontVect.size(); i++)
	{
		const CAFont& ft = m_vLabelFontVect[i].second;
		int tFSize = ft.getFontSize();
		if (tFSize > iFontSize)
		{
			iFontSize = tFSize;
			cszFontName = ft.getFontName();
		}
	}
	return CAImage::getFontHeight(cszFontName.c_str(), iFontSize);
}

void CARichLabel::updateImage()
{
	int fontHeight = getMaxFontHeight();
	int defaultLineSpace = fontHeight / 5;

	DSize size;
	unsigned int linenumber = (int)m_obContentSize.height / fontHeight;
	if (linenumber == 0)
	{
		size = m_obContentSize;
	}
	else
	{
		size = DSize(m_obContentSize.width, (defaultLineSpace + fontHeight) * (linenumber+1));
	}

	CAImage* image = g_AFTRichFont.initWithString(m_vLabelFontVect, size);
	this->setImage(image);
	CC_RETURN_IF(image == NULL);

	DRect rect = DRectZero;
	rect.size.width = MIN(m_obContentSize.width, image->getContentSize().width);
	rect.size.height = m_obContentSize.height;

	this->setImageRect(rect);
}

void CARichLabel::visitEve()
{
	if (m_bUpdateImage)
	{
		m_bUpdateImage = false;
		this->updateImage();
	}
	CAView::visitEve();
}

bool CARichLabel::init()
{
	if (!CAView::init())
	{
		return false;
	}
	CAView::setColor(ccc4(255, 255, 255, 255));
	return true;
}

void CARichLabel::appendText(const std::string& cszText)
{
	m_vLabelFontVect.push_back(std::make_pair(cszText, CAFont()));
	updateImageDraw();
}

void CARichLabel::appendText(const std::string& text, const CAFont& font)
{
	m_vLabelFontVect.push_back(std::make_pair(text, font));
	updateImageDraw();
}





NS_CC_END