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

NS_CC_BEGIN

CATextField::CATextField()
:m_pDelegate(0)
,m_pMark(NULL)
,spaceHolderIsOn(true)
,m_cTextColor(ccc4(0, 0, 0, 255))
,m_cSpaceHolderColor(ccc4(193, 193, 193, 255))
,m_fFontSize(24)
,willBg(NULL)
,isEditing(false)
,m_nInputType(KEY_BOARD_INPUT_NORMAL)
,m_keyboardType(KEY_BOARD_TYPE_NORMAL)
,m_fString_right_length(0)
,m_fString_left_offX(0)
,m_fString_left_length(0)
,m_sText("")
,labelWidth(0)
,labelOrginX(0)
,m_cCursorColor(CAColor_black)
,m_bUpdateImage(false)
,m_pBackgroundView(NULL)
{
   
}

CATextField::~CATextField()
{
    
}

void CATextField::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    CC_RETURN_IF(m_sPlaceHolder.empty());
	if (m_bUpdateImage)
	{
		m_bUpdateImage = false;
		this->updateImage();
	}
    
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
    if (!CAControl::initWithFrame(frame,CAColor_red)) {
        return false;
    }
    width = CAApplication::getApplication()->getWinSize().width;
    TextAttribute t;
    t.charsize =0;
    t.charlength =0;
    this->setBackGroundImage(CAImage::create("source_material/textField_bg.png"));
    m_vByteLengthArr.push_back(t);
    labelOrginX =0.015625*width;
    labelWidth =this->getBounds().size.width-0.03125*width;

    willBg = CAView::createWithFrame(CCRectZero,ccc4(65, 105, 255, 200));
    this->insertSubview(willBg,-1);
    this->initMarkSprite();
    return true;
}

bool CATextField::initWithCenter(const CCRect& rect)
{
    if (!CAControl::initWithCenter(rect)) {
        return false;
    }
    width = CAApplication::getApplication()->getWinSize().width;
    TextAttribute t;
    t.charsize =0;
    t.charlength =0;
    this->setBackGroundImage(CAImage::create("source_material/textField_bg.png"));
    m_vByteLengthArr.push_back(t);
    labelOrginX =0.015625*width;
    labelWidth =this->getBounds().size.width-0.03125*width;
    willBg = CAView::createWithFrame(CCRectZero,ccc4(65, 105, 255, 200));
    this->addSubview(willBg);
    this->initMarkSprite();
    return true;
}

void CATextField::initMarkSprite()
{
 
    m_pMark = CAView::create();
    m_pMark->setColor(m_cCursorColor);
    m_pMark->setVisible(false);
    CCSize contentsize =this->getBounds().size;
    m_pMark->setFrame(CCRect(labelOrginX, 0, 2, m_fFontSize));
    this->addSubview(m_pMark);
    
    m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.5f), CCFadeIn::create(0.5f), NULL));
    m_pMark->runAction(m_pCursorAction);
    
}

void CATextField::setFontSize(float var)
{
    m_fFontSize = var;
    
    m_pMark->setFrame(CCRect(labelOrginX, 0, var / 10.0f, var));
    
	m_bUpdateImage = true;
}

float CATextField::getFontSize()
{
    return m_fFontSize;
}

void CATextField::setText(std::string var)
{
    m_sText=var;
}

std::string CATextField::getText()
{
    if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
    {
        return m_sPassWord;
    }
    return m_sText;
}

void CATextField::setPlaceHolder(std::string var)
{
    m_sPlaceHolder = var;
    m_sText = var;
	m_bUpdateImage = true;
}

std::string CATextField::getPlaceHolder()
{
    return m_sPlaceHolder;
}

void CATextField::setSpaceHolderColor(CAColor4B var)
{
    m_cSpaceHolderColor = var;
	m_bUpdateImage = true;
}

CAColor4B CATextField::getSpaceHolderColor()
{
    return m_cSpaceHolderColor;
}

void CATextField::setTextColor(CAColor4B var)
{
    m_cTextColor = var;
	m_bUpdateImage = true;
}

CAColor4B CATextField::getTextColor()
{
    return m_cTextColor;
}

void CATextField::setTextAlignment(CATextAlignment var)
{
    m_aTextAlignment = var;
	m_bUpdateImage = true;
}

