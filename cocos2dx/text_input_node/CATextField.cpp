//
//  CATextField.cpp
//  project
//
//  Created by lh on 14-5-15.
//
//

#include "CATextField.h"
#include "CCDirector.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "actions/CCActionInterval.h"
#include "CCEGLView.h"

NS_CC_BEGIN

static int _calcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}
CATextField::CATextField()
:status(false)
,m_pDelegate(0)
,m_pMark(NULL)
,_oldPos(0)
,frontlength(0)
,backlength(0)
,spaceHolderIsOn(true)
,m_cTextColor(ccc4(0, 0, 0, 255))
,m_cSpaceHolderColor(ccc4(192, 192, 192, 255))
,m_fFontSize(18)
{
   
}

CATextField::~CATextField()
{
    
}

void CATextField::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void CATextField::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

CATextField* CATextField::create(CCRect frame)
{
    CATextField *text = new CATextField();
    if (text && text->initWithTextFieldFrame(frame))
    {
        //text->initWithTextFrame(frame);
        text->autorelease();
        return text;
    }
    CC_SAFE_DELETE(text);
    return NULL;
}
void CATextField::initMarkSprite()
{
    int pixels[1][1];
    pixels[0][0] = 0;

    CAImage *image = new CAImage();
    image->initWithData(pixels, kCAImagePixelFormat_RGB888, 1, 1, CCSizeMake(m_fFontSize / 10.0f, m_fFontSize));
    
    m_pMark = CAImageView::createWithImage(image);
    m_pMark->setVisible(false);
    CCSize contentsize =this->getBounds().size;
    m_pMark->setFrame(CCRect(0, 0, image->getContentSize().width, m_fFontSize));
    this->addSubview(m_pMark);
    
    m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.5f), CCFadeIn::create(0.5f), NULL));
    m_pMark->runAction(m_pCursorAction);
    
}
bool CATextField::initWithTextFieldFrame(cocos2d::CCRect frame)
{
    if (!CAControl::initWithFrame(frame)) {
        return false;
    }
    this->setColor(ccYELLOW);
    this->setOpacity(255);
    
    m_pText = CALabel::create(this->getBounds());
    m_pText->setText("");
    m_pText->setVerticalTextAlignmet(kCCVerticalTextAlignmentCenter);
    m_pText->setNumberOfLine(1);
    m_pText->setTextcolor(m_cTextColor);
    
    this->addSubview(m_pText);
    
    this->initMarkSprite();
    return true;
}

void CATextField::setFontSize(float var)
{
    m_fFontSize = var;
    
    m_pText->setfontSize(var);
    
    m_pMark->setFrame(CCRect(0, 0, var / 10.0f, var));
}

float CATextField::getFontSize()
{
    return m_fFontSize;
}

void CATextField::setPlaceHolder(std::string var)
{
    m_sPlaceHolder = var;
    m_pText->setText(var);
}
std::string CATextField::getPlaceHolder()
{
    return m_sPlaceHolder;
}
void CATextField::setSpaceHolderColor(ccColor4B var)
{
    m_cSpaceHolderColor = var;
    m_pText->setTextcolor(var);
    
}

void CATextField::setTextColor(ccColor4B var)
{
    m_pText->setTextcolor(var);
    m_cTextColor = var;
}
void CATextField::setTextAlignment(CCTextAlignment var)
{
    m_pText->setTextAlignment(var);
    m_aTextAlignment = var;
}

ccColor4B CATextField::getTextColor()
{
    
    return m_cTextColor;
}
ccColor4B CATextField::getSpaceHolderColor()
{
    return m_cSpaceHolderColor;
}
CCTextAlignment CATextField::getTextAlignment()
{
    return m_aTextAlignment;
}


bool CATextField::attachWithIME()
{
    bool bRet = CCIMEDelegate::attachWithIME();
    if (bRet)
    {
        // open keyboard
        CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
        if (pGlView)
        {
            pGlView->setIMEKeyboardState(true);
        }
    }
    return bRet;
}
bool CATextField::detachWithIME()
{
    bool bRet = CCIMEDelegate::detachWithIME();
    if (bRet)
    {
        // close keyboard
        CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
        if (pGlView)
        {
            pGlView->setIMEKeyboardState(false);
        }
    }
    return bRet;
}

bool CATextField::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
        if (attachWithIME())
        {
            m_pMark->setCenterOrigin(CCPoint(m_pText->getLabelSize().width, this->getBounds().size.height/2));
            m_pMark->setVisible(true);
        }

        return true;
    }
    else
    {
        if (detachWithIME())
        {
            if (!strcmp(m_pText->getText().c_str(), ""))
            {
                m_pText->setTextcolor(m_cSpaceHolderColor);
                m_pText->setText(m_sPlaceHolder);
                spaceHolderIsOn=true;
            }
            m_pMark->setVisible(false);
            return false;
        }
        return false;
    }
    
    return false;
}
bool CATextField::canAttachWithIME()
{
    return (m_pDelegate) ? (! m_pDelegate->onTextFieldAttachWithIME(this)) : true;
}

