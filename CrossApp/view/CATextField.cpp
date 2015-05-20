//
//  CATextField.cpp
//  project
//
//  Created by lh on 14-5-15.
//
//

#include "CATextField.h"
#include "basics/CAApplication.h"
#include "actions/CCActionInterval.h"
#include "CCEGLView.h"
#include <locale>
#include <algorithm>
#include "view/CAScale9ImageView.h"
#include "shaders/CAShaderCache.h"
#include "platform/CAClipboard.h"
#include "CATextEditHelper.h"

NS_CC_BEGIN

#define BORDER_WIDTH(w) (0.01125*w)

CATextField::CATextField()
: m_pDelegate(NULL)
, m_cTextColor(ccc4(0, 0, 0, 255))
, m_cSpaceHolderColor(ccc4(127, 127, 127, 255))
, m_iFontSize(_px(24))
, m_nInputType(KEY_BOARD_INPUT_NORMAL)
, m_keyboardType(KEY_BOARD_TYPE_NORMAL)
, m_sText("")
, m_sPlaceHolder("")
, m_cCursorColor(CAColor_black)
, m_iCurPos(0)
, m_curSelCharRange(std::make_pair(0, 0))
, m_iLabelWidth(0)
, m_pCursorMark(NULL)
, m_pTextViewMark(NULL)
, m_iString_left_offX(0)
, m_iString_l_length(0)
, m_iString_r_length(0)
, m_iFontHeight(0)
, m_iHoriMargins(0)
, m_iVertMargins(0)
, m_pBackgroundView(NULL)
, m_isTouchInSide(false)
, m_keyBoardReturnType(KEY_BOARD_RETURN_DONE)
{
	m_iFontHeight = CAImage::getFontHeight(m_nfontName.c_str(), m_iFontSize);
}

CATextField::~CATextField()
{
} 

void CATextField::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    CC_RETURN_IF(m_sPlaceHolder.empty());
    this->updateImage();
    
}

void CATextField::onExitTransitionDidStart()
{
	CAView::onExitTransitionDidStart();
}

bool CATextField::resignFirstResponder()
{
	bool result = CAView::resignFirstResponder();
    if (result)
    {
		detachWithIME();
        hideCursorMark();
        this->updateImage();
    }
    return result;
}

bool CATextField::becomeFirstResponder()
{
	bool result = CAView::becomeFirstResponder();
    if (result) 
	{
		attachWithIME();
        this->showCursorMark();
        if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
        {
            if (m_sText.empty())
            {
                m_pCursorMark->setCenterOrigin(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height / 2));
            }
        }
        calculateSelChars(CCPoint(this->getCursorX() + m_iHoriMargins, m_obContentSize.height / 2), m_iString_l_length, m_iString_r_length, m_iCurPos);
        
        m_pCursorMark->setCenterOrigin(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height / 2));

#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
        CCEGLView * pGlView = CAApplication::getApplication()->getOpenGLView();
        pGlView->setIMECursorPos(getCursorPos(), getContentText());
#endif
    }
    return result;
}

CATextField* CATextField::createWithFrame(const CCRect& frame)
{
    CATextField *text = new CATextField();
    if (text && text->initWithFrame(frame))
    {
        text->autorelease();
        return text;
    }
    CC_SAFE_DELETE(text);
    return NULL;
}

CATextField* CATextField::createWithCenter(const CCRect& rect)
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

bool CATextField::init()
{
    if (!CAView::init())
	{
        return false;
    }
    
    this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/textField_bg.png")));
    this->m_pTextViewMark = CAView::createWithColor(ccc4(60, 120, 240, 127));
	this->addSubview(m_pTextViewMark);
	m_pTextViewMark->setVisible(false);

    return true;
}

void CATextField::initMarkSprite()
{
    if (m_pCursorMark == NULL)
    {
        m_pCursorMark = CAView::create();
        m_pCursorMark->setColor(m_cCursorColor);
        this->addSubview(m_pCursorMark);
        this->hideCursorMark();
    }

	m_pCursorMark->setFrame(CCRect(m_iHoriMargins, 0, _px(2), CAImage::getFontHeight(m_nfontName.c_str(), m_iFontSize)));
}

