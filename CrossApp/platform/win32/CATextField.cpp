//
//  CATextField.cpp
//  CrossApp
//
//  Created by 朱建 on 15/12/20.
//
//

#include "platform/CATextField.h"
#include "platform/CADensityDpi.h"
#include "platform/CAClipboard.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#include "control/CAButton.h"
#include "CCEGLView.h"
#include "CAIMEDelegate.h"
#include "CAFreeTypeFont.h"
#include "CAFTFontCache.h"
#include "support/ccUTF8.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN
static std::map<CrossApp::CATextField*, std::function<bool()> > s_ShouldBeginEditing_map;
static std::map<CrossApp::CATextField*, std::function<bool()> > s_ShouldEndEditing_map;
static std::map<CrossApp::CATextField*, std::function<void()> > s_ShouldReturn_map;
static std::map<CrossApp::CATextField*, std::function<void(int height)> > s_KeyBoardHeight_map;
static std::map<CrossApp::CATextField*, std::function<bool(ssize_t, ssize_t, const std::string&)> > s_ShouldChangeCharacters_map;
static std::map<CrossApp::CATextField*, std::function<void()> > s_DidChangeText_map;

class CATextFieldWin32 : public CAView, public CAIMEDelegate
{
	friend class CATextField;
public:
	CATextFieldWin32(CATextField* e)
		: m_pTextFieldX(e)
		, m_pCursorMark(NULL)
		, m_pTextViewMark(NULL)
		, m_iCurPos(0)
		, m_curSelCharRange(std::make_pair(0, 0))
		, m_iLabelWidth(0)
		, m_iString_l_length(0)
		, m_iString_r_length(0)
		, m_iString_o_length(0)
		, m_bCallbackTextChanged(true)
	{
		m_hEditCur = LoadCursor(NULL, IDC_IBEAM);
		m_iFontHeight = CAImage::getFontHeight(m_szFontName.c_str(), m_pTextFieldX->getFontSize());
	}

	virtual ~CATextFieldWin32()
	{
		DestroyCursor(m_hEditCur);
	}

	bool init()
	{
		if (!CAView::init())
		{
			return false;
		}
		this->m_pTextViewMark = CAView::createWithColor(CAColor4B(60, 120, 240, 127));
		this->addSubview(m_pTextViewMark);
		m_pTextViewMark->setVisible(false);
		return true;
	}
	
	void setContentSize(const DSize& var)
	{
		CAView::setContentSize(var);
		m_iLabelWidth = m_obContentSize.width;
		this->initMarkSprite();
	}
	
