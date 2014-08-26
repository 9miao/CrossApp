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

#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC) && (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
#include "platform/CAFTFontCache.h"
#include "support/ConvertUTF.h"
#endif

NS_CC_BEGIN

#define BORDER_WIDTH(w) (0.01125*w)

CATextField::CATextField()
: m_pDelegate(NULL)
, m_cTextColor(ccc4(0, 0, 0, 255))
, m_cSpaceHolderColor(ccc4(193, 193, 193, 255))
, m_iFontSize(24)
, m_nInputType(KEY_BOARD_INPUT_NORMAL)
, m_keyboardType(KEY_BOARD_TYPE_NORMAL)
, m_sText("")
, m_cCursorColor(CAColor_black)
, m_pBackgroundView(NULL)
, m_iCurPos(0)
, m_iLabelWidth(0)
, m_pCursorAction(NULL)
, m_pCursorMark(NULL)
, m_iString_left_offX(0)
, m_iString_l_length(0)
, m_iString_r_length(0)
, m_iFontHeight(0)
, m_iHoriMargins(0)
, m_iVertMargins(0)
{

	m_iFontHeight = CAImage::getFontHeight("", m_iFontSize);

}

CATextField::~CATextField()
{
}

void CATextField::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    CC_RETURN_IF(m_sPlaceHolder.empty());
    this->updateImage();
    
}

void CATextField::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
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

bool CATextField::initWithFrame(const CCRect& frame)
{
    if (!CAControl::initWithFrame(frame,CAColor_red))
	{
        return false;
    }

    float width = CAApplication::getApplication()->getWinSize().width;
    this->setBackGroundImage(CAImage::create("source_material/textField_bg.png"));

	if (m_iHoriMargins==0)
	{
		m_iHoriMargins = BORDER_WIDTH(width);
	}
	if (m_iVertMargins==0)
	{
		m_iVertMargins = (this->getBounds().size.height - m_iFontHeight) / 2;
	}
	m_iLabelWidth = this->getBounds().size.width - 2 * m_iHoriMargins;
    this->initMarkSprite();
    return true;
}

bool CATextField::initWithCenter(const CCRect& rect)
{
    if (!CAControl::initWithCenter(rect))
	{
        return false;
    }

	float width = CAApplication::getApplication()->getWinSize().width;
    this->setBackGroundImage(CAImage::create("source_material/textField_bg.png"));
	if (m_iHoriMargins == 0)
	{
		m_iHoriMargins = BORDER_WIDTH(width);
	}
	if (m_iVertMargins == 0)
	{
		m_iVertMargins = (this->getBounds().size.height - m_iFontHeight) / 2;
	}
	m_iLabelWidth = this->getBounds().size.width - 2 * m_iHoriMargins;
    this->initMarkSprite();
    return true;
}

void CATextField::initMarkSprite()
{
    m_pCursorMark = CAView::create();
	m_pCursorMark->setColor(m_cCursorColor);
	m_pCursorMark->setVisible(false);

	m_pCursorMark->setFrame(CCRect(m_iHoriMargins, 0, 2, CAImage::getFontHeight("", m_iFontSize)));
	this->addSubview(m_pCursorMark);
    
    m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.5f), CCFadeIn::create(0.5f), NULL));
	m_pCursorMark->runAction(m_pCursorAction);
}

void CATextField::setFontSize(int var)
{
	CCAssert(m_pCursorMark != NULL, "");
    
	m_iFontSize = var;
	m_iFontHeight = CAImage::getFontHeight("", m_iFontSize);

	m_pCursorMark->setFrame(CCRect(m_iHoriMargins, 0, var / 10.0f, var));
    this->updateImage();
}

int CATextField::getFontSize()
{
	return m_iFontSize;
}

void CATextField::setText(std::string var)
{
    CATextFieldDelegate* pTemp = m_pDelegate;
    m_pDelegate = NULL;
    m_sText.clear();
    insertText(var.c_str(), var.length());
    m_pDelegate = pTemp;
}

std::string CATextField::getText()
{
    return m_sText;
}

void CATextField::setPlaceHolder(std::string var)
{
    m_sPlaceHolder = var;
    this->updateImage();
}

std::string CATextField::getPlaceHolder()
{
    return m_sPlaceHolder;
}

void CATextField::setSpaceHolderColor(CAColor4B var)
{
    m_cSpaceHolderColor = var;
    this->updateImage();
}

CAColor4B CATextField::getSpaceHolderColor()
{
    return m_cSpaceHolderColor;
}

void CATextField::setTextColor(CAColor4B var)
{
    m_cTextColor = var;
    this->updateImage();
}

CAColor4B CATextField::getTextColor()
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
#endif
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

bool CATextField::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
		becomeFirstResponder();
		if (isFirstResponder())
        {
			m_pCursorMark->setVisible(true);
			int dtValue = point.x - m_iString_left_offX - m_iHoriMargins;

			m_iString_l_length = 0;
			m_iCurPos = 0;
			for (int i = 0; i < m_vTextFiledChars.size(); i++)
			{
				TextAttribute& t = m_vTextFiledChars[i];
				if (m_iString_l_length + t.charlength / 2>dtValue && getCursorX() + m_iHoriMargins>0)
				{
					break;
				}
				m_iString_l_length += t.charlength;
				m_iCurPos += t.charSize;
			}

			m_iString_r_length = m_cImageSize.width - m_iString_l_length;

			m_pCursorMark->setCenterOrigin(CCPoint(getCursorX() + m_iHoriMargins, this->getBounds().size.height / 2));
        }
        return true;
    }
    else
    {
        if (resignFirstResponder())
        {
			m_pCursorMark->setVisible(false);
			this->updateImage();
            return false;
        }
        return false;
    }
    
    return true;
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
}