void CATextField::showCursorMark()
{
    if(!m_pCursorMark->isVisible()){
        m_pCursorMark->setVisible(true);
        m_pCursorMark->runAction(CCRepeat::create(CCBlink::create(0.8f, 1), 1048576));
    }
}

void CATextField::hideCursorMark()
{
    m_pCursorMark->setVisible(false);
    m_pCursorMark->stopAllActions();
}

void CATextField::setFontSize(int var)
{
	CCAssert(m_pCursorMark != NULL, "");
    
	m_iFontSize = var;
	m_iFontHeight = CAImage::getFontHeight(m_nfontName.c_str(), m_iFontSize);
    m_iVertMargins = (m_obContentSize.height - m_iFontHeight) / 2;
    
	m_pCursorMark->setFrame(CCRect(m_iHoriMargins, 0, _px(2), var));
    setText(m_sText);
    this->updateImage();
}

int CATextField::getFontSize()
{
	return m_iFontSize;
}

void CATextField::setFontName(const std::string& szFontName)
{
	m_nfontName = szFontName;
	setFontSize(m_iFontSize);
}

const std::string& CATextField::getFontName()
{
	return m_nfontName;
}

void CATextField::setText(const std::string &var)
{
    CATextFieldDelegate* pTemp = m_pDelegate;
    m_pDelegate = NULL;
    m_sText.clear();
	m_iCurPos = 0;
	m_curSelCharRange = std::make_pair(0, 0);
	m_iString_left_offX = 0;
	m_iString_l_length = 0;
	m_iString_r_length = 0;
	m_vTextFiledChars.clear();
    CCPoint p = CCPoint(this->getCursorX() + m_iHoriMargins, m_obContentSize.height / 2);
    m_pCursorMark->setCenterOrigin(p);
    insertText(var.c_str(), (int)var.length());
    m_pDelegate = pTemp;
    this->updateImage();
}

const std::string &CATextField::getText()
{
    return m_sText;
}

void CATextField::setPlaceHolder(const std::string &var)
{
    m_sPlaceHolder = var;
    this->updateImage();
}

const std::string &CATextField::getPlaceHolder()
{
    return m_sPlaceHolder;
}

void CATextField::setSpaceHolderColor(const CAColor4B &var)
{
    m_cSpaceHolderColor = var;
    this->updateImage();
}

const CAColor4B &CATextField::getSpaceHolderColor()
{
    return m_cSpaceHolderColor;
}

void CATextField::setTextColor(const CAColor4B &var)
{
    m_cTextColor = var;
    this->updateImage();
}

const CAColor4B &CATextField::getTextColor()
{
    return m_cTextColor;
}

bool CATextField::attachWithIME()
{
    bool bRet = CAIMEDelegate::attachWithIME();
    if (bRet)
    {
        // open keyboard
        CCEGLView * pGlView = CAApplication::getApplication()->getOpenGLView();
        if (pGlView)
        {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
            if (getKeyboardType() ==KEY_BOARD_TYPE_NORMAL)
            {
                pGlView->setIMEKeyboardDefault();
            }
            else if (getKeyboardType() ==KEY_BOARD_TYPE_NUMBER)
            {
                pGlView->setIMEKeyboardNumber();
            }
            else if(getKeyboardType() ==KEY_BOARD_TYPE_ALPHABET)
            {
                pGlView->setIMEKeyboardAlphabet();
            }
            
            if (getKeyboardReturnType() ==KEY_BOARD_RETURN_SEND)
            {
                pGlView->setIMEKeyboardReturnSend();
            }
            else if (getKeyboardReturnType() ==KEY_BOARD_RETURN_SEARCH)
            {
                pGlView->setIMEKeyboardReturnSearch();
            }
            else if(getKeyboardReturnType() ==KEY_BOARD_RETURN_DONE)
            {
                pGlView->setIMEKeyboardReturnDone();
            }
#endif
            m_pCursorMark->setCenterOrigin(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height / 2));
            pGlView->setIMEKeyboardState(true);
        }
    }
    return bRet;
}

bool CATextField::detachWithIME()
{
    bool bRet = CAIMEDelegate::detachWithIME();
    if (bRet)
    {
        // close keyboard
        CCEGLView * pGlView = CAApplication::getApplication()->getOpenGLView();
        
        if (pGlView)
        {
            pGlView->setIMEKeyboardState(false);
        }
    }
    return bRet;
}

