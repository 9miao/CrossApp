//
//  CATextField.cpp
//  project
//
//  Created by lh on 14-5-15.
//
//

#include "CATextField.h"
#include "CCDirector.h"
#include "actions/CCActionInterval.h"
#include "CCEGLView.h"
#include <locale>
#include <algorithm>

NS_CC_BEGIN

CATextField::CATextField()
:m_pDelegate(0)
,m_pMark(NULL)
,spaceHolderIsOn(true)
,m_cTextColor(ccc4(0, 0, 0, 255))
,m_cSpaceHolderColor(ccc4(192, 192, 192, 255))
,m_fFontSize(18)
,willBg(NULL)
,isEditing(false)
,frontWidth(0)
,frontStr("")
,byteCount(0)
,bytePos(0)
,m_nInputType(KEY_BOARD_INPUT_NORMAL)
,m_keyboardType(KEY_BOARD_TYPE_NORMAL)
{
   
}

CATextField::~CATextField()
{
    
}

void CATextField::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
}

void CATextField::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}
bool CATextField::resignFirstResponder()
{
    bool result =CAView::resignFirstResponder();
    if (result) {
        attachWithIME();
    }
    return result;
}
bool CATextField::becomeFirstResponder()
{
    bool result = CAView::becomeFirstResponder();
    if (result) {
        detachWithIME();
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
    if (!CAControl::initWithFrame(frame)) {
        return false;
    }
    lengthArr.push_back(0.0f);
    byteArr.push_back(0);
    this->setColor(ccYELLOW);
    this->setOpacity(255);
    m_pText = CALabel::createWithFrame(this->getBounds());
    m_pText->setText("");
    m_sText="";
    m_pText->setVerticalTextAlignmet(kCCVerticalTextAlignmentCenter);
    m_pText->setNumberOfLine(1);
    m_pText->setTextcolor(m_cTextColor);
    
    this->addSubview(m_pText);
    willBg = CAView::createWithFrame(CCRectZero,ccc4(65, 105, 255, 200));
    m_pText->addSubview(willBg);
    this->initMarkSprite();
    return true;
}
bool CATextField::initWithCenter(const CCRect& rect)
{
    if (!CAControl::initWithCenter(rect)) {
        return false;
    }
    lengthArr.push_back(0.0f);
    byteArr.push_back(0);
    this->setColor(ccYELLOW);
    this->setOpacity(255);
    m_pText = CALabel::createWithFrame(this->getBounds());
    m_pText->setText("");
    m_sText="";
    m_pText->setVerticalTextAlignmet(kCCVerticalTextAlignmentCenter);
    m_pText->setNumberOfLine(1);
    m_pText->setTextcolor(m_cTextColor);
    
    this->addSubview(m_pText);
    willBg = CAView::createWithFrame(CCRectZero,ccc4(65, 105, 255, 200));
    m_pText->addSubview(willBg);
    this->initMarkSprite();
    return true;
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
void CATextField::setText(std::string var)
{
    m_sText=var;
}
std::string CATextField::getText()
{
    return m_sText;
}
void CATextField::setPlaceHolder(std::string var)
{
    m_sPlaceHolder = var;
    m_pText->setTextcolor(getSpaceHolderColor());
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
ccColor4B CATextField::getSpaceHolderColor()
{
    return m_cSpaceHolderColor;
}
void CATextField::setTextColor(ccColor4B var)
{
    m_pText->setTextcolor(var);
    m_cTextColor = var;
}
ccColor4B CATextField::getTextColor()
{
    
    return m_cTextColor;
}
void CATextField::setTextAlignment(CCTextAlignment var)
{
    m_pText->setTextAlignment(var);
    m_aTextAlignment = var;
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
            
            if (getKeyboardType() ==KEY_BOARD_TYPE_NORMAL) {
                pGlView->setIMEKeyboardDefault();
            }else if (getKeyboardType() ==KEY_BOARD_TYPE_NUMBER) {
                pGlView->setIMEKeyboardNumber();
            }else if(getKeyboardType() ==KEY_BOARD_TYPE_ALPHABET)
            {
                pGlView->setIMEKeyboardAlphabet();
            }
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
void CATextField::setInputType(eKeyBoardInputType type)
{
    m_nInputType=type;
}
eKeyBoardInputType CATextField::getInputType()
{
    return m_nInputType;
}
bool CATextField::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
    if (isEditing) {
        return false;
    }
    
    CCPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
        resignFirstResponder();
        if (isFirstResponder())
        {
            
            m_pMark->setVisible(true);
            float markWidth = m_pText->getLabelSize().width;
            if (m_pText->getLabelSize().width>this->getBounds().size.width)
            {
                markWidth = this->getBounds().size.width;
            }else if(spaceHolderIsOn)
            {
                markWidth =0;
            }
            m_pMark->setCenterOrigin(CCPoint(markWidth, this->getBounds().size.height/2));
            
            
            if (m_nInputType ==KEY_BOARD_INPUT_PASSWORD) {
                
                return true;
            }

            for (int i=0; i<lengthArr.size(); i++)
            {
                
                float length = ((*(lengthArr.begin()+i)));
                
               
                float lengthadd =0;
                if (i==lengthArr.size()-1)
                {
                    lengthadd = (*lengthArr.end()-1);
                }
                else
                {
                    lengthadd = ((*(lengthArr.begin()+i+1)));
                }
                if (point.x>length && point.x<lengthadd)
                {
                    m_pMark->setCenterOrigin(CCPoint(length, this->getBounds().size.height/2));
                    bytePos = i;
                    break;
                }
                else if(i==lengthArr.size()-1 &&lengthArr.size()>1)
                {
                    bytePos = i;
                    m_pMark->setCenterOrigin(CCPoint(m_pText->getLabelSize().width, this->getBounds().size.height/2));
                    break;
                }
            }
            
        }
        return true;
    }
    else
    {
        if (becomeFirstResponder())
        {
            if (!strcmp(m_pText->getText().c_str(), ""))
            {
                m_pText->setTextcolor(m_cSpaceHolderColor);
                m_pText->setText(m_sPlaceHolder);
                m_sText="";
                spaceHolderIsOn=true;
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

void CATextField::insertText(const char * text, int len)
{

    if (m_nInputType == KEY_BOARD_INPUT_PASSWORD) {
        if (len>=2) {
            return;
        }
        m_sText.append(text);
        
        std::string password="";
        for (int i=0; i<m_sText.length(); i++) {
            password.append("*");
        }
        m_pText->setTextcolor(m_cTextColor);
        m_pText->setText(password);
        
        CCRect rect = CCRectZero;
        rect.size = this->getBounds().size;
        rect.size.width = MIN(this->getBounds().size.width, m_pText->getLabelSize().width);
        if (m_pText->getLabelSize().width < this->getBounds().size.width)
        {
            m_pMark->setCenterOrigin(CCPoint(m_pText->getLabelSize().width, this->getBounds().size.height/2));
        }else
        {
            
            //textlength+lengt
            if (m_pText->getLabelSize().width<rect.size.width)
            {
                
                
                m_pMark->setCenterOrigin(CCPoint(m_pText->getLabelSize().width, this->getBounds().size.height/2));
                CCPoint pos = m_pText->getTextImage()->getFrame().origin;
                
                CCRect r =m_pText->getTextImage()->getImageRect();
                
                r.size.width = this->getBounds().size.width;
                
                m_pText->getTextImage()->setImageRect(r);
                m_pText->getTextImage()->setFrameOrigin(pos);
                
            }
            else
            {
                CCPoint pos = m_pText->getTextImage()->getFrame().origin;
                
                CCRect r =m_pText->getTextImage()->getImageRect();
                r.origin.x =(m_pText->getTextImage()->getImage()->getContentSize().width-this->getBounds().size.width-afterWidth);
                
                r.size.width = this->getBounds().size.width;
                
                m_pText->getTextImage()->setImageRect(r);
                m_pText->getTextImage()->setFrameOrigin(pos);
                m_pMark->setCenterOrigin(CCPoint(rect.size.width, this->getBounds().size.height/2));
            }
        }
        m_pText->setFrame(rect);
        
        return;
    }
    
    
    if (!strcmp(text, "\n")) {
        return;
    }
    
    std::string inputstr;

    if (spaceHolderIsOn)
    {
        m_pText->setText("");
        m_sText="";
        m_pText->setTextcolor(m_cTextColor);
        spaceHolderIsOn=false;
        
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
        std::string sText(frontStr);
        std::string oldstr = sText;
        beforeWidth = -1.0f;
        afterWidth= -1.0f;
        char str[512]="";
        if ((*(byteArr.begin()+bytePos))!=frontStr.length())
        {
            std::string beforeStr;
            if (bytePos ==0)
            {
                beforeStr="";
            }
            else
            {
                beforeStr=frontStr.substr(0,(*(byteArr.begin()+bytePos)));
            }
            beforeWidth = (*(lengthArr.begin()+bytePos));
            afterWidth = frontWidth-beforeWidth;
            std::string backStr= frontStr.substr((*(byteArr.begin()+bytePos)),strlen(frontStr.c_str()));
            sprintf(str, "%s%s%s",beforeStr.c_str(),text,backStr.c_str());
        }
        else
        {
            sprintf(str, "%s%s",sText.c_str(),text);
        }
        m_sText = str;
        m_pText->setText(str);
        
        
        int numbers = (m_pText->getLabelSize().width-frontWidth)/m_pText->getfontSize();
        if (numbers<0)
        {
            numbers=0;
        }
        if (numbers*m_pText->getfontSize()>m_pText->getfontSize())
        {
            for (int i =1; i<=numbers; i++)
            {
                byteCount+=3;
                bytePos++;
                lengthArr.push_back(frontWidth+m_pText->getfontSize()*i);
                byteArr.push_back(byteCount);
            }
        }else
        {
           
            byteCount+=strlen(text);
            bytePos++;
            byteArr.push_back(byteCount);
            if (beforeWidth<m_pText->getLabelSize().width && beforeWidth !=-1.0f)
            {
                vector <float>::iterator theIterator = lengthArr.begin()+bytePos;
                lengthArr.insert(theIterator, beforeWidth+m_pText->getLabelSize().width-frontWidth);
                for (vector <float>::iterator i = (lengthArr.begin()+bytePos+1); i !=lengthArr.end(); i++)
                {
                    *i=(*i)+m_pText->getLabelSize().width-frontWidth;
                }
            }
            else
            {
                lengthArr.push_back(m_pText->getLabelSize().width);
            }
        }
        
   

        frontStr = m_pText->getText();
        frontWidth = m_pText->getLabelSize().width;
        
        CCRect rect = CCRectZero;
        rect.size = this->getBounds().size;
        rect.size.width = MIN(this->getBounds().size.width, m_pText->getLabelSize().width);
        
        float textlength=(float)(*(lengthArr.begin()+bytePos));
        float length =(m_pText->getLabelSize().width)-textlength-(*(lengthArr.end()-1)-(*(lengthArr.begin()+bytePos)));
        if (m_pText->getLabelSize().width < this->getBounds().size.width)
        {
            m_pMark->setCenterOrigin(CCPoint(textlength+length, this->getBounds().size.height/2));
        }
        else
        {
            //textlength+lengt
            if (textlength+length<rect.size.width)
            {
                m_pMark->setCenterOrigin(CCPoint(textlength+length, this->getBounds().size.height/2));
                CCPoint pos = m_pText->getTextImage()->getFrame().origin;
                
                CCRect r =m_pText->getTextImage()->getImageRect();

                r.size.width = this->getBounds().size.width;
                
                m_pText->getTextImage()->setImageRect(r);
                m_pText->getTextImage()->setFrameOrigin(pos);

            }
            else
            {
                CCPoint pos = m_pText->getTextImage()->getFrame().origin;
                
                CCRect r =m_pText->getTextImage()->getImageRect();
                r.origin.x =(m_pText->getTextImage()->getImage()->getContentSize().width-this->getBounds().size.width-afterWidth);
                
                r.size.width = this->getBounds().size.width;
                
                m_pText->getTextImage()->setImageRect(r);
                m_pText->getTextImage()->setFrameOrigin(pos);
                m_pMark->setCenterOrigin(CCPoint(rect.size.width, this->getBounds().size.height/2));
            }
        }
        m_pText->setFrame(rect);
        
      
    }
}
void CATextField::willInsertText(const char *text, int len)
{
    
    if (m_nInputType ==KEY_BOARD_INPUT_PASSWORD) {
        return;
    }
    
    if (len>0)
    {
        temporaryString =text;
        isEditing = true;
        char sText[512]="";
        
        if ((*(byteArr.begin()+bytePos))!=strlen(frontStr.c_str()))
        {
            std::string beforeStr;
            if (bytePos ==0)
            {
                beforeStr = "";
            }
            else
            {
                beforeStr=frontStr.substr(0,(*(byteArr.begin()+bytePos)));
            }
            
            std::string backStr= frontStr.substr((*(byteArr.begin()+bytePos)),frontStr.length());
            sprintf(sText, "%s%s%s",beforeStr.c_str(),text,backStr.c_str());
        }
        else
        {
            sprintf(sText, "%s%s",frontStr.c_str(),text);
        }
        
        m_pText->setText(sText);
        
        m_pText->setTextcolor(ccc4(0, 0, 0, 255));
        float bglength=m_pText->getLabelSize().width;
        float textlength =0;
        if (lengthArr.size()>0)
        {
            textlength=(float)((*(lengthArr.begin()+bytePos)));
            bglength =(m_pText->getLabelSize().width)-textlength-(*(lengthArr.end()-1)-(*(lengthArr.begin()+bytePos)));
        }
        CCRect rect = CCRectZero;
        rect.size = this->getBounds().size;
        rect.size.width = MIN(this->getBounds().size.width, m_pText->getLabelSize().width);
        
        float longlength=(float)((*(lengthArr.begin()+bytePos)));

        float shortlength = (m_pText->getLabelSize().width)-textlength-(*(lengthArr.end()-1)-(*(lengthArr.begin()+bytePos)));
        
        if (m_pText->getLabelSize().width < this->getBounds().size.width)
        {
            m_pText->setFrame(rect);
            m_pMark->setCenterOrigin(CCPoint(longlength+shortlength, this->getBounds().size.height/2));
            willBg->setVisible(true);
            willBg->setFrame(CCRectMake(textlength, (this->getBounds().size.height-m_pText->getLabelSize().height)/2,bglength, m_pText->getLabelSize().height));
        }
        else
        {
            CCPoint pos = m_pText->getTextImage()->getFrame().origin;
            CCRect r =m_pText->getTextImage()->getImageRect();
            r.origin.x =(m_pText->getTextImage()->getImage()->getContentSize().width-this->getBounds().size.width);
            r.size.width = this->getBounds().size.width;
            m_pText->getTextImage()->setImageRect(r);
            m_pText->getTextImage()->setFrameOrigin(pos);
            m_pText->setFrame(rect);
            m_pMark->setCenterOrigin(CCPoint(rect.size.width, this->getBounds().size.height/2));//rect.size.width
            
            willBg->setVisible(true);
            
            if (longlength ==0)
            {
                willBg->setFrame(CCRectMake(longlength, (this->getBounds().size.height-m_pText->getLabelSize().height)/2,this->getBounds().size.width, m_pText->getLabelSize().height));
            }
            else
            {
                float orgx=(m_pText->getBounds().size.width- (m_pText->getLabelSize().width-longlength));
                if(bglength>this->getBounds().size.width&&m_pText->getBounds().size.width- (m_pText->getLabelSize().width-longlength)<0)
                {
                    bglength=this->getBounds().size.width;
                    
                    orgx =0;
                    
                }
                willBg->setFrame(CCRectMake(orgx, (this->getBounds().size.height-m_pText->getLabelSize().height)/2,bglength, m_pText->getLabelSize().height));
            }
            
            
        }

        return;
    }
    else if(len==0)
    {
        willBg->setFrame(CCRectMake(0, (this->getBounds().size.height-m_pText->getLabelSize().height)/2,0, m_pText->getLabelSize().height));
        willBg->setVisible(false);
        temporaryString="";
        isEditing =false;
        if (strcmp(m_sText.c_str(), "")) {
            m_pText->setText(m_sText);
            m_pMark->setCenterOrigin(CCPoint(m_pText->getLabelSize().width, this->getBounds().size.height/2));
        }else
        {
            m_pText->setText(m_sPlaceHolder.c_str());
            m_pMark->setCenterOrigin(CCPoint(0, this->getBounds().size.height/2));
        }
        
        
        
        
        return;
    }
}
void CATextField::deleteBackward()
{
    if (m_nInputType==KEY_BOARD_INPUT_PASSWORD)
    {
        m_pText->setTextcolor(m_cSpaceHolderColor);
        m_pText->setText(m_sPlaceHolder);
        m_sText="";
        spaceHolderIsOn=true;
        m_pMark->setCenterOrigin(CCPoint(0, this->getBounds().size.height/2));
        return;
    }
    if(spaceHolderIsOn)
    {
        
        return;
    }
    
    int nStrLen = m_pText->getText().length();
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
//
    // if all text deleted, show placeholder string
    
    if (nStrLen <= nDeleteLen)
    {
        m_pText->setTextcolor(m_cSpaceHolderColor);
        m_pText->setText(m_sPlaceHolder);
        m_sText="";
        spaceHolderIsOn=true;
        frontWidth=0;
        lengthArr.clear();
        lengthArr.push_back(0);
        bytePos =0;
        byteCount =0;
        byteArr.clear();
        byteArr.push_back(0);
        frontStr="";
        m_pMark->setCenterOrigin(CCPoint(0, this->getBounds().size.height/2));
        return;
    }
    
    // set new input text
    std::string sText(m_pText->getText().c_str(), nStrLen - nDeleteLen);

    char str[512]="";
    if ((*(byteArr.begin()+bytePos))<frontStr.length())
    {
        std::string beforeStr;

        {
            beforeStr=frontStr.substr(0,(*(byteArr.begin()+bytePos))-nDeleteLen);
        }
        beforeWidth = (*(lengthArr.begin()+bytePos));
        afterWidth = frontWidth-beforeWidth;
        std::string backStr= frontStr.substr((*(byteArr.begin()+bytePos)),strlen(frontStr.c_str()));
        sprintf(str, "%s%s",beforeStr.c_str(),backStr.c_str());
    }else
    {
        sprintf(str, "%s",sText.c_str());
    }

    
//    if ((*(byteArr.begin()+bytePos))<frontStr.length())
//    {
//        for (vector <float>::iterator i = (lengthArr.begin()+bytePos); i !=lengthArr.end(); ++i)
//        {
////            CCLog("%f   %f",*(i+1),*(i));
////            //  CCLog("%f",*(i+1));
//////            
////            *i=(*i)-((*(i+1)-*(i)));
////            CCLOG("%f",*i);
//        }
//    }
    
    lengthArr.erase(lengthArr.end()-1);
    
    
    if (byteArr.size()==1)
    {
        byteCount=0;
    }else{
        byteCount=(*(byteArr.end()-2));
    }
    
    byteArr.erase(byteArr.end()-1);
    
    if ( bytePos>0)
    {
        bytePos--;
    }
    else
    {
        return;
    }
    frontStr=str;
    CCRect rect = CCRectZero;
    rect.size = this->getBounds().size;
    rect.size.width = MIN(this->getBounds().size.width, m_pText->getLabelSize().width);
    m_pText->setText(str);
    m_sText=str;
    float markWidth =(*(lengthArr.begin()+bytePos));
    if (m_pText->getLabelSize().width>this->getBounds().size.width) {
        markWidth = this->getBounds().size.width;
        CCPoint pos = m_pText->getTextImage()->getFrame().origin;
        CCRect r =m_pText->getTextImage()->getImageRect();
        r.origin.x =(m_pText->getTextImage()->getImage()->getContentSize().width-this->getBounds().size.width);
        r.size.width = this->getBounds().size.width;
        m_pText->getTextImage()->setImageRect(r);
        m_pText->getTextImage()->setFrameOrigin(pos);
    }
    m_pText->setFrame(rect);
    
    m_pMark->setCenterOrigin(CCPoint(markWidth, this->getBounds().size.height/2));
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
NS_CC_END