CATextAlignment CATextField::getTextAlignment()
{
    return m_aTextAlignment;
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
    if (isEditing)
    {
        return false;
    }
    CCPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
		becomeFirstResponder();
		if (isFirstResponder())
        {
            
            m_pMark->setVisible(true);
            attachWithIME();

            if (m_nInputType ==KEY_BOARD_INPUT_PASSWORD)
            {
                m_pMark->setCenterOrigin(CCPoint(labelOrginX+m_rLabelRect.size.width, this->getBounds().size.height/2));
                return true;
            }
            m_fString_left_length = 0;
            int byteCount = 0;
            for (std::vector<TextAttribute>::iterator itr = m_vByteLengthArr.begin(); itr!=m_vByteLengthArr.end(); itr++)
            {
                TextAttribute t =*(itr);
                m_fString_left_length+=t.charlength;
                byteCount += t.charsize;
                if (m_fString_left_length>point.x-m_fString_left_offX)
                {
                    m_sLeft_string = m_sText.substr(0,byteCount);
                    m_sRight_string = m_sText.substr(byteCount,m_sText.length());
                    m_fString_right_length = m_rLabelRect.size.width-m_fString_left_length;
                    break;
                }else if(itr == m_vByteLengthArr.end()-1&&!spaceHolderIsOn)
                {
                    m_sLeft_string = m_sText.substr(0,byteCount);
                    m_sRight_string = m_sText.substr(byteCount,m_sText.length());
                    m_fString_right_length = m_rLabelRect.size.width-m_fString_left_length;
                }
                
             }
             m_pMark->setCenterOrigin(CCPoint(getCursorX()+labelOrginX, this->getBounds().size.height/2));

            
        }
        return true;
    }
    else
    {
        if (resignFirstResponder())
        {
            if (!strcmp(m_sText.c_str(), ""))
            {
                m_sText="";
                spaceHolderIsOn=true;
                this->updateImage();
            }
            m_pMark->setVisible(false);
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
void CATextField::analyzeString(std::string str,int len)
{
    
    for (int i=0; i<len; i++)
    {
        if (str[i]>0&& str[i]<127)
        {
//            if (str[i]!=32)
            {
               insertText(str.substr(i,1).c_str(), 1);
            }
//            else
//            {
//                continue;
//            }
        }
        else
        {
            std::string ss= str.substr(i,3);
            insertText(ss.c_str(), 3);
            i+=2;
        }
        
    }
    
}
void CATextField::insertText(const char * text, int len)
{
    if (len>3)
    {
        analyzeString(text,len);
        return;
    }
    if (m_nInputType == KEY_BOARD_INPUT_PASSWORD)
    {
        if (len>=2)
            return;
        if (spaceHolderIsOn)
        {
            spaceHolderIsOn = false;
            m_sText="";
        }
        m_sText = m_sPassWord;                                                                                                                                                                                                                                                                             
        m_sText.append(text);
        
        
        
        std::string password="";
        for (int i=0; i<m_sText.length(); i++)
        {
            password.append("*");
        }
        m_sPassWord = m_sText;
        m_sText = password;
        this->updateImage();

        CCRect r = m_rLabelRect;
        r.origin.x = 0;
        r.origin.x = MAX(labelWidth-m_rLabelRect.size.width, 0);
        r.origin.x = MIN(0, r.origin.x);
        r.size.width = MIN(m_rLabelRect.size.width, labelWidth);
        this->setImageRect(r);
        float offsetX = MIN(labelWidth, m_rLabelRect.size.width);
        m_pMark->setCenterOrigin(CCPoint(offsetX+labelOrginX,this->getBounds().size.height/2));
        
        return;
    }
    
    if (!strcmp(text, "\n"))
    {
        return;
    }

    std::string inputstr;
    if (spaceHolderIsOn)
    {
        spaceHolderIsOn = false;
        m_sText = "";
    }
    
    if(isEditing)
    {
        isEditing =false;
    }
    willBg->setVisible(false);
    
    if (len > 0)
    {
        if (m_pDelegate && m_pDelegate->onTextFieldInsertText(this,text, len))
        {
            // delegate doesn't want to insert text
            return;
        }
        m_sLeft_string.append(text);
        
        float left_width = getStringLength(m_sLeft_string);
        
        TextAttribute t;
        t.charlength = left_width - m_fString_left_length;
        t.charsize = strlen(text);
        m_vByteLengthArr.insert(m_vByteLengthArr.begin()+getStringCharCount(m_sLeft_string), t);
        
        char str[512]="";
        sprintf(str, "%s%s",m_sLeft_string.c_str(),m_sRight_string.c_str());
        m_sText = str;
        
        this->updateImage();
        
		m_fString_left_length = m_rLabelRect.size.width;
			//left_width;
        m_fString_right_length = m_rLabelRect.size.width - m_fString_left_length;
        
        if (m_fString_left_length + m_fString_left_offX > labelWidth)
        {
            m_fString_left_offX = labelWidth - m_fString_left_length;
        }
        

        CCRect rect = CCRectZero;
        rect.size = this->getBounds().size;
        CCRect r = m_rLabelRect;
        r.origin.x = -m_fString_left_offX;
        r.size.width = getStringViewLength();
        this->setImageRect(r);
    }
    m_pMark->setCenterOrigin(CCPoint(getCursorX()+labelOrginX,this->getBounds().size.height/2));
    
}

void CATextField::willInsertText(const char *text, int len)
{
    
    
    
    if (m_nInputType ==KEY_BOARD_INPUT_PASSWORD)
        return;
    
    if (len>0)
    {
       
        
        if (!strcmp(temporaryString.c_str(), text))
        {
            return;
        }
         temporaryString =text;
        isEditing = true;
        spaceHolderIsOn = false;
        char sText[512]="";
        
       // m_sLeft_string.append(temporaryString);
        
        sprintf(sText, "%s%s%s",m_sLeft_string.c_str(),text,m_sRight_string.c_str());
        m_sText = sText;
        
        this->updateImage();
        float left_width = getStringLength(m_sLeft_string)+getStringLength(temporaryString);
        m_fString_right_length = m_rLabelRect.size.width - left_width;
        
        float willOffsetX = 0;
        
        if (left_width - m_fString_left_offX > labelWidth)
        {
            willOffsetX = labelWidth - left_width-m_fString_left_offX;
        }
        
        CCRect rect = CCRectZero;
        rect.size = this->getBounds().size;
        CCRect r = m_rLabelRect;
        r.origin.x = -m_fString_left_offX-willOffsetX;
        r.size.width = m_rLabelRect.size.width;
        //-m_fString_left_length-m_fString_right_length;
        r.size.width = MIN(r.size.width, labelWidth);

        this->setImageRect(r);

        m_pMark->setCenterOrigin(CCPoint(left_width+willOffsetX+labelOrginX,this->getBounds().size.height/2));

        return;
    }
    else if(len==0&&isEditing)
    {
        willBg->setFrame(CCRectMake(labelOrginX, m_rLabelRect.size.height/2,0, m_rLabelRect.size.height));
        willBg->setVisible(false);
        temporaryString="";
        isEditing =false;
        if (strcmp(m_sLeft_string.c_str(), ""))
        {
            this->updateImage();
        }
        else
        {
            m_sText = "";
            spaceHolderIsOn = true;
            this->updateImage();
        }
        m_pMark->setCenterOrigin(CCPoint(labelOrginX+getCursorX() , this->getBounds().size.height/2));
        return;
    }
}
void CATextField::deleteBackward()
{
    if (m_nInputType==KEY_BOARD_INPUT_PASSWORD)
    {
        m_sText="";
        spaceHolderIsOn=true;
        this->updateImage();
        m_pMark->setCenterOrigin(CCPoint(labelOrginX+getCursorX(), this->getBounds().size.height/2));
        
        return;
    }
    if(spaceHolderIsOn)
    {
        
        return;
    }
    
    int nStrLen =m_sLeft_string.length();

    int nDeleteLen = 1;    // default, erase 1 byte
    
    if (nStrLen == 0)
    {
        return;
    }
    
    while(0x80 == (0xC0 & m_sLeft_string.at(m_sLeft_string.length() - nDeleteLen)))
    {
        ++nDeleteLen;
    }
    if (m_pDelegate && m_pDelegate->onTextFieldDeleteBackward(this, m_sLeft_string.c_str() + nStrLen - nDeleteLen, nDeleteLen))
    {
        // delegate doesn't wan't to delete backwards
        return;
    }
    // if all text deleted, show placeholder string
    if (m_sText.length() <= nDeleteLen)
    {
        m_sLeft_string="";
        spaceHolderIsOn=true;
        m_vByteLengthArr.clear();
        TextAttribute tt;
        tt.charsize =0;
        tt.charlength =0;
        m_vByteLengthArr.push_back(tt);
        m_pMark->setCenterOrigin(CCPoint(labelOrginX, this->getBounds().size.height/2));
        this->updateImage();
        return;
    }

    
    int length = MAX(0, nStrLen - nDeleteLen);
    
    std::string sText(m_sLeft_string.c_str(), length);
    char str[512]="";
    sprintf(str, "%s%s",sText.c_str(),m_sRight_string.c_str());
    m_sText = str;
    this->updateImage();
    
    m_vByteLengthArr.erase(m_vByteLengthArr.begin()+getStringCharCount(m_sLeft_string));
    m_sLeft_string = sText.c_str();

	m_fString_left_length = m_rLabelRect.size.width;
		//left_width;
    m_fString_right_length = m_rLabelRect.size.width - m_fString_left_length;
    
    if (m_fString_right_length + getCursorX() < labelWidth)
    {
        m_fString_left_offX = MIN(0, labelWidth - m_rLabelRect.size.width);
    }
    
    CCRect rect = CCRectZero;
    rect.size = this->getBounds().size;
    CCRect r = m_rLabelRect;
    r.origin.x = -m_fString_left_offX;
    r.size.width = getStringViewLength();
    this->setImageRect(r);
    m_pMark->setCenterOrigin(CCPoint( getCursorX()+labelOrginX, this->getBounds().size.height/2));
    return;
}

const char* CATextField::getContentText()
{
    return m_sText.c_str();
}

void CATextField::getKeyBoardHeight(int height)
{
    if( m_pDelegate && m_pDelegate->getKeyBoardHeight(height) )
    {
        return;
    }
    
}

void CATextField::setBackGroundImage(CAImage *image)
{
//    if (m_pBackgroundView == NULL)
//    {
//        m_pBackgroundView = CAScale9ImageView::create();
//        m_pBackgroundView->setFrame(this->getBounds());
//        this->insertSubview(m_pBackgroundView, -1);
//    }
//    m_pBackgroundView->setImage(image);
}

CAImage *CATextField::getBackGroundImage()
{
    if (m_pBackgroundView == NULL)
    {
        return NULL;
    }
    return m_pBackgroundView->getImage();
}

void CATextField::setContentSize(const CCSize& var)
{
    CAControl::setContentSize(var);
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getBounds());
    }
}

