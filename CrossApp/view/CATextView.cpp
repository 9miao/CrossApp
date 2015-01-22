#include "CATextView.h"
#include "basics/CAApplication.h"
#include "view/CAWindow.h"
#include "actions/CCActionInterval.h"
#include "CCEGLView.h"
#include <utility>
#include "shaders/CAShaderCache.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
#include "platform/CAFTFontCache.h"
#endif

NS_CC_BEGIN

#pragma CATextView

CATextView::CATextView()
: m_pTextViewDelegate(NULL)
, m_pBackgroundView(NULL)
, m_pCursorMark(NULL)
, m_pImageView(NULL)
, m_cCursorColor(CAColor_black)
, m_cTextColor(ccc4(0, 0, 0, 255))
, m_cSpaceHolderColor(ccc4(193, 193, 193, 255))
, m_nInputType(KEY_BOARD_INPUT_NORMAL)
, m_keyboardType(KEY_BOARD_TYPE_NORMAL)
, m_keyBoardReturnType(KEY_BOARD_RETURN_DONE)
, m_szFontName("")
, m_iFontSize(24)
, m_iCurPos(0)
, m_bUpdateImage(false)
, m_iLineSpacing(0)
, m_bWordWrap(true)
{
	m_iLineHeight = CAImage::getFontHeight(m_szFontName.c_str(), m_iFontSize);
}


CATextView::~CATextView()
{
    m_pTextViewDelegate = NULL;
}

void CATextView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();
	CC_RETURN_IF(m_sPlaceHolder.empty());
	this->updateImage();

}

bool CATextView::resignFirstResponder()
{
	bool result = CAView::resignFirstResponder();
	if (result)
	{
		detachWithIME();
	}
	return result;
}

bool CATextView::becomeFirstResponder()
{
	bool result = CAView::becomeFirstResponder();
	if (result)
	{
		attachWithIME();
	}
	return result;
}

CATextView* CATextView::createWithFrame(const CCRect& rect)
{
	CATextView* pTextView = new CATextView();
	if (pTextView && pTextView->initWithFrame(rect))
	{
		pTextView->autorelease();
		return pTextView;
	}
	CC_SAFE_DELETE(pTextView);
	return NULL;
}

CATextView* CATextView::createWithCenter(const CCRect& rect)
{
	CATextView* pTextView = new CATextView();
	if (pTextView && pTextView->initWithCenter(rect))
	{
		pTextView->autorelease();
		return pTextView;
	}
	CC_SAFE_DELETE(pTextView);
	return NULL;
}

bool CATextView::init()
{
	if (!CAScrollView::init())
	{
		return false;
	}

	this->setShowsHorizontalScrollIndicator(false);
	this->setBounceHorizontal(false);
	this->setTouchMovedListenHorizontal(false);

    m_pImageView = new CAImageView();
    m_pImageView->setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureA8Color));
    m_pImageView->autorelease();
	this->addSubview(m_pImageView);
	return true;
}


bool CATextView::initWithFrame(const CCRect& frame)
{
	if (!CAScrollView::initWithFrame(frame))
	{
		return false;
	}
	initMarkSprite();
	return true;
}

bool CATextView::initWithCenter(const CCRect& rect)
{
	if (!CAScrollView::initWithCenter(rect))
	{
		return false;
	}
	initMarkSprite();
	return true;
}

void CATextView::initMarkSprite()
{
	if (m_pCursorMark == NULL)
	{
		m_pCursorMark = CAView::create();
		m_pCursorMark->setColor(m_cCursorColor);
		m_pCursorMark->setVisible(false);
		m_pCursorMark->stopAllActions();
		this->addSubview(m_pCursorMark);

		m_pCursorMark->runAction(CCRepeatForever::create((CCActionInterval *)CCSequence::create(CCFadeOut::create(0.5f), CCFadeIn::create(0.5f), NULL)));
	}
	m_pCursorMark->setFrame(CCRect(0, 0, 2, m_iLineHeight));
}

void CATextView::updateImage()
{
	std::string text;
	if (m_szText.empty())
	{
		text = m_sPlaceHolder;
	}
	else
	{
		text = m_szText;
	}

	m_cFontColor = m_szText.empty() ? m_cSpaceHolderColor : m_cTextColor;
	if (m_pImageView)
	{
		m_pImageView->setColor(m_cFontColor);
	}

	float width = this->getBounds().size.width;

	CAImage* image = NULL;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)

	image = g_AFTFontCache.initWithStringEx(text.c_str(),
		m_szFontName.c_str(),
		m_iFontSize,
		width,
		0,
		m_vLinesTextView,
		m_iLineSpacing,
		m_bWordWrap);
#endif

	if (image == NULL || m_szText.empty())
	{
		m_vLinesTextView.clear();
	}
    m_pImageView->setColor(CAColor_black);
	m_pImageView->setImage(image);
    CCRect rect = CCRectZero;
    rect.size = image->getContentSize();
    m_pImageView->setImageRect(rect);
    m_pImageView->setFrame(rect);
	setViewSize(rect.size);
	calcCursorPosition();
}


