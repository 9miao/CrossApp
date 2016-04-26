//
//  CAtextView.cpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#include "platform/CATextView.h"
#include "platform/CADensityDpi.h"
#include "platform/CAFreeTypeFont.h"
#include "platform/CAFTFontCache.h"
#include "platform/CAClipboard.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#include "dispatcher/CAIMEDelegate.h"
#include "view/CAScrollView.h"
#include "shaders/CAShaderCache.h"
#include "support/ccUTF8.h"

NS_CC_BEGIN


class CATextViewWin32 : public CAView, public CAIMEDelegate
{
	friend class CATextView;
public:
	CATextViewWin32(CATextView* e)
		: m_pCATextView(e)
		, m_pContainerView(NULL)
		, m_pCursorMark(NULL)
		, m_pImageView(NULL)
		, m_iCurPos(0)
		, m_curSelCharRange(std::make_pair(0, 0))
		, m_iStartMovePos(0)
		, m_bCallbackTextChanged(true)
	{
		m_hEditCur = LoadCursor(NULL, IDC_IBEAM);
		updateFontSize();
	}

	virtual ~CATextViewWin32()
	{
		DestroyCursor(m_hEditCur);
	}

	bool init()
	{
		if (!CAView::init())
		{
			return false;
		}

		this->setColor(CAColor_clear);
		m_pContainerView = CAScrollView::createWithFrame(DRectZero);
		m_pContainerView->setShowsHorizontalScrollIndicator(false);
		m_pContainerView->setHorizontalScrollEnabled(false);
		m_pContainerView->setBounceHorizontal(false);
		m_pContainerView->setHaveNextResponder(true);
		m_pContainerView->setBackgroundColor(CAColor_clear);
		this->addSubview(m_pContainerView);

		m_pImageView = new CAImageView();
		m_pImageView->setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureA8Color));
		m_pContainerView->addSubview(m_pImageView);
		return true;
	}

	void setContentSize(const DSize& var)
	{
		CAView::setContentSize(var);

		if (m_pContainerView)
		{
			m_pContainerView->setFrame(this->getBounds());
		}
		this->initMarkSprite();
	}

	void insertText(const char * text, int len)
	{
		if (!strcmp(text, "\n"))
		{
			if (m_pCATextView->getReturnType() != CrossApp::CATextView::Default)
			{
				m_pCATextView->resignFirstResponder();
				if (m_pCATextView->getDelegate())
				{
					m_pCATextView->getDelegate()->textViewShouldReturn(m_pCATextView);
				}
				return;
			}
		}

		std::string cszAddText;
		cszAddText.append(text, len);
		if (m_curSelCharRange.first != m_curSelCharRange.second)
		{
			if (!textValueChanged(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first, cszAddText))
				return;
		}
		else
		{
			if (!textValueChanged(m_iCurPos, 0, cszAddText))
				return;
		}

		execCurSelCharRange();
		m_szText.insert(m_iCurPos, text, len);
		m_iCurPos += len;
		m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);
		updateImage();
	}
	void clearText()
	{
		HideTextViewMask();

		m_vLinesTextView.clear();
		m_curSelCharRange.first = m_curSelCharRange.second = 0;
		m_iCurPos = 0;
		m_szText.clear();
		m_iStartMovePos = 0;
		
		calcCursorPosition();
		m_pCurPosition = m_pCursorMark->getCenterOrigin();
	}
	void deleteBackward()
	{
		CC_RETURN_IF(m_iCurPos == 0 || m_szText.empty());

		if (m_curSelCharRange.first != m_curSelCharRange.second)
		{
			if (!textValueChanged(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first, ""))
				return;

			CC_RETURN_IF(execCurSelCharRange());
		}
		else
		{
			int nDeleteLen = 1;
			while (0x80 == (0xC0 & m_szText.at(m_iCurPos - nDeleteLen)))
			{
				++nDeleteLen;
			}

			if (!textValueChanged(m_iCurPos, nDeleteLen, ""))
				return;

			m_iCurPos = MAX(m_iCurPos, nDeleteLen);
			m_szText.erase(m_iCurPos - nDeleteLen, nDeleteLen);
			m_iCurPos -= nDeleteLen;

			updateImage();
		}
	}
	void copyToClipboard()
	{
		std::string cszText = m_szText.substr(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first);
		CAClipboard::setText(cszText);
	}
	void cutToClipboard()
	{
		copyToClipboard();

		execCurSelCharRange();
	}
	void pasteFromClipboard()
	{
		std::string cszText = CAClipboard::getText();
		insertText(cszText.c_str(), (int)cszText.size());
	}
	bool canAttachWithIME() { return true; }
	bool canDetachWithIME() { return true; }
	void cursorMoveBackward()
	{
		if (m_iCurPos == 0 || m_szText.empty())
			return;

		int nMoveLen = 1;
		while (0x80 == (0xC0 & m_szText.at(m_iCurPos - nMoveLen)))
		{
			++nMoveLen;

		}
		m_iCurPos -= nMoveLen;

		m_curSelCharRange.first = m_curSelCharRange.second = 0;
		calcCursorPosition();
		m_pCurPosition = m_pCursorMark->getCenterOrigin();
	}
	void cursorMoveForward()
	{
		if (m_iCurPos == m_szText.length() || m_szText.empty())
			return;

		int nMoveLen = 1;
		for (std::string::size_type n = m_iCurPos + nMoveLen; n < m_szText.length() && 0x80 == (0xC0 & m_szText.at(n));)
		{
			++nMoveLen;
			n = m_iCurPos + nMoveLen;
		}
		m_iCurPos += nMoveLen;

		m_curSelCharRange.first = m_curSelCharRange.second = 0;
		calcCursorPosition();
		m_pCurPosition = m_pCursorMark->getCenterOrigin();
	}
	void cursorMoveUp()
	{
		if (m_iCurPos == 0 || m_szText.empty())
			return;

		DPoint cursorRect = m_pCursorMark->getCenterOrigin();
		float scrollViewOffSetY = m_pContainerView->getContentOffset().y;
		DPoint point = DPoint(m_pCurPosition.x, cursorRect.y - m_iLineHeight - scrollViewOffSetY);

		int iCurLine = 0; int iCurPosX = 0;
		calculateSelChars(point, iCurLine, iCurPosX, m_iCurPos);
		m_pCursorMark->setCenterOrigin(DPoint(iCurPosX, m_iLineHeight*1.25f*iCurLine + m_iLineHeight / 2));
		showCursorMark();

		m_curSelCharRange = std::pair<int, int>(m_iCurPos, m_iCurPos);
		calcCursorPosition();
	}

	void cursorMoveDown()
	{
		if (m_szText.empty() || m_iCurPos == m_szText.size())
			return;

		DPoint cursorRect = m_pCursorMark->getCenterOrigin();
		float scrollViewOffSetY = m_pContainerView->getContentOffset().y;
		DPoint point = DPoint(m_pCurPosition.x, cursorRect.y + m_iLineHeight - scrollViewOffSetY);

		int iCurLine = 0; int iCurPosX = 0;
		calculateSelChars(point, iCurLine, iCurPosX, m_iCurPos);
		m_pCursorMark->setCenterOrigin(DPoint(iCurPosX, m_iLineHeight*1.25f*iCurLine + m_iLineHeight / 2));
		showCursorMark();

		m_curSelCharRange = std::pair<int, int>(m_iCurPos, m_iCurPos);
		calcCursorPosition();
	}
	void initMarkSprite()
	{
		if (m_pCursorMark == NULL)
		{
			m_pCursorMark = CAView::create();
			m_pCursorMark->setColor(ccc4(0, 0, 0, 255));
			m_pContainerView->addSubview(m_pCursorMark);
			this->hideCursorMark();
		}
		m_pCursorMark->setFrame(DRect(0, 0, 1.5f, m_iLineHeight));
	}
	void showCursorMark()
	{
		if (!m_pCursorMark->isVisible())
		{
			m_pCursorMark->setVisible(true);

			m_pCursorMark->setAlpha(0);
			CAViewAnimation::beginAnimations(m_s__StrID, NULL);
			CAViewAnimation::setAnimationDuration(0.5f);
			CAViewAnimation::setAnimationRepeatAutoreverses(true);
			CAViewAnimation::setAnimationRepeatCount(1048576);
			m_pCursorMark->setAlpha(1.0f);
			CAViewAnimation::commitAnimations();
		}
	}
	void hideCursorMark()
	{
		m_pCursorMark->setVisible(false);
		CAViewAnimation::removeAnimations(m_s__StrID);
	}
	
	void setText(const std::string &var)
	{
		m_bCallbackTextChanged = false;
		clearText();

		if (var.empty())
		{
			updateImage();
		}
		else
		{
			insertText(var.c_str(), (int)var.length());
		}
		m_bCallbackTextChanged = true;
	}
	const std::string& getText()
	{
		return m_szText;
	}
	void updateFontSize()
	{
		m_iLineHeight = CAImage::getFontHeight(m_szFontName.c_str(), m_pCATextView->getTextFontSize());
	}
	
	bool execCurSelCharRange()
	{
		if (m_curSelCharRange.first == m_curSelCharRange.second)
			return false;

		int iOldCurPos = m_curSelCharRange.first;
		std::string cszText = m_szText.erase(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first);
		setText(cszText);

		m_iCurPos = iOldCurPos;
		this->showCursorMark();
		calcCursorPosition();
		HideTextViewMask();
		return true;
	}
	void updateImage()
	{
		float width = this->getBounds().size.width;

		CAImage* image = g_AFTFontCache.initWithString(m_szText.c_str(),
			m_pCATextView->getTextColor(),
			m_szFontName.c_str(),
			m_pCATextView->getTextFontSize(),
			width,
			0,
			CATextAlignmentLeft,
			CAVerticalTextAlignmentTop,
			true,
			0,
			false,
			false,
			false,
			false,
			&m_vLinesTextView);

		if (m_szText.empty())
		{
			m_vLinesTextView.clear();
		}
		m_pImageView->setColor(CAColor_white);
		m_pImageView->setImage(image);
		DRect rect = DRectZero;

		if (image)
		{
			rect.size = image->getContentSize();
		}

		m_pImageView->setImageRect(rect);
		m_pContainerView->setViewSize(rect.size);
		m_pImageView->setFrame(rect);

		calcCursorPosition();
		m_pCurPosition = m_pCursorMark->getCenterOrigin();
	}
	bool textValueChanged(int location, int length, const std::string& changedText)
	{
		if (m_bCallbackTextChanged && m_pCATextView->getDelegate())
		{
			std::u16string c1, c2;
			StringUtils::UTF8ToUTF16(m_szText.substr(0, location), c1);
			StringUtils::UTF8ToUTF16(m_szText.substr(location, length), c2);

			return m_pCATextView->getDelegate()->textViewShouldChangeCharacters(m_pCATextView, c1.size(), c2.size(), changedText);
		}
		return true;
	}
	void calcCursorPosition()
	{
		int iCurLine = -1, iCurLineCharPos = 0;

		for (unsigned i = 0; i < m_vLinesTextView.size(); i++)
		{
			TextViewLineInfo& t = m_vLinesTextView[i];

			if (m_iCurPos >= t.iStartCharPos && m_iCurPos <= t.iEndCharPos)
			{
				iCurLine = i; iCurLineCharPos = t.iStartCharPos;
				break;
			}
		}

		float fHalfLineHeight = m_iLineHeight / 2.0f;
		float fLineSpaceValue = m_iLineHeight / 4.0f;

		DPoint cCurPosition;
		if (iCurLine == -1)
		{
			cCurPosition.x = 0;
			cCurPosition.y = fHalfLineHeight;
		}
		else
		{
			std::string s = m_szText.substr(iCurLineCharPos, m_iCurPos - iCurLineCharPos);
			if (!s.empty() && s[0] == '\n')
			{
				s.erase(0, 1);
			}
			cCurPosition.x = getStringLength(s);
			cCurPosition.y = (m_iLineHeight + fLineSpaceValue)*iCurLine + fHalfLineHeight;
		}

		if (m_pCursorMark)
		{
			m_pCursorMark->setCenterOrigin(cCurPosition);
		}

		float h = getBounds().size.height;
		float y = cCurPosition.y - m_pContainerView->getContentOffset().y;
		if (y < 0 || y > h)
		{
			y = y < 0 ? fHalfLineHeight : (h - fHalfLineHeight);
			m_pContainerView->setContentOffset(DPoint(0, cCurPosition.y - y), true);
		}
	}
	int getCurrentByPointY(int y)
	{
		int iCurLine = y / (m_iLineHeight*1.25f);
		if (m_vLinesTextView.empty())
		{
			iCurLine = 0;
		}
		else if (iCurLine >= m_vLinesTextView.size())
		{
			iCurLine = (int)m_vLinesTextView.size() - 1;
		}
		return iCurLine;
	}
	void calculateSelChars(const DPoint& point, int& l, int& r, int& p)
	{
		float y = point.y + m_pContainerView->getContentOffset().y;
		int iCurLine = getCurrentByPointY(y);
		int iHalfCharSize = 0;
		int iStartPos = 0;
		if (!m_vLinesTextView.empty())
		{
			p = iStartPos = m_vLinesTextView[iCurLine].iStartCharPos;
			std::vector<TextAttribute>& v = m_vLinesTextView[iCurLine].TextAttrVect;
			for (int i = 0, iStringLeftX = 0; i < v.size(); i++)
			{
				TextAttribute& t = v[i];
				if (point.x >= iStringLeftX - iHalfCharSize && point.x < iStringLeftX + t.charlength / 2)
				{
					break;
				}

				iStringLeftX += t.charlength;
				p += t.charSize;
				iHalfCharSize = t.charlength / 2;
			}
		}

		std::string s = m_szText.substr(iStartPos, p - iStartPos);
		if (!s.empty() && s[0] == '\n')
		{
			s.erase(0, 1);
		}
		l = iCurLine;
		r = getStringLength(s);
	}
	int getStringLength(const std::string &var)
	{
		return g_AFTFontCache.getStringWidth(m_szFontName.c_str(), m_pCATextView->getTextFontSize(), var);
	}
	std::pair<int, int> getLineAndPos(int iPos)
	{
		std::pair<int, int> t;

		t.first = t.second = 0;
		for (int i = 0; i < m_vLinesTextView.size(); i++)
		{
			if (iPos >= m_vLinesTextView[i].iStartCharPos && iPos <= m_vLinesTextView[i].iEndCharPos)
			{
				t.first = i; t.second = m_vLinesTextView[i].iStartCharPos;
				break;
			}
		}
		return t;
	}
	std::vector<DRect> getZZCRect()
	{
		DSize size = getFrame().size;
		std::pair<int, int> t1 = getLineAndPos(m_curSelCharRange.first);
		std::pair<int, int> t2 = getLineAndPos(m_curSelCharRange.second);

		std::string s1 = m_szText.substr(t1.second, m_curSelCharRange.first - t1.second);
		if (!s1.empty() && s1[0] == '\n') s1.erase(0, 1);
		int l1 = getStringLength(s1);

		std::string s2 = m_szText.substr(t2.second, m_curSelCharRange.second - t2.second);
		if (!s2.empty() && s2[0] == '\n') s2.erase(0, 1);
		int l2 = getStringLength(s2);

		std::vector<DRect> vr;
		if (t1.first == t2.first)
		{
			vr.push_back(DRect(l1, m_iLineHeight*1.25f*t1.first, l2 - l1, m_iLineHeight));
		}
		else
		{
			vr.push_back(DRect(l1, m_iLineHeight*1.25f*t1.first, size.width - l1, m_iLineHeight*1.25f));

			int i = t1.first + 1;
			for (; i < t2.first; i++)
			{
				vr.push_back(DRect(0, m_iLineHeight*1.25f*i, size.width, m_iLineHeight*1.25f));
			}
			vr.push_back(DRect(0, m_iLineHeight*1.25f*i, l2, m_iLineHeight));
		}
		return vr;
	}
	void ShowTextViewMask()
	{
		HideTextViewMask();

		int iFlagTrick = -1;
		std::vector<DRect> vt = getZZCRect();
		for (int i = 0; i < vt.size(); i++)
		{
			CAView* pTextMaskView = CAView::createWithColor(ccc4(60, 120, 240, 127));
			if (vt[i].origin.y == iFlagTrick)
			{
				vt[i].InflateRect(0, -1, 0, -1);
			}
			iFlagTrick = vt[i].origin.y + vt[i].size.height;
				
			pTextMaskView->setFrame(vt[i]);
			m_pContainerView->addSubview(pTextMaskView);
			m_vTextViewMask.push_back(pTextMaskView);
		}
	}
	void HideTextViewMask()
	{
		for (int i = 0; i < m_vTextViewMask.size(); i++)
		{
			m_vTextViewMask[i]->removeFromSuperview();
		}
		m_vTextViewMask.clear();
	}
	void StartMouseMove(const DPoint& point)
	{
		CAIMEDelegate::attachWithIME();
		showCursorMark();

		int iCurLine = 0; int iCurPosX = 0;
		calculateSelChars(point, iCurLine, iCurPosX, m_iCurPos);
		m_pCursorMark->setCenterOrigin(DPoint(iCurPosX, m_iLineHeight*1.25f*iCurLine + m_iLineHeight / 2));
		showCursorMark();
		m_pCurPosition = m_pCursorMark->getCenterOrigin();

		m_curSelCharRange = std::pair<int, int>(m_iCurPos, m_iCurPos);
		m_iStartMovePos = m_iCurPos;
		HideTextViewMask();

		::SetCursor(m_hEditCur);
	}
	void MoveMouseArrow(const DPoint& pt)
	{
		int iCurLine = 0; int iCurPosX = 0;
		calculateSelChars(pt, iCurLine, iCurPosX, m_iCurPos);
		m_pCursorMark->setCenterOrigin(DPoint(iCurPosX, m_iLineHeight*1.25f*iCurLine + m_iLineHeight / 2));
		showCursorMark();
		m_pCurPosition = m_pCursorMark->getCenterOrigin();

		m_curSelCharRange.first = MIN(m_iCurPos, m_iStartMovePos);
		m_curSelCharRange.second = MAX(m_iCurPos, m_iStartMovePos);
		ShowTextViewMask();
	}
	void EndMouseMoveArr()
	{
		CAIMEDelegate::detachWithIME();
		this->hideCursorMark();
		HideTextViewMask();
	}