void CATextField::setInputType(eKeyBoardInputType type)
{
    m_nInputType=type;
}

eKeyBoardInputType CATextField::getInputType()
{
    return m_nInputType;
}

void CATextField::calculateSelChars(const CCPoint& point, int& l, int& r, int& p)
{
_CalcuAgain:
	int dtValue = point.x - m_iString_left_offX - m_iHoriMargins;
	l = r = p = 0;
	for (std::vector<TextAttribute>::iterator it = m_vTextFiledChars.begin(); it != m_vTextFiledChars.end(); ++it)
	{
		TextAttribute& t = *it;
		CC_BREAK_IF(l + t.charlength / 2 > dtValue && getCursorX() + m_iHoriMargins > 0);
		l += t.charlength;
		p += t.charSize;
	}
	r = m_cImageSize.width - l;

	if (p == 0 && m_iString_left_offX < 0)
	{
		m_iString_left_offX = 0;
		goto _CalcuAgain;
	}
}

bool CATextField::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	return CATouchView::ccTouchBegan(pTouch, pEvent);
}

void CATextField::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	CATouchView::ccTouchEnded(pTouch, pEvent);

	if (CATextToolBarView::isTextToolBarShow())
		return;

    CCPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
        if (!isFirstResponder() && canAttachWithIME())
        {
            becomeFirstResponder();
            
            calculateSelChars(point, m_iString_l_length, m_iString_r_length, m_iCurPos);
            
            m_pCursorMark->setCenterOrigin(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height / 2));
        }
    }
    else
    {
        if (canDetachWithIME())
        {
            resignFirstResponder();
        }
    }

	m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);
	execCurSelCharRange();
}

void CATextField::ccTouchPress(CATouch *pTouch, CAEvent *pEvent)
{
	if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
		return;

	std::string cszText = CAClipboard::getText();
	if (cszText.empty() && m_sText.empty())
		return;

	CATextToolBarView *pToolBar = CATextToolBarView::create();
	if (m_sText.empty())
	{
		pToolBar->addButton(UTF8("\u7c98\u8d34"), this, callfunc_selector(CATextField::ccPasteFromClipboard));
	}
	else
	{
		pToolBar->addButton(UTF8("\u7c98\u8d34"), this, callfunc_selector(CATextField::ccPasteFromClipboard));
		pToolBar->addButton(UTF8("\u5168\u9009"), this, callfunc_selector(CATextField::ccSelectAll));
		pToolBar->addButton(UTF8("\u9009\u62e9"), this, callfunc_selector(CATextField::ccStartSelect));
	}
    
    if (canDetachWithIME())
    {
        resignFirstResponder();
    }
    
    pToolBar->show();
}

bool CATextField::canAttachWithIME()
{
    return (m_pDelegate) ? m_pDelegate->onTextFieldAttachWithIME(this) : true;
}

bool CATextField::canDetachWithIME()
{
    return (m_pDelegate) ? m_pDelegate->onTextFieldDetachWithIME(this) : true;
}

void CATextField::didDetachWithIME()
{
    hideCursorMark();
}

void CATextField::didAttachWithIME()
{
    showCursorMark();
}

void CATextField::analyzeString(const char * text, int len)
{
	std::string strLeft = m_sText.substr(0, m_iCurPos);
	std::string strRight = m_sText.substr(m_iCurPos, m_sText.size());

	for (int i = 0; i < len; i++)
	{
		TextAttribute t;
		
		int iStrLen1 = getStringLength(strLeft);
		if (text[i] >= 0 && text[i] <= 127)
		{
			t.charSize = 1;
			strLeft += text[i];
		}
		else
		{
			t.charSize = 3;
			strLeft += text[i++];
			strLeft += text[i++];
			strLeft += text[i];
		}
		int iStrLen2 = getStringLength(strLeft);

		t.charlength = iStrLen2 - iStrLen1;

		m_vTextFiledChars.insert(m_vTextFiledChars.begin() + (getStringCharCount(strLeft)-1), t);
		m_iCurPos += t.charSize;
	}
	m_sText = strLeft + strRight;
	m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);
}