	void insertText(const char * text, int len)
	{
		if (!strcmp(text, "\n"))
		{
			getKeyBoradReturnCallBack();

			CATextFieldDelegate* pDelegate = m_pTextFieldX->getDelegate();
            if (m_pTextFieldX->isAllowkeyBoardHide())
            {
                m_pTextFieldX->resignFirstResponder();
				EndMouseMoveArr();
            }
            
            if (s_ShouldReturn_map.count(m_pTextFieldX) > 0 && s_ShouldReturn_map[m_pTextFieldX])
            {
                s_ShouldReturn_map[m_pTextFieldX]();
            }
            else if (pDelegate)
            {
                pDelegate->textFieldShouldReturn(m_pTextFieldX);
            }

			return;
		}

		if (m_sText.size() + len > m_pTextFieldX->getMaxLength() && m_pTextFieldX->getMaxLength() > 0)
			return;
		
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
		analyzeString(text, len);
		adjustCursorMove();
	}
	void clearText()
	{
		m_sText.clear();
		m_vTextFiledChars.clear();
		m_iString_l_length = 0;
		m_iString_r_length = 0;
		m_iString_o_length = 0;
		m_iCurPos = 0;
		m_curSelCharRange = std::make_pair(0, 0);
		setCursorPosition();
	}
	void deleteBackward()
	{
		CC_RETURN_IF(m_sText.empty() || m_iCurPos==0);
		if (m_pTextFieldX->isSecureTextEntry())
		{
			clearText();
			updateImage();
			return;
		}

		if (m_curSelCharRange.first != m_curSelCharRange.second)
		{
			if (!textValueChanged(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first, ""))
				return;
			
			CC_RETURN_IF(execCurSelCharRange());
		}
		else
		{
			int nDeleteLen = 1;
			while (0x80 == (0xC0 & m_sText.at(m_iCurPos - nDeleteLen)))
			{
				++nDeleteLen;
			}

			if (!textValueChanged(m_iCurPos, nDeleteLen, ""))
				return;

			m_sText.erase(m_iCurPos - nDeleteLen, nDeleteLen);
			m_iCurPos -= nDeleteLen;
			m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);
			m_vTextFiledChars.erase(m_vTextFiledChars.begin() + getStringCharCount(m_sText.substr(0, m_iCurPos)));

			adjustCursorMove();
		}
	}
	void copyToClipboard()
	{
		std::string cszText = m_sText.substr(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first);
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
		if (m_iCurPos == 0 || m_sText.empty())
			return;

		int iPrevPos = m_iCurPos, nMoveLen = 1;
		while (0x80 == (0xC0 & m_sText.at(m_iCurPos - nMoveLen)))
		{
			++nMoveLen;
		}
		m_iCurPos -= nMoveLen;

		m_curSelCharRange.first = m_curSelCharRange.second = m_iCurPos;
		adjustCursorMove();
		m_pTextViewMark->setVisible(false);
	}
	void cursorMoveForward()
	{
		if (m_iCurPos == m_sText.length() || m_sText.empty())
			return;

		int iPrevPos = m_iCurPos, nMoveLen = 1;
		for (std::string::size_type n = m_iCurPos + nMoveLen; n < m_sText.length() && 0x80 == (0xC0 & m_sText.at(n));)
		{
			++nMoveLen;
			n = m_iCurPos + nMoveLen;
		}
		m_iCurPos += nMoveLen;

		m_curSelCharRange.first = m_curSelCharRange.second = m_iCurPos;
		adjustCursorMove();
		m_pTextViewMark->setVisible(false);
	}
	void initMarkSprite()
	{
		if (m_pCursorMark == NULL)
		{
			m_pCursorMark = CAView::create();
			m_pCursorMark->setColor(CAColor4B(0,0,0,255));
			this->addSubview(m_pCursorMark);
			this->hideCursorMark();
		}
		setCursorPosition();
	}
	void showCursorMark()
	{
		if (!m_pCursorMark->isVisible())
		{
			m_pCursorMark->setVisible(true);

			m_pCursorMark->setAlpha(0);
			CAViewAnimation::beginAnimations(m_s__StrID);
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
	bool textValueChanged(int location, int length, const std::string& changedText)
	{
		if (m_bCallbackTextChanged)
		{
			std::u16string c1, c2;
			StringUtils::UTF8ToUTF16(m_sText.substr(0, location), c1);
			StringUtils::UTF8ToUTF16(m_sText.substr(location, length), c2);

            if (s_ShouldChangeCharacters_map.count(m_pTextFieldX) > 0 && s_ShouldChangeCharacters_map[m_pTextFieldX])
            {
                return s_ShouldChangeCharacters_map[m_pTextFieldX](c1.size(), c2.size(), changedText);
            }
            else if (m_pTextFieldX->getDelegate())
            {
                return m_pTextFieldX->getDelegate()->textFieldShouldChangeCharacters(m_pTextFieldX, c1.size(), c2.size(), changedText);
            }
		}
		return true;
	}
	void calculateSelChars(const DPoint& point, int& l, int& r, int& p)
	{
	_CalcuAgain:

		int dtValue = point.x - m_iString_o_length - getDtStrLength();
		int ll = 0, rr = 0, pp = 0;
		for (std::vector<TextAttribute>::iterator it = m_vTextFiledChars.begin(); it != m_vTextFiledChars.end(); ++it)
		{
			TextAttribute& t = *it;
			CC_BREAK_IF(ll + t.charlength / 2 > dtValue && getCursorX() >= 0);
			ll += t.charlength;
			pp += t.charSize;
		}
		rr = m_cImageSize.width - ll;

		if (pp == 0 && m_iString_o_length < 0)
		{
			m_iString_o_length = 0;
			goto _CalcuAgain;
		}
		l = ll; r = rr; p = pp;
	}
	void analyzeString(const char * text, int len)
	{
		std::string strLeft = m_sText.substr(0, m_iCurPos);
		std::string strRight = m_sText.substr(m_iCurPos, m_sText.size());
		std::string cszNewText(text, len);

		std::u32string cszU32Text;
		StringUtils::UTF8ToUTF32(cszNewText, cszU32Text);
		
		for (int i = 0; i < cszU32Text.size(); i++)
		{
			std::u32string c;
			c += cszU32Text[i];

			TextAttribute t;

			int iStrLen1 = getStringLength(strLeft);

			std::string str;
			StringUtils::UTF32ToUTF8(c, str);

			strLeft += str;
			t.charSize = str.size();

			int iStrLen2 = getStringLength(strLeft);

			t.charlength = iStrLen2 - iStrLen1;

			m_vTextFiledChars.insert(m_vTextFiledChars.begin() + (getStringCharCount(strLeft) - 1), t);
			m_iCurPos += t.charSize;
		}

		m_sText = strLeft + strRight;
		m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);
	}
	void setCursorPosition()
	{
		m_pCursorMark->setFrame(DRect(0, 0, 2.5f, m_iFontHeight));
		if (m_pTextFieldX->isSecureTextEntry())
		{
			m_pCursorMark->setCenterOrigin(DPoint((m_sText.empty() ? 0 : this->getImageRect().size.width), m_obContentSize.height / 2));
		}
		else
		{
			int mPmarkWidth = MIN(m_iLabelWidth, getCursorX());
			m_pCursorMark->setCenterOrigin(DPoint(mPmarkWidth, m_obContentSize.height / 2));
		}
	}
	int getStringLength(const std::string &var)
	{
		std::string cszText = var;
		if (m_pTextFieldX->isSecureTextEntry())
		{
			cszText.clear();
			for (int i = 0; i < var.size(); i++)
				cszText += '*';
		}
		return g_AFTFontCache.getStringWidth(m_szFontName.c_str(), m_pTextFieldX->getFontSize(), cszText);
	}
	int getStringViewLength()
	{
		return MIN(m_iLabelWidth, m_iString_r_length + m_iString_l_length + m_iString_o_length);
	}
	bool execCurSelCharRange()
	{
		m_pTextViewMark->setVisible(false);

		if (m_curSelCharRange.first == m_curSelCharRange.second)
			return false;

		int iOldCurPos = m_curSelCharRange.first;
		std::string cszText = m_sText.erase(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first);
		setText(cszText);

		m_iCurPos = iOldCurPos;
		showCursorMark();
		adjustCursorMove();
		return true;
	}
	DRect getZZCRect(bool* l=0, bool* r=0)
	{
		int l1 = getStringLength(m_sText.substr(0, m_curSelCharRange.first));
		int l2 = getStringLength(m_sText.substr(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first));
		int ld = getDtStrLength();

		bool ll = (l1 + m_iString_o_length) >= 0;
		bool rr = (l1 + m_iString_o_length + l2 + ld) <= m_iLabelWidth;

		int dd = 0;
		if (ll && rr)
		{
			dd = l2;
		}
		else if (ll)
		{
			dd = m_iLabelWidth - (l1 + m_iString_o_length + ld);
		}
		else if (rr)
		{
			dd = l1 + m_iString_o_length + l2;
		}
		else dd = m_iLabelWidth;

		if (l != NULL)
		{
			*l = ll;
		}
		if (r != NULL)
		{
			*r = rr;
		}
		int y = (m_obContentSize.height - m_obRect.size.height) / 2;
		return DRect((ll ? (l1 + m_iString_o_length) : 0) + ld, y, dd, m_iFontHeight);
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
		return m_sText;
	}
	void adjustCursorMove()
	{
		this->updateImage();

		if (m_sText.empty())
		{
			m_iString_l_length = m_iString_r_length = 0;
		}
		else
		{
			m_iString_l_length = getStringLength(m_sText.substr(0, m_iCurPos));
			m_iString_r_length = m_cImageSize.width - m_iString_l_length;
		}

		if (getCursorX() > m_iLabelWidth && m_iLabelWidth < m_iString_l_length)
		{
			m_iString_o_length = m_iLabelWidth - m_iString_l_length;
		}

		if (getCursorX() <= 0)
		{
			m_iString_o_length = -m_iString_l_length;
		}
		else
		{
			if (m_iString_r_length + getCursorX() < m_iLabelWidth)
			{
				m_iString_o_length = MIN(0, m_iLabelWidth - m_cImageSize.width);
			}
		}

		DRect r = DRect(0, 0, MIN(m_iLabelWidth, m_cImageSize.width), m_cImageSize.height);
		r.origin.x = -m_iString_o_length;
		r.size.width = getStringViewLength();
		if (r.size.width == 0)
		{
			r.size.width = m_cImageSize.width;
		}
		this->setImageRect(r);

		setCursorPosition();
	}
	int getDtStrLength()
	{
		int iStr_d_length = 0;
		if (m_iString_l_length + m_iString_r_length < m_iLabelWidth)
		{
			CATextField::Align align = m_pTextFieldX->getAlign();
			switch (align)
			{
			case CATextField::Align::Left:
				iStr_d_length = 0;
				break;
			case CATextField::Align::Center:
				iStr_d_length = (m_iLabelWidth - (m_iString_l_length + m_iString_r_length)) / 2;
				break;
			case CATextField::Align::Right:
				iStr_d_length = (m_iLabelWidth - (m_iString_l_length + m_iString_r_length));
				break;
			}
		}
		return iStr_d_length;
	}
	int getStringCharCount(const std::string &var)
	{
		std::u32string cszU32Text;
		StringUtils::UTF8ToUTF32(var, cszU32Text);
		return cszU32Text.size();
	}
	int getCursorX()
	{
		return m_iString_l_length + m_iString_o_length + getDtStrLength();
	}
	void updateImage()
	{
		const std::string& sPlaceHolder = m_pTextFieldX->getPlaceHolderText();
		CAColor4B cFontColor;
		std::string text;
		if (m_sText.empty())
		{
			text = sPlaceHolder;
			cFontColor = m_pTextFieldX->getPlaceHolderColor();
		}
		else
		{
			text = m_sText;
			cFontColor = m_pTextFieldX->getTextColor();
		}
		
		if (m_pTextFieldX->isSecureTextEntry())
		{
			std::string password;
			for (std::string::size_type i = 0; i < m_sText.length(); i++)
			{
				password.append("*");
			}
			if (password.empty())
			{
				text = sPlaceHolder;
			}
			else
			{
				text = password;
			}
		}

		int fontSize = m_pTextFieldX->getFontSize();
		m_iFontHeight = CAImage::getFontHeight(m_szFontName.c_str(), fontSize);

		DSize size = DSize(0, m_iFontHeight);
		CAImage* image = CAFTFontCache::getAFTFontCache().initWithString(text.c_str(),
			cFontColor,
			m_szFontName.c_str(),
			fontSize,
			size.width,
			size.height,
			CATextAlignment::Left,
			CAVerticalTextAlignment::Center,
			true, 0, false, false, false, false, nullptr);
		DRect rect = DRectZero;
		rect.size = image->getContentSize();
		if (sPlaceHolder.length() == 0)
		{
			this->setImage(image);
			this->setImageRect(rect);
		}

		CC_RETURN_IF(image == NULL);

		rect.size.height = image->getContentSize().height;
		rect.size.width = MIN(m_iLabelWidth, image->getContentSize().width);

		if (text.empty())
		{
			m_cImageSize = DSizeZero;
		}
		else
		{
			m_cImageSize = image->getContentSize();
		}
		this->setImage(image);

		rect.origin.x = -m_iString_o_length;
		this->setImageRect(rect);
	}

	void updateImageRect()
	{
		GLfloat x1, x2, y1, y2;

		CATextField::Align align = m_pTextFieldX->getAlign();
		if (align == CATextField::Align::Right)
		{
			x1 = m_iLabelWidth - m_obRect.size.width;
		}
		else if (align == CATextField::Align::Center)
		{
			x1 = (m_iLabelWidth - m_obRect.size.width) / 2;
		}
		else
		{
			x1 = 0;
		}

		y1 = (m_obContentSize.height - m_obRect.size.height) / 2;
		x2 = x1 + m_obRect.size.width;
		y2 = y1 + m_obRect.size.height;
		m_sQuad.bl.vertices = DPoint3D(x1, y1, m_fPointZ);
		m_sQuad.br.vertices = DPoint3D(x2, y1, m_fPointZ);
		m_sQuad.tl.vertices = DPoint3D(x1, y2, m_fPointZ);
		m_sQuad.tr.vertices = DPoint3D(x2, y2, m_fPointZ);
	}

	void StartMouseMove(const DPoint& point)
	{
		CAIMEDelegate::attachWithIME();
		showCursorMark();

		calculateSelChars(point, m_iString_l_length, m_iString_r_length, m_iCurPos);
		setCursorPosition();
		m_iStartMovePos = m_iCurPos;
		m_pTextViewMark->setVisible(false);

		::SetCursor(m_hEditCur);
	}
	void MoveMouseArrow(const DPoint& pt)
	{
		calculateSelChars(pt, m_iString_l_length, m_iString_r_length, m_iCurPos);
		m_curSelCharRange.first = MIN(m_iCurPos, m_iStartMovePos);
		m_curSelCharRange.second = MAX(m_iCurPos, m_iStartMovePos);
		adjustCursorMove();
		m_pTextViewMark->setFrame(getZZCRect());
		m_pTextViewMark->setVisible(true);
	}
	void EndMouseMoveArr()
	{
		CAIMEDelegate::detachWithIME();
		this->hideCursorMark();
		m_pTextViewMark->setVisible(false);
	}

private:
	CATextField* m_pTextFieldX;
	CAView* m_pCursorMark;
	CAView* m_pTextViewMark;

	std::vector<TextAttribute> m_vTextFiledChars;
	std::pair<int, int> m_curSelCharRange;
	int m_iCurPos;

	int m_iLabelWidth;
	int m_iString_l_length;
	int m_iString_r_length;
	int m_iString_o_length;
	int m_iFontHeight;
	std::string m_szFontName;
	std::string m_sText;
	DSize m_cImageSize;
	int m_iStartMovePos;
	HCURSOR m_hEditCur;
	bool m_bCallbackTextChanged;
};