private:
	CATextView* m_pCATextView;

	CAScrollView* m_pContainerView;
	CAView* m_pCursorMark;
	CAImageView* m_pImageView;
	std::vector<CAView*> m_vTextViewMask;

	std::vector<TextViewLineInfo> m_vLinesTextView;
	std::pair<int, int> m_curSelCharRange;
	int m_iCurPos;
	int m_iLineHeight;
	DPoint m_pCurPosition;
	bool m_bCallbackTextChanged;
	std::string m_szFontName;
	std::string m_szText;
	int m_iStartMovePos;
	HCURSOR m_hEditCur;
};


CATextView::CATextView()
: m_pBackgroundView(NULL)
, m_pShowImageView(NULL)
, m_pTextView(NULL)
, m_iFontSize(40)
, m_pDelegate(NULL)
, m_sTextColor(CAColor_black)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
}

CATextView::~CATextView()
{
    CAViewAnimation::removeAnimations(m_s__StrID + "showImage");
}

void CATextView::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextView::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

bool CATextView::resignFirstResponder()
{
	if (m_pDelegate && (!m_pDelegate->textViewShouldEndEditing(this)))
	{
		return false;
	}

    bool result = CAView::resignFirstResponder();


    return result;
}

bool CATextView::becomeFirstResponder()
{
	if (m_pDelegate &&( !m_pDelegate->textViewShouldBeginEditing(this)))
	{
		return false;
	}

	bool result = CAView::becomeFirstResponder();


    return result;
}