void CATextField::insertText(const char * text, int len)
{
    CC_RETURN_IF(len <= 0);
    CC_RETURN_IF(text == 0);
    
    if (!strcmp(text, "\n"))
    {
        getKeyBoradReturnCallBack();
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CC_RETURN_IF(m_nInputType == KEY_BOARD_INPUT_PASSWORD && len >= 2);
#endif
    
	execCurSelCharRange();
    analyzeString(text, len);
    CC_RETURN_IF(m_pDelegate && m_pDelegate->onTextFieldInsertText(this, m_sText.c_str(), (int)m_sText.length()));

    adjustCursorMoveForward();
}

void CATextField::AndroidWillInsertText(int start,const char* str,int before,int count)
{
    CCAssert(str != NULL, "");
	CCAssert(count > 0, "");
    
    for (int i=0; i<before; i++)
    {
        deleteBackward();
    }
    CC_RETURN_IF(str == NULL || count <= 0);
    
    std::string s = str;
    insertText(s.c_str(), s.length());
}

void CATextField::willInsertText(const char *text, int len)
{
	execCurSelCharRange();

	int iOldCurPos = m_iCurPos;
	insertText(text, len);
	m_curSelCharRange = std::make_pair(iOldCurPos, m_iCurPos);

	m_pTextViewMark->setFrame(getZZCRect());
	m_pTextViewMark->setVisible(true);
}

void CATextField::deleteBackward()
{
    if (m_nInputType==KEY_BOARD_INPUT_PASSWORD)
    {
        m_sText.clear();
		m_vTextFiledChars.clear();
		this->updateImage();
		m_iString_l_length = 0;
		m_iString_r_length = 0;
		m_iString_left_offX = 0;
		m_iCurPos = 0;
		m_curSelCharRange = std::make_pair(0, 0);
		m_pCursorMark->setCenterOrigin(CCPoint(m_iHoriMargins + getCursorX(), m_obContentSize.height / 2));

        return;
    }

	if (m_iCurPos==0 || m_sText.empty())
		return;

	if (execCurSelCharRange())
		return;

	int nDeleteLen = 1;
	while (0x80 == (0xC0 & m_sText.at(m_iCurPos - nDeleteLen)))
	{
		++nDeleteLen;
	}
	
    m_sText.erase(m_iCurPos - nDeleteLen, nDeleteLen);
    m_iCurPos -= nDeleteLen;
	m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);
    CC_RETURN_IF(m_pDelegate && m_pDelegate->onTextFieldDeleteBackward(this, m_sText.c_str(), (int)m_sText.length()));
	
	m_vTextFiledChars.erase(m_vTextFiledChars.begin() + getStringCharCount(m_sText.substr(0, m_iCurPos)));
	
    adjustCursorMoveBackward();
}

void CATextField::adjustCursorMoveBackward()
{
	this->updateImage();
    
	m_iString_l_length = getStringLength(m_sText.substr(0, m_iCurPos));
	m_iString_r_length = m_cImageSize.width - m_iString_l_length;
    
	if (getCursorX() <= 0)
	{
		m_iString_left_offX = -m_iString_l_length;
	}
	else
	{
		if (m_iString_r_length + getCursorX() < m_iLabelWidth)
		{
			m_iString_left_offX = MIN(0, m_iLabelWidth - m_cImageSize.width);
		}
	}
	CCRect r = CCRectMake(0, 0, m_cImageSize.width, m_cImageSize.height);
	r.origin.x = -m_iString_left_offX;
	r.size.width = getStringViewLength();
	this->setImageRect(r);
    
    if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
    {
        //float mPmarkWidth = MIN(m_obContentSize.width, getCursorX());
        m_pCursorMark->setCenterOrigin(CCPoint(this->getImageRect().size.width + m_iHoriMargins, m_obContentSize.height / 2));
    }
    else
    {
        float mPmarkWidth = MIN(m_obContentSize.width, getCursorX());
        m_pCursorMark->setCenterOrigin(CCPoint(mPmarkWidth + m_iHoriMargins, m_obContentSize.height / 2));
    }
}

