//
//  CATextField.cpp
//  project
//
//  Created by lh on 14-5-15.
//
//m_iCurPos

#include "CATextField.h"
#include "basics/CAApplication.h"
#include "actions/CCActionInterval.h"
#include "CCEGLView.h"
#include <locale>
#include <algorithm>
#include "shaders/CAShaderCache.h"
#include "platform/CAClipboard.h"
#include "basics/CAScheduler.h"

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
, m_iString_left_offX(0)
, m_iString_l_length(0)
, m_iString_r_length(0)
, m_iFontHeight(0)
, m_iHoriMargins(0)
, m_iVertMargins(0)
, m_pBackgroundView(NULL)
, m_pCurTouch(NULL)
, m_pCurEvent(NULL)
{
	m_iFontHeight = CAImage::getFontHeight("", m_iFontSize);
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
    }
    return result;
}

bool CATextField::becomeFirstResponder()
{
	bool result = CAView::becomeFirstResponder();
    if (result) 
	{
		attachWithIME();
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

    return true;
}

void CATextField::initMarkSprite()
{
    if (m_pCursorMark == NULL)
    {
        m_pCursorMark = CAView::create();
        m_pCursorMark->setColor(m_cCursorColor);
        m_pCursorMark->setVisible(false);
        this->addSubview(m_pCursorMark);
    }

    m_pCursorMark->setFrame(CCRect(m_iHoriMargins, 0, _px(2), CAImage::getFontHeight("", m_iFontSize)));
}

void CATextField::setFontSize(int var)
{
	CCAssert(m_pCursorMark != NULL, "");
    
	m_iFontSize = var;
	m_iFontHeight = CAImage::getFontHeight("", m_iFontSize);
    m_iVertMargins = (m_obContentSize.height - m_iFontHeight) / 2;
    
	m_pCursorMark->setFrame(CCRect(m_iHoriMargins, 0, _px(2), var));
    setText(m_sText);
    this->updateImage();
}

int CATextField::getFontSize()
{
	return m_iFontSize;
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
    insertText(var.c_str(), var.length());
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
            m_pCursorMark->setVisible(true);
            //m_pCursorMark->runAction(CCRepeat::create(CCBlink::create(1.0f, 1), 1048576));
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
            //m_pCursorMark->stopAllActions();
            m_pCursorMark->setVisible(false);

			CATextArrowView::hideTextArrowView();
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

void CATextField::ccTouchTimer(float interval)
{
	CAScheduler::unschedule(schedule_selector(CATextField::ccTouchTimer), this);
	ccTouchPress(m_pCurTouch, m_pCurEvent);
}

bool CATextField::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
		becomeFirstResponder();
		if (isFirstResponder())
        {
			m_pCursorMark->setVisible(true);

            //m_pCursorMark->runAction(CCRepeat::create(CCBlink::create(1.0f, 1), 1048576));
            if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
            {
                if (m_sText.empty())
                {
                    m_pCursorMark->setCenterOrigin(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height / 2));
                }
                return true;
            }
			calculateSelChars(point, m_iString_l_length, m_iString_r_length, m_iCurPos);
			m_curSelCharRange = std::make_pair(m_iCurPos, m_iCurPos);
           
			m_pCursorMark->setCenterOrigin(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height / 2));

			CATextArrowView* pTextArrowView = CATextArrowView::create();
			pTextArrowView->showTextArrView(convertToWorldSpace(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height + CATextArrowViewHeight/2)), this);
        }

#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
        CCEGLView * pGlView = CAApplication::getApplication()->getOpenGLView();
        pGlView->setIMECursorPos(getCursorPos(),getContentText());
#endif

    }
    else
    {
        if (resignFirstResponder())
        {
            //m_pCursorMark->stopAllActions();
			m_pCursorMark->setVisible(false);
			CATextArrowView::hideTextArrowView();
			this->updateImage();
        }
        return false;
    }
    
	CAScheduler::schedule(schedule_selector(CATextField::ccTouchTimer), this, 0, 0, 1.5f);

	m_pCurTouch = pTouch;
	m_pCurEvent = pEvent;

	return CAView::ccTouchBegan(pTouch, pEvent);
}

void CATextField::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	CAScheduler::unschedule(schedule_selector(CATextField::ccTouchTimer), this);
}

void CATextField::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	CAScheduler::unschedule(schedule_selector(CATextField::ccTouchTimer), this);
}

void CATextField::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
	CAScheduler::unschedule(schedule_selector(CATextField::ccTouchTimer), this);
}