void CATextView::hideTextView()
{
    m_pBackgroundView->setVisible(false);
}

void CATextView::showTextView()
{
    m_pBackgroundView->setVisible(true);
}

void CATextView::hideNativeTextView()
{
	CAScheduler::unschedule(schedule_selector(CATextView::update), this);
}

void CATextView::showNativeTextView()
{
    this->update(0);
	CAScheduler::schedule(schedule_selector(CATextView::update), this, 1 / 60.0f);
}

void CATextView::delayShowImage()
{
    if (!CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::beginAnimations(m_s__StrID + "showImage", NULL);
        CAViewAnimation::setAnimationDuration(0);
		CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextView::showImage));
        CAViewAnimation::commitAnimations();
    }
}

void CATextView::showImage()
{
	((CATextViewWin32*)m_pTextView)->updateImage();
}

CATextView* CATextView::createWithFrame(const DRect& frame)
{
	CATextView *textView = new CATextView();
	if (textView && textView->initWithFrame(frame))
    {
		textView->autorelease();
		return textView;
    }
	CC_SAFE_DELETE(textView);
    return NULL;
}

CATextView* CATextView::createWithCenter(const DRect& rect)
{
	CATextView* textView = new CATextView();
    
	if (textView && textView->initWithCenter(rect))
    {
		textView->autorelease();
		return textView;
    }
    
	CC_SAFE_DELETE(textView);
    return NULL;
}