void CATextField::updateImage()
{

    float fontHeight = CAImage::getFontHeight("", m_fFontSize);
    CCSize size = CCSizeZero;
    size = CCSizeMake(0, fontHeight);
    std::string text = "";
    
    if (spaceHolderIsOn)
    {
        text = m_sPlaceHolder;
        this->setColor(m_cSpaceHolderColor);
    }else
    {
        text = m_sText;
        this->setColor(m_cTextColor);
    }
    
	CAImage* image = CAImage::createWithString(text.c_str(),
                                               "",
                                               m_fFontSize * CC_CONTENT_SCALE_FACTOR(),
                                               size,
                                               m_aTextAlignment,
                                               CAVerticalTextAlignmentCenter);
	CCRect rect = CCRectZero;
	float imageWidth = 0;
	if (image != NULL)
	{
		rect.size.height = image->getContentSize().height;
		m_rLabelRect.size = image->getContentSize();
		imageWidth = image->getContentSize().width;
	}

    if (spaceHolderIsOn)
    {
        m_rLabelRect = CCRectZero;
    }
    
	float width = MIN(labelWidth, imageWidth);
    rect.size.width = width;
    this->setImage(image);
    
    pTextHeight = (this->getBounds().size.height-rect.size.height)/2;
    rect.origin.x = m_fString_left_offX;
    this->setImageRect(rect);
    return ;
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
    x1=labelOrginX;
    y1=0;
    y1=m_obContentSize.height - m_obRect.size.height - y1;
    y1=y1-pTextHeight;
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
    if (m_pMark)
    {
        m_pMark->setColor(m_cCursorColor);
    }
    
}