bool CATextField::canDetachWithIME()
{
    return (m_pDelegate) ? (! m_pDelegate->onTextFieldDetachWithIME(this)) : true;
}

void CATextField::insertText(const char * text, int len)
{
    if (spaceHolderIsOn)
    {
        m_pText->setText("");
        m_pText->setTextcolor(m_cTextColor);
        spaceHolderIsOn=false;
        
    }
    std::string sInsert(text, len);
    
    // insert \n means input end
    int nPos = sInsert.find('\n');
    if ((int)sInsert.npos != nPos)
    {
        len = nPos;
        sInsert.erase(nPos);
    }
    
    if (len > 0)
    {
        if (m_pDelegate && m_pDelegate->onTextFieldInsertText(this, sInsert.c_str(), len))
        {
            
            
            // delegate doesn't want to insert text
            return;
        }
        m_nCharCount += _calcCharCount(sInsert.c_str());
        std::string sText(m_pText->getText());
        std::string oldstr = sText;
        sText.append(sInsert);
        
        float length = 0;
        
        m_pText->setText(sText.c_str());
        
        CCRect rect = CCRectZero;
        rect.size = this->getBounds().size;
        rect.size.width = MIN(this->getBounds().size.width, m_pText->getLabelSize().width);
        
        if (m_pText->getLabelSize().width < this->getBounds().size.width)
        {
            m_pText->setFrame(rect);
            m_pMark->setCenterOrigin(CCPoint(m_pText->getLabelSize().width, this->getBounds().size.height/2));
        }
        else
        {
            m_pText->setText(oldstr);
            m_pText->setFrame(rect);
            m_pMark->setCenterOrigin(CCPoint(m_pText->getLabelSize().width, this->getBounds().size.height/2));
        }
        
        if (_oldPos != 0)
        {
            length = m_pText->getLabelSize().width - _oldPos;
        }
        else
        {
            length = m_pText->getLabelSize().width;
        }
        
    }
    
    if ((int)sInsert.npos == nPos) {
        return;
    }
    
    if (m_pDelegate && m_pDelegate->onTextFieldInsertText(this, "\n", 1))
    {
        return;
    }
    
    // if delegate hasn't processed, detach from IME by default
    detachWithIME();
}

void CATextField::deleteBackward()
{
    if(spaceHolderIsOn)
    {
        return;
    }
    int nStrLen = m_pText->getText().length();
    if (! nStrLen)
    {
        // there is no string
        m_pText->setText("");
        m_pText->setFrame(CCRect(0, 0, m_pText->getLabelSize().width, this->getBounds().size.height));
        return;
    }
    
    // get the delete byte number
    int nDeleteLen = 1;    // default, erase 1 byte
    
    while(0x80 == (0xC0 & m_pText->getText().at(nStrLen - nDeleteLen)))
    {
        ++nDeleteLen;
    }
    
    if (m_pDelegate && m_pDelegate->onTextFieldDeleteBackward(this, m_pText->getText().c_str() + nStrLen - nDeleteLen, nDeleteLen))
    {
        // delegate doesn't wan't to delete backwards
        return;
    }
    
    // if all text deleted, show placeholder string
    if (nStrLen <= nDeleteLen)
    {
        m_pText->setTextcolor(m_cSpaceHolderColor);
        m_pText->setText(m_sPlaceHolder);
        spaceHolderIsOn=true;
        m_nCharCount = 0;
        m_pMark->setCenterOrigin(CCPoint(0, this->getBounds().size.height/2));
        return;
    }
    
    // set new input text
    std::string sText(m_pText->getText().c_str(), nStrLen - nDeleteLen);
    m_pText->setText(sText.c_str());
    m_pText->setFrame(CCRect(0, 0, m_pText->getLabelSize().width, this->getBounds().size.height));
    m_pMark->setCenterOrigin(CCPoint(m_pText->getLabelSize().width, this->getBounds().size.height/2));
}

const char * CATextField::getContentText()
{
    return m_sPlaceHolder.c_str();
}

void CATextField::draw()
{
    if (m_pDelegate && m_pDelegate->onDraw(this))
    {
        return;
    }
    if (m_sPlaceHolder.length())
    {
        CAControl::draw();
        return;
    }
    
    // draw placeholder
    ccColor3B color = getColor();
    setColor(ccc3(m_cSpaceHolderColor.r, m_cSpaceHolderColor.g, m_cSpaceHolderColor.b));
    CAControl::draw();
    setColor(color);
}

NS_CC_END