CATextField::CATextField()
: m_pBackgroundView(nullptr)
, m_pImgeView(nullptr)
, m_pTextField(nullptr)
, m_pDelegate(nullptr)
, m_bUpdateImage(true)
, m_bSecureTextEntry(false)
, m_bAllowkeyBoardHide(true)
, m_cTextColor(CAColor4B::BLACK)
, m_cPlaceHdolderColor(CAColor4B::GRAY)
, m_iMarginLeft(10)
, m_iMarginRight(10)
, m_iFontSize(40)
, m_iMaxLength(0)
, m_eClearBtn(CATextField::ClearButtonMode::None)
, m_eAlign(CATextField::Align::Left)
, m_eReturnType(CATextField::ReturnType::Done)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
}

CATextField::~CATextField()
{
    s_ShouldBeginEditing_map.erase(this);
    s_ShouldEndEditing_map.erase(this);
    s_ShouldReturn_map.erase(this);
    s_KeyBoardHeight_map.erase(this);
    s_ShouldChangeCharacters_map.erase(this);
    s_DidChangeText_map.erase(this);
    
    CAViewAnimation::removeAnimations(m_s__StrID + "showImage");
    m_pDelegate = NULL;
}

void CATextField::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
}

void CATextField::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();

	CAViewAnimation::removeAnimations(m_s__StrID + "showImage");
    if (this->isFirstResponder())
    {
        this->resignFirstResponder();
    }
}