void CATextField::ccTouchPress(CATouch *pTouch, CAEvent *pEvent)
{
	if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
		return;

	CATextToolBar* pTextEditView = NULL;
	if (m_sText.empty())
	{
		pTextEditView = CATextToolBar::createWithText(UTF8("粘贴"), NULL);
	}
	else
	{
		pTextEditView = CATextToolBar::createWithText(UTF8("粘贴"), UTF8("全选"), UTF8("选择"), NULL);
	}
	pTextEditView->setTarget(this, CATextToolBar_selector(CATextField::CATextEditBtnEvent));
	pTextEditView->showTextEditView(pTouch->getLocation());
}

void CATextField::CATextEditBtnEvent(int iButtonIndex)
{
	CATextToolBar::hideTextToolBar();

	if (iButtonIndex == 0)
	{
		pasteFromClipboard();
	}
	else if (iButtonIndex == 1)
	{
		selectAll();
	}
	else
	{
		startSelect();
	}
	
}

void CATextField::CATextEditBtnEvent2(int iButtonIndex)
{
	CATextToolBar::hideTextToolBar();

	if (iButtonIndex == 0)
	{
		cutToClipboard();
	}
	else if (iButtonIndex == 1)
	{
		copyToClipboard();
	}
	else
	{
		pasteFromClipboard();
	}
}

bool CATextField::canAttachWithIME()
{
    return (m_pDelegate) ? (! m_pDelegate->onTextFieldAttachWithIME(this)) : true;
}

bool CATextField::canDetachWithIME()
{
    return (m_pDelegate) ? (! m_pDelegate->onTextFieldDetachWithIME(this)) : true;
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
	execCurSelCharRange();
    CC_RETURN_IF(len <= 0);
    CC_RETURN_IF(text == 0);
    
    if (!strcmp(text, "\n")) {
        getKeyBoradReturnCallBack();
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CC_RETURN_IF(m_nInputType == KEY_BOARD_INPUT_PASSWORD && len >= 2);
#endif
    
    analyzeString(text, len);
    CC_RETURN_IF(m_pDelegate && m_pDelegate->onTextFieldInsertText(this, m_sText.c_str(), m_sText.length()));
    adjustCursorMoveForward();

	CATextArrowView::hideTextArrowView();
}

void CATextField::AndroidWillInsertText(int start,const char* str,int before,int count)
{
    CCAssert(str != NULL, "");
    CCAssert(count > 0, "");
    std::string cszNewStr = str;
    if (cszNewStr.size() >= m_sText.size())
    {
        for (int i = 0; i < start; i++)
        {
            if (cszNewStr[i] < 0 || cszNewStr[i]>127)
            {
                start += 2; i += 2;
            }
        }
        std::string cszStrInsert = cszNewStr.substr(start, cszNewStr.size() - m_sText.size());
        insertText(cszStrInsert.c_str(), cszStrInsert.size());
    }
}

void CATextField::willInsertText(const char *text, int len)
{
    
	
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
    CC_RETURN_IF(m_pDelegate && m_pDelegate->onTextFieldDeleteBackward(this, m_sText.c_str(), m_sText.length()));
	
	m_vTextFiledChars.erase(m_vTextFiledChars.begin() + getStringCharCount(m_sText.substr(0, m_iCurPos)));
    adjustCursorMoveBackward();
	CATextArrowView::hideTextArrowView();
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

CCRect CATextField::getZZCRect(bool& l, bool& r)
{
	int l1 = getStringLength(m_sText.substr(0, m_curSelCharRange.first));
	int l2 = getStringLength(m_sText.substr(m_curSelCharRange.first, m_curSelCharRange.second-m_curSelCharRange.first));

	l = (l1 + m_iString_left_offX) >= 0;
	r = (l1 + m_iString_left_offX + l2) <= m_iLabelWidth;

	int dd = 0;
	if (l && r)
	{
		dd = l2;
	}
	else if (l)
	{
		dd = m_iLabelWidth - (l1 + m_iString_left_offX);
	}
	else if (r)
	{
		dd = l1 + m_iString_left_offX + l2;
	}
	else dd = m_iLabelWidth;
	
	return CCRect((l ? (l1 + m_iString_left_offX) : 0) + m_iHoriMargins, m_iVertMargins, dd, m_iFontHeight);
}

void CATextField::startSelect()
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
		m_curSelCharRange.first = m_iCurPos - m_vTextFiledChars[index-1].charSize;
		m_curSelCharRange.second = m_iCurPos;
	}

	CATextSelectView* pSelCharsView = CATextSelectView::create();
	bool l, r;
	CCRect cc = getZZCRect(l, r);
	pSelCharsView->showTextSelView(convertRectToWorldSpace(cc), l, r);
	m_pCursorMark->setVisible(false);
	CATextArrowView::hideTextArrowView();
}

