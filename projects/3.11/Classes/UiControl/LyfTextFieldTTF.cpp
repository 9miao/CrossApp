//  出自九秒社区 http://www.9miao.com
//
//  LTextField.cpp
//  mir-mobile
//
//  Created by 栗元峰 on 13-12-12.
//
//

#include "LyfTextFieldTTF.h"

NS_CC_BEGIN

const static float DELTA = 20.0f;

static int _calcCharCount(const char * pszText) {
	int n = 0;
	char ch = 0;
	while ((ch = *pszText)) {
		CC_BREAK_IF(! ch);
        
		if (0x80 != (0xC0 & ch)) {
			++n;
		}
		++pszText;
	}
	return n;
}

LyfTextFieldTTF::LyfTextFieldTTF()
:m_pCursorSprite(0)
,m_pCursorAction(0)
,m_nMaxLength(60)
,m_nSize(CCSizeZero)
,m_beginPos(CCPointZero)
,m_isInputState(false)
,m_nbSwallowsTouches(false)
,m_IsRehisterTouchDispatcher(false)
,m_nType(0)
{
    
}

LyfTextFieldTTF::~LyfTextFieldTTF() {
}

LyfTextFieldTTF * LyfTextFieldTTF::textFieldWithPlaceHolder(
                                                                      const char *placeholder, const CCSize& dimensions,
                                                                      CCTextAlignment alignment, const char *fontName, float fontSize) {
    
	LyfTextFieldTTF *pRet = new LyfTextFieldTTF();
    
	if (pRet && pRet->initWithPlaceHolder("", CCSizeZero, alignment, fontName,
                                          fontSize))
    {
        pRet->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		pRet->initCursor(fontSize);
        pRet->setSize(dimensions);
        if (placeholder)
        {
			pRet->setPlaceHolder(placeholder);
		}
        pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

LyfTextFieldTTF * LyfTextFieldTTF::textFieldWithPlaceHolder(
                                                                      const char *placeholder, const char *fontName, float fontSize) {
    
	LyfTextFieldTTF *pRet = new LyfTextFieldTTF();
    
	if (pRet && pRet->initWithString("", fontName, fontSize))
    {
        pRet->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		pRet->initCursor(fontSize);
        if (placeholder)
        {
			pRet->setPlaceHolder(placeholder);
		}
        pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void LyfTextFieldTTF::setPlaceHolder(const char *text){
    
    CCTextFieldTTF::setPlaceHolder(text);
    CCRect rect=CCRectZero;
    rect.origin.y=(this->getContentSize().height-this->getSize().height)/2;
    rect.size=CCSize(this->getTextureRect().size.width, this->getSize().height);
    this->setTextureRect(rect);
    this->setContentSize(this->getSize());
}

void LyfTextFieldTTF::initCursor(int height) {
	// 初始化光标
    
	int column = 4;
	int pixels[height][column];
    
	for (int i = 0; i < height; ++i)
    {
		for (int j = 0; j < column; ++j)
        {
			pixels[i][j] = 0xffffffff;
		}
	}
    
	CCTexture2D *texture = new CCTexture2D();
	texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1,
                          CCSizeMake(column, height));
    
	m_pCursorSprite = CCSprite::createWithTexture(texture);
	CCSize winSize = getContentSize();
	CCPoint cursorPos = ccp(0, winSize.height / 2);
	m_pCursorSprite->setPosition(cursorPos);
	this->addChild(m_pCursorSprite);
    
	m_pCursorAction = CCRepeatForever::create(
                                              (CCActionInterval *) CCSequence::create(CCFadeOut::create(0.40f),
                                                                                      CCFadeIn::create(0.40f), NULL));
    
	m_pCursorSprite->runAction(m_pCursorAction);
	m_pCursorSprite->setVisible(false);
}

void LyfTextFieldTTF::setContentSize(const cocos2d::CCSize &contentSize){
    
    CCTextFieldTTF::setContentSize(contentSize);
}

void LyfTextFieldTTF::onEnter() {
    
	CCTextFieldTTF::onEnter();
	this->registerWithTouchDispatcher();
}

void LyfTextFieldTTF::onExit() {
    
	this->detachWithIME();
	this->unRegisterWithTouchDispatcher();
    CCTextFieldTTF::onExit();
}

bool LyfTextFieldTTF::isInTextField(CCTouch *pTouch) {
    
    CCSize size = getSize();
	CCRect rect =
    CCRect(0 - size.width * getAnchorPoint().x, 0 - size.height * getAnchorPoint().y, size.width, size.height);
	CCPoint pTouchPt = convertTouchToNodeSpaceAR(pTouch);
	return rect.containsPoint(pTouchPt);
}

bool LyfTextFieldTTF::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    
    m_touchBeginUIPoint = pTouch->getLocationInView();
	m_beginPos = pTouch->getLocation();
    
    if (!this->isVisible())
        return false;
    
    if (!m_isInputState)
    {
        if (isInTextField(pTouch))
            return true;
        
        return false;
    }
    
	return true;
}

void LyfTextFieldTTF::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    
	CCLog("CNewEditBox::ccTouchEnded");
	CCPoint endPos = pTouch->getLocation();
    
	// 判断是否为点击事件
	if (::abs(endPos.x - m_beginPos.x) > DELTA
        || ::abs(endPos.y - m_beginPos.y) > DELTA)
    {
		// 不是点击事件
		m_beginPos.x = m_beginPos.y = -1;
        
		return;
	}
    
    //	CCLOG("width: %f, height: %f.", getContentSize().width, getContentSize().height);
    
	//判断是打开输入法还是关闭输入法
	isInTextField(pTouch) ? attachWithIME() : detachWithIME();
}

bool LyfTextFieldTTF::attachWithIME() {
    
	if (CCTextFieldTTF::attachWithIME())
    {
		m_pCursorSprite->setVisible(true);
        
		if (m_pInputText && m_pInputText->length() <= 0)
        {
			CCLabelTTF::setString("");
            m_pCursorSprite->setPosition(CCPoint(-getSize().width*getAnchorPoint().x, getSize().height*(0.5-getAnchorPoint().y)));
            this->setContentSize(CCSizeZero);
        }
        m_isInputState=true;
		return true;
	}
	return false;
}

bool LyfTextFieldTTF::detachWithIME() {
    
	if (CCTextFieldTTF::detachWithIME())
    {
		m_pCursorSprite->setVisible(false);
        
		if (m_pInputText && m_pInputText->length() <= 0)
        {
			CCLabelTTF::setString(m_pPlaceHolder->c_str());
            CCRect rect=CCRectZero;
            rect.origin.y=(this->getContentSize().height-this->getSize().height)/2;
            rect.size=CCSize(this->getTextureRect().size.width, this->getSize().height);
            this->setTextureRect(rect);
            this->setContentSize(this->getSize());
        }
        m_isInputState=false;
		return true;
	}
	return false;
}

void LyfTextFieldTTF::deleteBackward() {
    
	int nStrLen = m_pInputText->length();
    
	if (!nStrLen)
    {
		// there is no string
		return;
	}
    
	// get the delete byte number
	int nDeleteLen = 1; // default, erase 1 byte
    
	while (0x80 == (0xC0 & m_pInputText->at(nStrLen - nDeleteLen)))
    {
		++nDeleteLen;
	}
    
	if (m_pDelegate
        && m_pDelegate->onTextFieldDeleteBackward(this,
                                                  m_pInputText->c_str() + nStrLen - nDeleteLen, nDeleteLen))
    {
		// delegate doesn't wan't to delete backwards
		return;
	}
    
	// set new input text
	std::string sText(m_pInputText->c_str(), nStrLen - nDeleteLen);
	setString(sText.c_str());
}

void LyfTextFieldTTF::setString(const char *text) {
    
    
    
	static char bulletString[] = { (char) 0xe2, (char) 0x80, (char) 0xa2,
        (char) 0x00 };
	std::string displayText;
	int length;
    
	CC_SAFE_DELETE(m_pInputText);
    
	if (text) {
		m_pInputText = new std::string(text);
		displayText = *m_pInputText;
		if (m_bSecureTextEntry)
        {
			displayText = "";
			length = m_pInputText->length();
			while (length)
            {
				displayText.append(bulletString);
				--length;
			}
		}
	}
    else
    {
		m_pInputText = new std::string;
	}
    
	// if there is no input text, display placeholder instead
    
	if (!m_pInputText->length())
    {
		if (m_pCursorSprite && m_pCursorSprite->isVisible())
        {
			CCLabelTTF::setString("");
        }
		else
        {
			CCLabelTTF::setString(m_pPlaceHolder->c_str());
        }
	}
    else
    {
        if (m_pInputText->length()<m_nMaxLength)
        {
            CCLabelTTF::setString(displayText.c_str());
        }
        else
        {
            displayText.assign(*m_pInputText, m_pInputText->length()-m_nMaxLength, m_nMaxLength);
            CCLabelTTF::setString(displayText.c_str());
		}
	}
	m_nCharCount = _calcCharCount(m_pInputText->c_str());
    
    float width=this->getSize().width;
    float height=this->getSize().height;
    float x=this->getContentSize().width;
    float y=height/2;
    
    if (x>width)
    {
        x=width;
    }
    
    if (m_pCursorSprite)
    {
		m_pCursorSprite->setPosition(CCPoint(x, y));
	}
    
    if (x==width)
    {
        x=this->getContentSize().width-x;
    }
    else
    {
        x=0;
        width=this->getContentSize().width;
    }
    
    y=(this->getContentSize().height-height)/2;
    
    CCRect rect=CCRect(x, y, width, height);
    
    this->setTextureRect(rect);
    this->setContentSize(this->getSize());
    
    if (strlen(text) > m_nMaxLength)
    {
        this->deleteBackward();
    }
}



void LyfTextFieldTTF::registerWithTouchDispatcher(){
    
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->addTargetedDelegate(this, -1000000, m_nbSwallowsTouches);
    m_IsRehisterTouchDispatcher=true;
}

void LyfTextFieldTTF::unRegisterWithTouchDispatcher(){
    
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->removeDelegate(this);
    m_IsRehisterTouchDispatcher=false;
}

void LyfTextFieldTTF::setTouchDispatcher(bool bSwallowsTouches){
    
    m_nbSwallowsTouches=bSwallowsTouches;
    
    if (m_IsRehisterTouchDispatcher)
    {
        CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        touchDispatcher->removeDelegate(this);
        touchDispatcher->addTargetedDelegate(this, -1000000, m_nbSwallowsTouches);
    }
}

NS_CC_END