CAColor4B CATextField::getCursorColor()
{
    return m_cCursorColor;
}

float CATextField::getCursorX()
{
    return m_fString_left_length + m_fString_left_offX;
}
float CATextField::getStringViewLength()
{
    return MIN(labelWidth, m_fString_right_length + getCursorX());
}
unsigned int CATextField::getStringCharCount(const std::string &var)
{
    
    unsigned int count = 0;
    
    std::string str = var;

    string::iterator itr=str.begin();
    
    for (itr=str.begin(); itr!=str.end(); itr++)
    {
        if (*itr>0 && *itr<127)
        {
            count++;
        }
        else
        {
            itr +=2;
            count ++;
        }
    }
    return count;

}
std::vector<std::string> CATextField::getStringVector(const std::string &var)
{
    std::string str = var;
    
    std::vector<std::string> stringVector;
    
    int i = 0;
    
    while (i<str.length())
    {
        if (str[i]>0&& str[i]<127)
        {
            stringVector.push_back(str.substr(i,1));
            i++;
        }
        else
        {
            stringVector.push_back(str.substr(i,3));
            i+=3;
        }
    }
    return stringVector;
}
float CATextField::getStringLength(const std::string &var)
{
    float length = 0;
    CAImage *image = CAImage::createWithString(var.c_str(),
                                               "",
                                               m_fFontSize,
                                               CCSizeZero,
                                               CATextAlignmentLeft,
                                               CAVerticalTextAlignmentCenter);
    if(image != NULL)
    {
        length = image->getContentSize().width;
    }
    
    return length;
}
void CATextField::visit()
{
	CAView::visit();
	if (m_bUpdateImage)
	{
		m_bUpdateImage = false;
		this->updateImage();
	}
}
NS_CC_END