bool CATextField::resignFirstResponder()
{
    if (s_ShouldEndEditing_map.count(this) > 0 && s_ShouldEndEditing_map[this] && !s_ShouldEndEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
    {
        return false;
    }

    bool result = CAControl::resignFirstResponder();
    return result;
}

bool CATextField::becomeFirstResponder()
{
    if (s_ShouldBeginEditing_map.count(this) > 0 && s_ShouldBeginEditing_map[this] && !s_ShouldBeginEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
    {
        return false;
    }

	bool result = CAControl::becomeFirstResponder();
    return result;
}

void CATextField::hideNativeTextField()
{

}

void CATextField::showNativeTextField()
{

}

void CATextField::delayShowImage()
{
	this->cancelPreviousPerformRequests(callfunc_selector(CATextField::showImage));
	this->performSelector(callfunc_selector(CATextField::showImage), 0.1);
}


void CATextField::showImage()
{
	((CATextFieldWin32*)m_pTextField)->updateImage();
}

CATextField* CATextField::createWithFrame(const DRect& frame)
{
    CATextField *textField = new CATextField();
    if (textField && textField->initWithFrame(frame))
    {
        textField->autorelease();
        return textField;
    }
    CC_SAFE_DELETE(textField);
    return NULL;
}

CATextField* CATextField::createWithCenter(const DRect& rect)
{
    CATextField* textField = new CATextField();
    
    if (textField && textField->initWithCenter(rect))
    {
        textField->autorelease();
        return textField;
    }
    
    CC_SAFE_DELETE(textField);
    return NULL;
}

CATextField* CATextField::createWithLayout(const DLayout& layout)
{
    CATextField* textField = new CATextField();
    if (textField && textField->initWithLayout(layout))
    {
        textField->autorelease();
        return textField;
    }
    CC_SAFE_DELETE(textField);
    return NULL;
}


bool CATextField::init()
{
	if (!CAControl::init())
	{
		return false;
	}
	this->setColor(CAColor4B::CLEAR);

	const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CATextField");
	CAImage* image = CAImage::create(map.at("backgroundView_normal"));
	m_pBackgroundView = CAScale9ImageView::createWithFrame(DRect(0, 0, 1, 1));
	m_pBackgroundView->setLayout(DLayoutFill);
	m_pBackgroundView->setCapInsets(DRect(image->getPixelsWide() / 2, image->getPixelsHigh() / 2, 1, 1));
	m_pBackgroundView->setImage(image);
	this->addSubview(m_pBackgroundView);

	CATextFieldWin32 *text = new CATextFieldWin32(this);
	text->initWithFrame(DRect(0, 0, 1, 1));

	this->addSubview(text);
	text->release();

	m_pTextField = (CATextFieldWin32*)text;
    return true;
}

void CATextField::setAlign(CATextField::Align e)
{
	m_eAlign = e;
	((CATextFieldWin32*)m_pTextField)->updateImage();
}

CATextField::Align CATextField::getAlign()
{
	return m_eAlign;
}

void CATextField::update(float dt)
{
}

void CATextField::setContentSize(const DSize& contentSize)
{
    CAControl::setContentSize(contentSize);

	if (m_eClearBtn == ClearButtonMode::WhileEditing)
	{
		const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CATextField");
		this->setMarginImageRight(DSize(m_obContentSize.height, m_obContentSize.height), map.at("clearImage"));
		CAButton* button = (CAButton*)this->getSubviewByTag(1011);
		button->setImageSize(DSize(m_obContentSize.height, m_obContentSize.height));
	}

	if (m_pTextField)
	{
		DRect r = this->getBounds();
		r.InflateRect(-m_iMarginLeft, 0, -m_iMarginRight, 0);
		((CATextFieldWin32*)m_pTextField)->setFrame(r);
	}
}

bool CATextField::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	DPoint point = this->convertTouchToNodeSpace(pTouch);

	if (this->getBounds().containsPoint(point))
	{
		becomeFirstResponder();
		((CATextFieldWin32*)m_pTextField)->StartMouseMove(point);
	}
	else
	{
		resignFirstResponder();
		((CATextFieldWin32*)m_pTextField)->EndMouseMoveArr();
	}
    return true;
}

void CATextField::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	((CATextFieldWin32*)m_pTextField)->MoveMouseArrow(convertTouchToNodeSpace(pTouch));
}