void CATextField::adjustCursorMoveForward()
{
    this->updateImage();

    m_iString_l_length = getStringLength(m_sText.substr(0, m_iCurPos));
    m_iString_r_length = m_cImageSize.width - m_iString_l_length;

    if (m_iString_l_length + m_iString_left_offX > m_iLabelWidth)
    {
        m_iString_left_offX = m_iLabelWidth - m_iString_l_length;
    }

    CCRect r = CCRectMake(0, 0, m_cImageSize.width, m_cImageSize.height);
    r.origin.x = -m_iString_left_offX;
    r.size.width = getStringViewLength();
    this->setImageRect(r);
    if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
    {
        //float mPmarkWidth = MIN(m_obContentSize.width, getCursorX());
        m_pCursorMark->setCenterOrigin(CCPoint(this->getImageRect().size.width + m_iHoriMargins, m_obContentSize.height / 2));
    }
    else
    {
        float mPmarkWidth = MIN(m_obContentSize.width, getCursorX());
        m_pCursorMark->setCenterOrigin(CCPoint(mPmarkWidth + m_iHoriMargins, m_obContentSize.height / 2));
    }
}

CCRect CATextField::getZZCRect(bool* l, bool* r)
{
	int l1 = getStringLength(m_sText.substr(0, m_curSelCharRange.first));
	int l2 = getStringLength(m_sText.substr(m_curSelCharRange.first, m_curSelCharRange.second-m_curSelCharRange.first));

	bool ll = (l1 + m_iString_left_offX) >= 0;
	bool rr = (l1 + m_iString_left_offX + l2) <= m_iLabelWidth;

	int dd = 0;
	if (ll && rr)
	{
		dd = l2;
	}
	else if (ll)
	{
		dd = m_iLabelWidth - (l1 + m_iString_left_offX);
	}
	else if (rr)
	{
		dd = l1 + m_iString_left_offX + l2;
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

	return CCRect((ll ? (l1 + m_iString_left_offX) : 0) + m_iHoriMargins, m_iVertMargins, dd, m_iFontHeight);
}


void CATextField::selectAll()
{
	if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
		return;

	m_curSelCharRange.first = 0;
	m_curSelCharRange.second = m_iCurPos = (int)m_sText.length();

	CATextSelectView* pSelCharsView = CATextSelectView::create();
	bool l, r;
	CCRect cc = getZZCRect(&l, &r);
	pSelCharsView->showTextSelView(convertRectToWorldSpace(cc), this, l, r);
	this->hideCursorMark();
//	CATextArrowView::hideTextArrowView();
}

void CATextField::moveSelectChars(bool isLeftBtn, const CCPoint& pt)
{
	int l, r, p;
	calculateSelChars(convertToNodeSpace(pt), l, r, p);

	if (isLeftBtn)
	{
		if (p < m_curSelCharRange.second)
			m_curSelCharRange.first = p;
	}
	else
	{
		if (p > m_curSelCharRange.first)
			m_curSelCharRange.second = p;
	}

	m_iString_l_length = l;
	m_iString_r_length = r;
	bool isBackward = p < m_iCurPos;
	m_iCurPos = p;
	if (isBackward)
	{
		adjustCursorMoveBackward();
	}
	else
	{
		adjustCursorMoveForward();
	}

	CATextSelectView* pSelCharsView = CATextSelectView::create();
	bool ll, rr;
	CCRect cc = convertRectToWorldSpace(getZZCRect(&ll, &rr));
	pSelCharsView->showTextSelView(cc, this, ll, rr);
	this->hideCursorMark();
	//CATextArrowView::hideTextArrowView();
}


void CATextField::moveArrowBtn(const CCPoint& pt)
{
	if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
		return;


    this->showCursorMark();
	
	int l, r, p;
	calculateSelChars(convertToNodeSpace(pt), l, r, p);
	m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);

	m_iString_l_length = l;
	m_iString_r_length = r;
	bool isBackward = p < m_iCurPos;
	m_iCurPos = p;
	if (isBackward)
	{
		adjustCursorMoveBackward();
	}
	else
	{
		adjustCursorMoveForward();
	}

	m_pCursorMark->setCenterOrigin(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height / 2));

//	CATextArrowView* pTextArrowView = CATextArrowView::create();
//	pTextArrowView->showTextArrView(convertToWorldSpace(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height + CATextArrowViewHeight/2)), this);
}