CATextView* CATextView::createWithLayout(const DLayout& layout)
{
    CATextView* textView = new CATextView();
    if (textView&&textView->initWithLayout(layout))
    {
        textView->autorelease();
        return textView;
    }
    
    CC_SAFE_RELEASE_NULL(textView);
    return NULL;
}

bool CATextView::init()
{
	if (!CAView::init())
	{
		return false;
	}
	this->setColor(CAColor_clear);

    CAImage* image = CAImage::create("source_material/textField_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);

	m_pBackgroundView = CAScale9ImageView::createWithImage(image);
	m_pBackgroundView->setLayout(DLayoutFill);
	m_pBackgroundView->setCapInsets(capInsets);
	m_pBackgroundView->setImage(image);
	this->insertSubview(m_pBackgroundView, -1);
	
	CATextViewWin32 *text = new CATextViewWin32(this);
	text->initWithLayout(DLayout(DHorizontalLayout_L_R(5, 5), DVerticalLayout_T_B(5, 5)));
	text->autorelease();
	this->addSubview(text);

	m_pTextView = (CATextViewWin32*)text;

    return true;
}

void CATextView::update(float dt)
{
    do
    {
        CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        
        CC_BREAK_IF(m_obLastPoint.equals(point));
        
        point.x = s_dip_to_px(point.x);
        point.y = s_dip_to_px(point.y);
    }
    while (0);
}

void CATextView::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
}