void CATextField::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
   
}

void CATextField::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextField::setClearButtonMode(ClearButtonMode var)
{
	if (var == ClearButtonMode::WhileEditing)
	{
		const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CATextField");
		this->setMarginImageRight(DSize(m_obContentSize.height, m_obContentSize.height), map.at("clearImage"));
		CAButton* button = (CAButton*)this->getSubviewByTag(1011);
		button->setImageColorForState(CAControl::State::Highlighted, ccc4Int(0xff666666));
		button->setImageSize(DSize(m_obContentSize.height, m_obContentSize.height));
		button->addTarget([&]{
		
			if (getText().length() > 0)
			{
				this->setText("");
				this->delayShowImage();
			}
		}, CAButton::Event::TouchUpInSide);
	}
	else
	{
		CAButton* button = (CAButton*)this->getSubviewByTag(1011);
		this->removeSubview(button);
		this->setMarginRight(10);
	}
	m_eClearBtn = var;
}

CATextField::ClearButtonMode CATextField::getClearButtonMode()
{
	return m_eClearBtn;
}

void CATextField::setMarginLeft(int var)
{
	m_iMarginLeft = var;

	DRect r = this->getBounds();
	r.InflateRect(-m_iMarginLeft, 0, -m_iMarginRight, 0);
	((CATextFieldWin32*)m_pTextField)->setFrame(r);

	delayShowImage();
}