void CATextField::cursorMoveBackward()
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
	adjustCursorMoveBackward();
}

void CATextField::cursorMoveForward()
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
	adjustCursorMoveForward();
}

void CATextField::copyToClipboard()
{
	std::string cszText = m_sText.substr(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first);
	CAClipboard::setText(cszText);
}

void CATextField::cutToClipboard()
{
	copyToClipboard();

	execCurSelCharRange();
}


void CATextField::pasteFromClipboard()
{
	std::string cszText = CAClipboard::getText();
	insertText(cszText.c_str(), (int)cszText.size());

#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
	CCEGLView * pGlView = CAApplication::getApplication()->getOpenGLView();
	pGlView->setIMECursorPos(getCursorPos(), getContentText());
#endif
}

bool CATextField::execCurSelCharRange()
{
	CATextSelectView::hideTextSelectView();
	CATextToolBarView::hideTextToolBar();
	m_pTextViewMark->setVisible(false);

	if (m_curSelCharRange.first == m_curSelCharRange.second)
		return false;

	int iOldCurPos = m_curSelCharRange.first;
	std::string cszText = m_sText.erase(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first);
	setText(cszText);

	m_iCurPos = iOldCurPos;
	showCursorMark();
	adjustCursorMoveBackward();
	return true;
}

void CATextField::ccStartSelect()
{
	if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
		return;

	if (m_sText.empty())
		return;

	int index = getStringCharCount(m_sText.substr(0, m_iCurPos));
	if (index == 0)
	{
		m_curSelCharRange.first = m_iCurPos;
		m_curSelCharRange.second = m_iCurPos + m_vTextFiledChars.front().charSize;
	}
	else
	{
		m_curSelCharRange.first = m_iCurPos - m_vTextFiledChars[index - 1].charSize;
		m_curSelCharRange.second = m_iCurPos;
	}

	CATextSelectView* pSelCharsView = CATextSelectView::create();
	bool l, r;
	CCRect cc = getZZCRect(&l, &r);
	pSelCharsView->showTextSelView(convertRectToWorldSpace(cc), this, l, r);
	m_pCursorMark->setVisible(false);

//	CATextArrowView::hideTextArrowView();
}

const char* CATextField::getContentText()
{
    return m_sText.c_str();
}

int CATextField::getCursorPos()
{
    return getStringCharCount(m_sText.substr(0, m_iCurPos));
}

void CATextField::setBackgroundView(CrossApp::CAView *var)
{
    this->removeSubview(m_pBackgroundView);
    m_pBackgroundView = var;
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getBounds());
		this->insertSubview(m_pBackgroundView, -1);
    }
}

CAView* CATextField::getBackgroundView()
{
    return m_pBackgroundView;
}

void CATextField::setContentSize(const CCSize& var)
{
	CAView::setContentSize(var);
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getBounds());
    }
    
    float width = CAApplication::getApplication()->getWinSize().width;
	if (m_iHoriMargins==0)
	{
		m_iHoriMargins = BORDER_WIDTH(width);
	} 	if (m_iVertMargins==0)
	{
		m_iVertMargins = (m_obContentSize.height - m_iFontHeight) / 2;
	}
	m_iLabelWidth = m_obContentSize.width - 2 * m_iHoriMargins;
    this->initMarkSprite();
}

void CATextField::updateImage()
{
	std::string text = "";
    if (m_sText.empty())
    {
        text = m_sPlaceHolder;
        this->setColor(m_cSpaceHolderColor);
    }
	else
    {
        text = m_sText;
        this->setColor(m_cTextColor);
    }
    std::string password("");
	if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
	{
		for (std::string::size_type i = 0; i<m_sText.length(); i++)
		{
			password.append("*");
		}
        if (m_sText.empty())
        {
            text = m_sPlaceHolder;
        }
        else
        {
            text = password;
        }
		
	}

	float dt = 1.0f;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	dt = 1.1f;
#endif
	CCSize size = CCSizeMake(0, m_iFontHeight*dt);
    CAImage* image = CAImage::createWithString(text.c_str(),
											   m_nfontName.c_str(),
                                               m_iFontSize,
                                               size,
                                               CATextAlignmentLeft,
                                               CAVerticalTextAlignmentCenter,
                                               true);
    
    CCRect rect = CCRectZero;
    if (m_sPlaceHolder.length() == 0)
    {
        this->setImage(image);
        this->setImageRect(rect);
    }
    
	if (image == NULL) return;

	rect.size.height = image->getContentSize().height;
	rect.size.width = MIN(m_iLabelWidth, image->getContentSize().width);

	if (text.empty())
	{
		m_cImageSize = CCSizeZero;
	}
	else
	{
		m_cImageSize = image->getContentSize();
	}
	this->setImage(image);

	rect.origin.x = -m_iString_left_offX;
	this->setImageRect(rect);
}