bool CATextView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	DPoint point = this->convertTouchToNodeSpace(pTouch);

	if (this->getBounds().containsPoint(point))
	{
		becomeFirstResponder();
		((CATextViewWin32*)m_pTextView)->StartMouseMove(point);
	}
	else
	{
		resignFirstResponder();
		((CATextViewWin32*)m_pTextView)->EndMouseMoveArr();
	}
    return true;
}

void CATextView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	((CATextViewWin32*)m_pTextView)->MoveMouseArrow(convertTouchToNodeSpace(pTouch));
}

void CATextView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
}

void CATextView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
}


void CATextView::setTextFontSize(const int& var)
{
	m_iFontSize = var;

	((CATextViewWin32*)m_pTextView)->updateFontSize();
    this->delayShowImage();
}

const int& CATextView::getTextFontSize()
{
	return m_iFontSize;
}


void CATextView::setText(const std::string& var)
{
	((CATextViewWin32*)m_pTextView)->setText(var);
}

const std::string& CATextView::getText()
{
	m_sText = ((CATextViewWin32*)m_pTextView)->getText();
	return m_sText;
}

void CATextView::setTextColor(const CAColor4B& var)
{
	m_sTextColor = var;

    this->delayShowImage();
}

const CAColor4B& CATextView::getTextColor()
{
	return m_sTextColor; 
}

void CATextView::setReturnType(const ReturnType& var)
{
    m_eReturnType = var;
}

const CATextView::ReturnType& CATextView::getReturnType()
{
    return m_eReturnType;
}

void CATextView::setBackgroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}

void CATextView::setTextViewAlign(const TextViewAlign& var)
{
    m_eAlign = var;
    
    this->delayShowImage();
}

const CATextView::TextViewAlign& CATextView::getTextViewAlign()
{
    return m_eAlign;
}



NS_CC_END