void CATextView::calcCursorPosition()
{
	int iCurLine = -1, iCurLineCharPos = 0;

	for (int i = 0; i < m_vLinesTextView.size(); i++)
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

	CCPoint cCurPosition;
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

	float w = getBounds().size.height;
	float y = cCurPosition.y - getContentOffset().y;
	if (y < 0 || y > w)
	{
		y = y < 0 ? fHalfLineHeight : w - fHalfLineHeight;
		setContentOffset(CCPointMake(0, cCurPosition.y-y), true);
	}
}


void CATextView::setFontSize(int var)
{
	m_iFontSize = var;
	m_iLineHeight = CAImage::getFontHeight(m_szFontName.c_str(), m_iFontSize);
	initMarkSprite();
	m_bUpdateImage = true;
}

int CATextView::getFontSize()
{
	return m_iFontSize;
}

void CATextView::setText(const std::string& var)
{
	CATextViewDelegate* pTemp = m_pTextViewDelegate;
	m_pTextViewDelegate = NULL;
	m_szText.clear();
	m_iCurPos = 0;
	m_vLinesTextView.clear();
	insertText(var.c_str(), var.length());
	m_pTextViewDelegate = pTemp;
}

const std::string& CATextView::getText()
{
	return m_szText;
}

void CATextView::setPlaceHolder(const std::string& var)
{
	m_sPlaceHolder = var;
	this->updateImage();
}

const std::string& CATextView::getPlaceHolder()
{
	return m_sPlaceHolder;
}

void CATextView::setSpaceHolderColor(CAColor4B var)
{
	m_cSpaceHolderColor = var;
	this->updateImage();
}

CAColor4B CATextView::getSpaceHolderColor()
{
	return m_cSpaceHolderColor;
}

void CATextView::setLineSpacing(unsigned int var)
{
	m_iLineSpacing = var;
	m_bUpdateImage = true;
}

unsigned int CATextView::getLineSpacing()
{
	return m_iLineSpacing;
}

void CATextView::setWordWrap(bool var)
{
	m_bWordWrap = var;
	m_bUpdateImage = true;
}

bool CATextView::getWordWrap()
{
	return m_bWordWrap;
}

void CATextView::setFontName(const std::string& var)
{
	m_szFontName = var;
	m_iLineHeight = CAImage::getFontHeight(m_szFontName.c_str(), m_iFontSize);
	initMarkSprite();
	m_bUpdateImage = true;
}

const std::string& CATextView::getFontName()
{
	return m_szFontName;
}

void CATextView::setCursorColor(CAColor4B var)
{
	m_cCursorColor = var;
	if (m_pCursorMark)
	{
		m_pCursorMark->setColor(m_cCursorColor);
	}
}

CAColor4B CATextView::getCursorColor()
{
	return m_cCursorColor;
}

void CATextView::setTextColor(CAColor4B var)
{
	m_cTextColor = var;
	this->updateImage();
}

CAColor4B CATextView::getTextColor()
{
	return m_cTextColor;
}

void CATextView::setFontColor(CAColor4B var)
{
	m_cFontColor = var;
	if (m_pImageView)
	{
		m_pImageView->setColor(m_cFontColor);
	}
	m_bUpdateImage = true;
}

CAColor4B CATextView::getFontColor()
{
	return m_cFontColor;
}

bool CATextView::canAttachWithIME()
{
	return (m_pTextViewDelegate) ? m_pTextViewDelegate->onTextViewAttachWithIME(this) : true;
}

bool CATextView::canDetachWithIME()
{
	return (m_pTextViewDelegate) ? m_pTextViewDelegate->onTextViewDetachWithIME(this) : true;
}

void CATextView::insertText(const char * text, int len)
{
	m_szText.insert(m_iCurPos, text, len);
 	m_iCurPos += len;
	updateImage();
}

void CATextView::willInsertText(const char* text, int len)
{

}

void CATextView::AndroidWillInsertText(int start, const char* str, int before, int count)
{
	CCAssert(str != NULL, "");
	CCAssert(count > 0, "");
	std::string cszNewStr = str;
	if (cszNewStr.size() >= m_szText.size())
	{
		cszNewStr = cszNewStr.substr(m_szText.size(), cszNewStr.size());
		insertText(cszNewStr.c_str(), cszNewStr.size());
	}
}

void CATextView::deleteBackward()
{
	if (m_iCurPos == 0 || m_szText.empty())
		return;

	CC_RETURN_IF(m_pTextViewDelegate && m_pTextViewDelegate->onTextViewDeleteBackward(this, m_szText.c_str(), m_szText.length()));

	int nDeleteLen = 1;
	while (0x80 == (0xC0 & m_szText.at(m_iCurPos - nDeleteLen)))
	{
		++nDeleteLen;
	}

	m_szText.erase(m_iCurPos - nDeleteLen, nDeleteLen);
	m_iCurPos -= nDeleteLen;
	updateImage();
}

void CATextView::getKeyBoardHeight(int height)
{
	if (m_pTextViewDelegate && m_pTextViewDelegate->getKeyBoardHeight(height))
	{
		return;
	}
}