void CATextField::insertText(const char * text, int len)
{
	CCAssert(text != NULL, "");
	CCAssert(len > 0, "");
    if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
	{
		if (len >= 2)
			return;
	}
    
    analyzeString(text, len);
    CC_RETURN_IF(m_pDelegate && m_pDelegate->onTextFieldInsertText(this, m_sText.c_str(), m_sText.length()));
    
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
        //float mPmarkWidth = MIN(this->getBounds().size.width, getCursorX());
        m_pCursorMark->setCenterOrigin(CCPoint(this->getImageRect().size.width + m_iHoriMargins, this->getBounds().size.height / 2));
    }else
    {
        float mPmarkWidth = MIN(this->getBounds().size.width, getCursorX());
        m_pCursorMark->setCenterOrigin(CCPoint(mPmarkWidth + m_iHoriMargins, this->getBounds().size.height / 2));
    }
}
void CATextField::AndroidWillInsertText(int start,const char* str,int before,int count)
{
    CCAssert(str != NULL, "");
	CCAssert(count > 0, "");
    if (strlen(str)>=m_sText.length())
    {
        m_vTextFiledChars.clear();
        
        m_sText = "";
        m_iCurPos = 0;
        m_iString_l_length = 0;
        m_iString_r_length = 0;
        insertText(str, strlen(str));
    }else
    {
        deleteBackward();
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
		this->updateImage();
		m_iString_l_length = 0;
		m_iString_r_length = 0;
        m_iString_left_offX= 0;
		m_iCurPos = 0;
		m_pCursorMark->setCenterOrigin(CCPoint(m_iHoriMargins + getCursorX(), this->getBounds().size.height / 2));
        
        return;
    }

	if (m_iCurPos==0 || m_sText.empty())
		return;

	int nDeleteLen = 1;
	while (0x80 == (0xC0 & m_sText.at(m_iCurPos - nDeleteLen)))
	{
		++nDeleteLen;
	}
    
    m_sText.erase(m_iCurPos - nDeleteLen, nDeleteLen);
    m_iCurPos -= nDeleteLen;
    CC_RETURN_IF(m_pDelegate && m_pDelegate->onTextFieldDeleteBackward(this, m_sText.c_str(), m_sText.length()));

	m_vTextFiledChars.erase(m_vTextFiledChars.begin() + getStringCharCount(m_sText.substr(0, m_iCurPos)));

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

	float mPmarkWidth = MIN(this->getBounds().size.width, getCursorX());
	m_pCursorMark->setCenterOrigin(CCPoint(mPmarkWidth + m_iHoriMargins, this->getBounds().size.height / 2));

	if (m_sText.empty())
	{
		updateImage();
	}
}
const char* CATextField::getContentText()
{
    return m_sText.c_str();
}
void CATextField::setBackGroundImage(CAImage *image)
{
	if (m_pBackgroundView == NULL)
	{
		m_pBackgroundView = CAScale9ImageView::create();
		m_pBackgroundView->setFrame(this->getBounds());
		this->insertSubview(m_pBackgroundView, -1);
	}
	m_pBackgroundView->setImage(image);
}

CAImage *CATextField::getBackGroundImage()
{
    return m_pBackgroundView ? m_pBackgroundView->getImage() : NULL;
}

void CATextField::setContentSize(const CCSize& var)
{
    CAControl::setContentSize(var);
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
		
		for (int i = 0; i<m_sText.length(); i++)
		{
			password.append("*");
		}
		
		if (m_sText.length() == 0)
		{
			password = m_sPlaceHolder;
		}
		text = password;
		this->setColor(m_cSpaceHolderColor);
	}
	CCSize size = CCSizeMake(0, m_iFontHeight);
    CAImage* image = CAImage::createWithString(text.c_str(),
                                               "",
                                               m_iFontSize * CC_CONTENT_SCALE_FACTOR(),
                                               size,
                                               CATextAlignmentLeft,
                                               CAVerticalTextAlignmentCenter, true);

	if (image == NULL)
		return;

 
	CCRect rect = CCRectZero;
	rect.size.height = image->getContentSize().height;
	rect.size.width = MIN(m_iLabelWidth, image->getContentSize().width);

	if (m_sText.empty())
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

void CATextField::setCursorColor(CAColor4B var)
{
    m_cCursorColor = var;
    if (m_pCursorMark)
    {
        m_pCursorMark->setColor(m_cCursorColor);
    }
}

CAColor4B CATextField::getCursorColor()
{
	return m_cCursorColor;
}

void CATextField::setHoriMargins(int var)
{
	int width = this->getBounds().size.width;
	if (2 * var > width)
		return;
	m_iHoriMargins = var;
	m_iLabelWidth = this->getBounds().size.width - 2 * m_iHoriMargins;
}

int CATextField::getHoriMargins()
{
	return m_iHoriMargins;
}

void CATextField::setVertMargins(int var)
{
	int height = this->getBounds().size.height;
	if (2 * var > height)
		return;
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
	for (int i = 0; i < var.size(); i++)
	{
		if (var[i] < 0 || var[i]>127)
		{
			i += 2;
		}
		count++;
	}
	return count;
}

int CATextField::getStringLength(const std::string &var)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC) && (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
	return g_AFTFontCache.getStringWidth("", m_iFontSize, var);
#else
    CAImage *image = CAImage::createWithString(var.c_str(),
                                               "",
                                               m_iFontSize,
                                               CCSizeZero,
                                               CATextAlignmentLeft,
                                               CAVerticalTextAlignmentCenter);
    if(image!=NULL)
    {
        return image->getContentSize().width;
    }
    return 0;
#endif
}
NS_CC_END