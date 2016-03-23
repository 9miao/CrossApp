#include "CARichLabel.h"
#include "ccMacros.h"
#include "platform/CAFTRichFont.h"
#include "support/deelx.h"
#include "support/device/CADevice.h"

NS_CC_BEGIN

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

CARichLabel* CARichLabel::createWithLayout(const DRectLayout& layout)
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

bool CARichLabel::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	return true;
}

void CARichLabel::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	DPoint point = this->convertTouchToNodeSpace(pTouch);

	for (int i = 0; i < m_vHyperlinkRects.size(); i++)
	{
		for (int j = 0; j < m_vHyperlinkRects[i].size(); j++)
		{
			if (m_vHyperlinkRects[i][j].containsPoint(point))
			{
				CADevice::OpenURL(m_vLabelUrls[i]);
				setUrlVisited(m_vLabelUrls[i]);
				return;
			}
		}
	}
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
		int tFSize = ft.fontSize;
		if (tFSize > iFontSize)
		{
			iFontSize = tFSize;
			cszFontName = ft.fontName;
		}
	}
	return CAImage::getFontHeight(cszFontName.c_str(), iFontSize);
}

void CARichLabel::splitUrlStrings(const std::string& text, std::vector<std::pair<int, int>>& vIntVect)
{
	static CRegexpT<char> regexp("((http|ftp|https)://)(([a-zA-Z0-9\\._-]+\\.[a-zA-Z]{2,6})|([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}))(:[0-9]{1,4})*(/[a-zA-Z0-9\\&%_\\./-~-]*)?");

	CContext* pContext = regexp.PrepareMatch(text.c_str());

	MatchResult result = regexp.Match(pContext);

	// matched or not
	while (result.IsMatched())
	{
		vIntVect.push_back(std::make_pair(result.GetStart(), result.GetEnd()));
		// get next
		result = regexp.Match(pContext);
	}

	regexp.ReleaseContext(pContext);
}

void CARichLabel::appendTextEx(const std::string& text, const CAFont& font)
{
	std::vector<std::pair<int, int>> vIntVect;
	splitUrlStrings(text, vIntVect);

	if (vIntVect.empty())
	{
		m_vLabelFontVect.push_back(std::make_pair(text, font));
	}
	else
	{
		int ipos = 0;
		for (int i = 0; i < vIntVect.size(); i++)
		{
			const std::pair<int, int>& pa = vIntVect[i];

			std::string cszStrTemp = text.substr(ipos, pa.first - ipos);
			if (!cszStrTemp.empty())
			{
				m_vLabelFontVect.push_back(std::make_pair(cszStrTemp, font));
			}

			cszStrTemp = text.substr(pa.first, pa.second - pa.first);
			if (!cszStrTemp.empty())
			{
				CAFont t(font);
				t.hyperlink = 1;
				m_vLabelFontVect.push_back(std::make_pair(cszStrTemp, t));
				m_vLabelUrls.push_back(cszStrTemp);
			}

			ipos = pa.second;
		}

		if (ipos < text.size())
		{
			std::string cszStrTemp = text.substr(ipos, text.size() - ipos);
			if (!cszStrTemp.empty())
			{
				m_vLabelFontVect.push_back(std::make_pair(cszStrTemp, font));
			}
		}
	}
	updateImageDraw();
}

void CARichLabel::setUrlVisited(const std::string& cszUrl)
{
	for (int i = 0; i < m_vLabelFontVect.size(); i++)
	{
		if (m_vLabelFontVect[i].first == cszUrl)
		{
			m_vLabelFontVect[i].second.hyperlink = 2;
		}
	}
	updateImageDraw();
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

	m_vHyperlinkRects.clear();
	CAImage* image = g_AFTRichFont.initWithString(m_vLabelFontVect, size, m_vHyperlinkRects);
	this->setImage(image);
	CC_RETURN_IF(image == NULL);
	CC_RETURN_IF(m_vLabelUrls.size() != m_vHyperlinkRects.size());

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

void CARichLabel::appendText(const std::string& text)
{
	appendTextEx(text, CAFont());
}

void CARichLabel::appendText(const std::string& text, const CAFont& font)
{
	appendTextEx(text, font);
}

void CARichLabel::clear()
{
    m_vLabelFontVect.clear();
    updateImageDraw();
}



NS_CC_END