void CATextField::selectAll()
{
	if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
		return;

	m_curSelCharRange.first = 0;
	m_curSelCharRange.second = m_iCurPos = m_sText.length();


	CATextSelectView* pSelCharsView = CATextSelectView::create();
	bool l, r;
	CCRect cc = getZZCRect(l, r);
	pSelCharsView->showTextSelView(convertRectToWorldSpace(cc), l, r);
	m_pCursorMark->setVisible(false);
	CATextArrowView::hideTextArrowView();

	CATextToolBar* pTextToolBar = CATextToolBar::createWithText(UTF8("剪切"), UTF8("拷贝"), UTF8("粘贴"), NULL);
	pTextToolBar->setTarget(this, CATextToolBar_selector(CATextField::CATextEditBtnEvent2));
	pTextToolBar->showTextEditView(cc.origin);
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
	CCRect cc = convertRectToWorldSpace(getZZCRect(ll, rr));
	pSelCharsView->showTextSelView(cc, ll, rr);
	m_pCursorMark->setVisible(false);
	CATextArrowView::hideTextArrowView();
}

void CATextField::moveSelectCharsCancel(const CCPoint& pt)
{
	CATextToolBar* pTextEditView = CATextToolBar::createWithText(UTF8("剪切"), UTF8("拷贝"), UTF8("粘贴"), NULL);
	pTextEditView->setTarget(this, CATextToolBar_selector(CATextField::CATextEditBtnEvent2));
	pTextEditView->showTextEditView(pt);
}

void CATextField::moveArrowBtn(const CCPoint& pt)
{
	if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
		return;


	m_pCursorMark->setVisible(true);
	//m_pCursorMark->runAction(CCRepeat::create(CCBlink::create(1.0f, 1), 1048576));
	
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

	CATextArrowView* pTextArrowView = CATextArrowView::create();
	pTextArrowView->showTextArrView(convertToWorldSpace(CCPoint(getCursorX() + m_iHoriMargins, m_obContentSize.height + CATextArrowViewHeight/2)), this);
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
	CAClipboard::SetText(cszText);
}

void CATextField::cutToClipboard()
{
	copyToClipboard();

	execCurSelCharRange();
}


void CATextField::pasteFromClipboard()
{
	std::string cszText = CAClipboard::GetText();
	insertText(cszText.c_str(), cszText.size());

#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
	CCEGLView * pGlView = CAApplication::getApplication()->getOpenGLView();
	pGlView->setIMECursorPos(getCursorPos(), getContentText());
#endif
}

bool CATextField::execCurSelCharRange()
{
	if (m_curSelCharRange.first == m_curSelCharRange.second)
		return false;

	int iOldCurPos = m_curSelCharRange.first;
	std::string cszText = m_sText.erase(m_curSelCharRange.first, m_curSelCharRange.second - m_curSelCharRange.first);
	setText(cszText);

	CATextSelectView::hideTextSelectView();
	CATextToolBar::hideTextToolBar();

	m_iCurPos = iOldCurPos;
	m_pCursorMark->setVisible(true);
	adjustCursorMoveBackward();
	return true;
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
	}
	if (m_iVertMargins==0)
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
	CCSize size = CCSizeMake(0, m_iFontHeight);
    CAImage* image = CAImage::createWithString(text.c_str(),
                                               "",
                                               m_iFontSize * CC_CONTENT_SCALE_FACTOR(),
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
	if (image == NULL )
		return;

 
	
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
    m_sQuad.bl.vertices = vertex3(x1, y1, 0);
    m_sQuad.br.vertices = vertex3(x2, y1, 0);
    m_sQuad.tl.vertices = vertex3(x1, y2, 0);
    m_sQuad.tr.vertices = vertex3(x2, y2, 0);
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
    if( m_pDelegate && m_pDelegate->getKeyBoardHeight(height) )
    {
        return;
    }
}

void CATextField::getKeyBoradReturnCallBack()
{
    if( m_pDelegate && m_pDelegate->keyBoardCallBack(this) )
    {
        return;
    }
    else
    {
        this->resignFirstResponder();
    }
}
void CATextField::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    this->resignFirstResponder();
}

int CATextField::getStringLength(const std::string &var)
{
    return g_AFTFontCache.getStringWidth("", m_iFontSize, var);
}
NS_CC_END