int CATextField::getMarginLeft()
{
	return m_iMarginLeft;
}

void CATextField::setMarginRight(int var)
{
	m_iMarginRight = var;

	DRect r = this->getBounds();
	r.InflateRect(-m_iMarginLeft, 0, -m_iMarginRight, 0);
	((CATextFieldWin32*)m_pTextField)->setFrame(r);

	delayShowImage();
}

int CATextField::getMarginRight()
{
	return m_iMarginRight;
}

void CATextField::setMarginImageLeft(const DSize& imgSize, const std::string& filePath)
{
	setMarginLeft(imgSize.width);

	CAImageView* ima = (CAImageView*)this->getSubviewByTag(1010);
	if (!ima)
	{
		ima = CAImageView::create();
		ima->setTag(1010);
		this->addSubview(ima);
	}
	DLayout layout;
	layout.horizontal.left = 0;
	layout.horizontal.width = imgSize.width;
	layout.vertical.height = imgSize.height;
	layout.vertical.center = 0.5f;
	ima->setLayout(layout);
	ima->setImage(CAImage::create(filePath));
}

void CATextField::setMarginImageRight(const DSize& imgSize, const std::string& filePath)
{
	setMarginRight(imgSize.width);

	CAButton* ima = (CAButton*)this->getSubviewByTag(1011);
	if (!ima)
	{
		ima = CAButton::create(CAButton::Type::Custom);
		ima->setTag(1011);
		this->addSubview(ima);
	}
	DLayout layout;
	layout.horizontal.right = 0;
	layout.horizontal.width = imgSize.width;
	layout.vertical.height = imgSize.height;
	layout.vertical.center = 0.5f;
	ima->setLayout(layout);
	ima->setImageForState(CAControl::State::Normal, CAImage::create(filePath));
}