void CATextView::getKeyBoradReturnCallBack()
{
	if (m_pTextViewDelegate && m_pTextViewDelegate->keyBoardCallBack(this))
	{
		return;
	}
	else
	{
		this->resignFirstResponder();
	}
}

void CATextView::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
	this->resignFirstResponder();
}

const char* CATextView::getContentText()
{
	return m_szText.c_str();
}

float CATextView::maxSpeed(float dt)
{
    return (CCPoint(m_obContentSize).getLength() * 8 * dt);
}

float CATextView::maxSpeedCache(float dt)
{
    return (maxSpeed(dt) * 3.0f);
}

float CATextView::decelerationRatio(float dt)
{
    return 2.0f * dt;
}

bool CATextView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	if (m_pTouches->count() > 0)
	{
		m_pTouches->replaceObjectAtIndex(0, pTouch);
	}

	if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
		return false;

	CCPoint point = this->convertTouchToNodeSpace(pTouch);

	if (this->getBounds().containsPoint(point))
	{
		becomeFirstResponder();
		if (isFirstResponder())
		{
			m_pCursorMark->setVisible(true);
			m_pCursorMark->runAction(CCRepeat::create(CCBlink::create(1.0f, 1), 1048576));

			point.y += getContentOffset().y;
			int iCurLine = point.y / (m_iLineHeight*1.25f);
			if (m_vLinesTextView.empty())
			{
				iCurLine = 0;
			}
			else if (iCurLine >= m_vLinesTextView.size())
			{
				iCurLine = m_vLinesTextView.size() - 1;
			}
			
			int iHalfCharSize = 0;
			int iStartPos = 0;
			if (!m_vLinesTextView.empty())
			{
				m_iCurPos = iStartPos = m_vLinesTextView[iCurLine].iStartCharPos;
				std::vector<TextAttribute>& v = m_vLinesTextView[iCurLine].TextAttrVect;
				for (int i = 0, iStringLeftX = 0; i < v.size(); i++)
				{
					TextAttribute& t = v[i];
					if (point.x >= iStringLeftX - iHalfCharSize && point.x < iStringLeftX + t.charlength / 2)
					{
						break;
					}

					iStringLeftX += t.charlength;
					m_iCurPos += t.charSize;
					iHalfCharSize = t.charlength / 2;
				}
			}

			std::string s = m_szText.substr(iStartPos, m_iCurPos - iStartPos);
			if (!s.empty() && s[0] == '\n')
			{
				s.erase(0, 1);
			}
			m_pCursorMark->setCenterOrigin(CCPoint(getStringLength(s), m_iLineHeight*1.25f*iCurLine + m_iLineHeight / 2));
		}
		return true;
	}
	else
	{
		if (resignFirstResponder())
		{
			m_pCursorMark->setVisible(false);
			m_pCursorMark->stopAllActions();
			return false;
		}
		return false;
	}

	return true;
}

bool CATextView::attachWithIME()
{
	bool bRet = CAIMEDelegate::attachWithIME();
	if (bRet)
	{
		// open keyboard
		CCEGLView * pGlView = CAApplication::getApplication()->getOpenGLView();
		if (pGlView)
		{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)        
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

			if (getKeyboardReturnType() == KEY_BOARD_RETURN_SEND)
			{
				pGlView->setIMEKeyboardReturnSend();
			}
			else if (getKeyboardReturnType() == KEY_BOARD_RETURN_SEARCH)
			{
				pGlView->setIMEKeyboardReturnSearch();
			}
			else if (getKeyboardReturnType() == KEY_BOARD_RETURN_DONE)
			{
				pGlView->setIMEKeyboardReturnDone();
			}
#endif
			m_pCursorMark->setVisible(true);
			m_pCursorMark->runAction(CCRepeat::create(CCBlink::create(1.0f, 1), 1048576));
			pGlView->setIMEKeyboardState(true);
		}
	}
	return bRet;
}

bool CATextView::detachWithIME()
{
	bool bRet = CAIMEDelegate::detachWithIME();
	if (bRet)
	{
		// close keyboard
		CCEGLView * pGlView = CAApplication::getApplication()->getOpenGLView();
		if (pGlView)
		{
			pGlView->setIMEKeyboardState(false);
			m_pCursorMark->setVisible(false);
			m_pCursorMark->stopAllActions();
		}
	}
	return bRet;
}

int CATextView::getStringLength(const std::string &var)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
	return g_AFTFontCache.getStringWidth(m_szFontName.c_str(), m_iFontSize, var);
#else
	CAImage *image = CAImage::createWithString(var.c_str(),
		"",
		m_iFontSize,
		CCSizeZero,
		CATextAlignmentLeft,
		CAVerticalTextAlignmentCenter);
	if (image != NULL)
	{
		return image->getContentSize().width;
	}
	return 0;
#endif
}


void CATextView::visit()
{
	if (m_bUpdateImage)
	{
		m_bUpdateImage = false;
		this->updateImage();
	}
	CAScrollView::visit();
}

NS_CC_END