void CATextField::setImageRect(const CCRect& rect)
{
    m_bRectRotated = false;
    setVertexRect(rect);
    setImageCoords(rect);
    
    if (m_pobBatchView)
    {
        // update dirty_, don't update recursiveDirty_
        setDirty(true);
    }
    else
    {
        this->updateImageRect();
    }
}

void CATextField::updateImageRect()
{
    GLfloat x1,x2,y1,y2;

	x1 = m_iHoriMargins;
	y1 = m_iVertMargins;
    x2=x1+m_obRect.size.width;
    y2=y1+m_obRect.size.height;
    m_sQuad.bl.vertices = vertex3(x1, y1, m_fVertexZ);
    m_sQuad.br.vertices = vertex3(x2, y1, m_fVertexZ);
    m_sQuad.tl.vertices = vertex3(x1, y2, m_fVertexZ);
    m_sQuad.tr.vertices = vertex3(x2, y2, m_fVertexZ);
}

void CATextField::setCursorColor(const CAColor4B &var)
{
    m_cCursorColor = var;
    if (m_pCursorMark)
    {
        m_pCursorMark->setColor(m_cCursorColor);
    }
}

const CAColor4B &CATextField::getCursorColor()
{
	return m_cCursorColor;
}

void CATextField::setHoriMargins(int var)
{
	int width = m_obContentSize.width;
	if (2 * var > width)
		return;
	m_iHoriMargins = var;
	m_iLabelWidth = m_obContentSize.width - 2 * m_iHoriMargins;
}

int CATextField::getHoriMargins()
{
	return m_iHoriMargins;
}

void CATextField::setVertMargins(int var)
{
	int height = m_obContentSize.height;
	CC_RETURN_IF(2 * var > height);
	m_iVertMargins = var;
	setFontSize(height - 2 * var - 2);
}

int CATextField::getVertMargins()
{
	return m_iVertMargins;
}

int CATextField::getCursorX()
{
	return m_iString_l_length + m_iString_left_offX;
}

int CATextField::getStringViewLength()
{
	return MIN(m_iLabelWidth, m_iString_r_length + getCursorX());
}

int CATextField::getStringCharCount(const std::string &var)
{
	int count = 0;
	for (std::string::size_type i = 0; i < var.size(); i++)
	{
		if (var[i] < 0 || var[i]>127)
		{
			i += 2;
		}
		count++;
	}
	return count;
}

void CATextField::getKeyBoardHeight(int height)
{
    CAView::becomeFirstResponder();
	
    if( m_pDelegate)
    {
        m_pDelegate->getKeyBoardHeight(height);
    }
}

void CATextField::getKeyBoradReturnCallBack()
{
    if( m_pDelegate && !m_pDelegate->keyBoardCallBack(this) )
    {
        return;
    }
    else
    {
        this->resignFirstResponder();
    }
}

void CATextField::keyboardDidShow(CCIMEKeyboardNotificationInfo& info)
{
    if (!m_isTouchInSide)
    {
        m_isTouchInSide = true;

    }
}

void CATextField::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);
    execCurSelCharRange();
}

void CATextField::keyboardDidHide(CCIMEKeyboardNotificationInfo& info)
{
    if(m_isTouchInSide)
    {
        m_isTouchInSide = false;
        CAView::resignFirstResponder();
    }
}

int CATextField::getStringLength(const std::string &var)
{
	return g_AFTFontCache.getStringWidth(m_nfontName.c_str(), m_iFontSize, var);
}

NS_CC_END