void CATextField::setFontSize(int var)
{
	m_iFontSize = var;

	delayShowImage();
}

int CATextField::getFontSize()
{
	return m_iFontSize;
}

void CATextField::setPlaceHolderText(const std::string& var)
{
	m_sPlaceHolderText = var;

	delayShowImage();
}

const std::string& CATextField::getPlaceHolderText()
{
	return m_sPlaceHolderText;
}

void CATextField::setPlaceHolderColor(const CAColor4B& var)
{
	m_cPlaceHdolderColor = var;

	delayShowImage();
}

const CAColor4B& CATextField::getPlaceHolderColor()
{
	return m_cPlaceHdolderColor;
}

void CATextField::setText(const std::string& var)
{
	((CATextFieldWin32*)m_pTextField)->setText(var);
}

const std::string& CATextField::getText()
{
	m_sText = ((CATextFieldWin32*)m_pTextField)->getText();
	return m_sText;
}


void CATextField::setTextColor(const CAColor4B& var)
{
	m_cTextColor = var;

	delayShowImage();
}

const CAColor4B& CATextField::getTextColor()
{
	return m_cTextColor; 
}


void CATextField::setKeyboardType(KeyboardType var)
{
	m_eKeyBoardType = var;
}

CATextField::KeyboardType CATextField::getKeyboardType()
{
	return m_eKeyBoardType;
}

void CATextField::setReturnType(CATextField::ReturnType var)
{
	m_eReturnType = var;
}

CATextField::ReturnType CATextField::getReturnType()
{
	return m_eReturnType;
}

void CATextField::setBackgroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}

void CATextField::setSecureTextEntry(bool var)
{
    m_bSecureTextEntry = var;

	delayShowImage();
}

bool CATextField::isSecureTextEntry()
{
    return m_bSecureTextEntry;
}

void CATextField::setMaxLength(int var)
{
    m_iMaxLength = var;
}

int CATextField::getMaxLength()
{
    return m_iMaxLength;
}

void CATextField::onShouldBeginEditing(const std::function<bool ()> &var)
{
    m_obShouldBeginEditing = var;
    s_ShouldBeginEditing_map[this] = var;
}

void CATextField::onShouldEndEditing(const std::function<bool ()> &var)
{
    m_obShouldEndEditing = var;
    s_ShouldEndEditing_map[this] = var;
}

void CATextField::onShouldReturn(const std::function<void ()> &var)
{
    m_obShouldReturn = var;
    s_ShouldReturn_map[this] = var;
}

void CATextField::onKeyBoardHeight(const std::function<void (int)> &var)
{
    m_obKeyBoardHeight = var;
    s_KeyBoardHeight_map[this] = var;
}

void CATextField::onShouldChangeCharacters(const std::function<bool (ssize_t, ssize_t, const std::string &)> &var)
{
    m_obShouldChangeCharacters = var;
    s_ShouldChangeCharacters_map[this] = var;
}

void CATextField::onDidChangeText(const std::function<void ()> &var)
{
    m_obDidChangeText = var;
    s_DidChangeText_map[this] = var;
}

NS_CC_END

