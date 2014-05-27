//
//  CATextField.h
//  project
//
//  Created by lh on 14-5-15.
//
//

#ifndef __project__CATextField__
#define __project__CATextField__

#include <iostream>
#include "CCIMEDispatcher.h"
#include "label_nodes/CALabel.h"
#include "ui_controller/CAControl.h"
#include <vector>
NS_CC_BEGIN
class CATextField;
class CC_DLL CATextFieldDelegate
{
public:
    virtual bool onTextFieldAttachWithIME(CATextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
    /**
     @brief    If the sender doesn't want to detach from the IME, return true;
     */
    virtual bool onTextFieldDetachWithIME(CATextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
    /**
     @brief    If the sender doesn't want to insert the text, return true;
     */
    virtual bool onTextFieldInsertText(CATextField * sender, const char * text, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(text);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
    /**
     @brief    If the sender doesn't want to delete the delText, return true;
     */
    virtual bool onTextFieldDeleteBackward(CATextField * sender, const char * delText, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(delText);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
    /**
     @brief    If the sender doesn't want to draw, return true.
     */
    virtual bool onDraw(CATextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
};

typedef struct _TextAttribute
{
    unsigned int charsize;
    
    float charlength;
 
}TextAttribute;

class CC_DLL CATextField
: public CAControl
, CCIMEDelegate
{
public:
    CATextField();
    
    virtual ~CATextField();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
public:
    
    static CATextField* create(CCRect frame);
    
    bool initWithTextFieldFrame(CCRect frame);
    
    CC_PROPERTY(std::string, m_sPlaceHolder, PlaceHolder);
    
    CC_SYNTHESIZE_READONLY(int, m_nCharCount, CharCount);
    
    CC_PROPERTY(ccColor4B, m_cSpaceHolderColor, SpaceHolderColor);
    
    CC_PROPERTY(ccColor4B, m_cTextColor, TextColor);
    
    CC_PROPERTY(CCTextAlignment, m_aTextAlignment, TextAlignment);
    
    CC_SYNTHESIZE(CATextFieldDelegate*, m_pDelegate, Delegate);
    
    CC_PROPERTY(float, m_fFontSize, FontSize);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual bool attachWithIME();//打开键盘
    
    virtual bool detachWithIME();//关闭键盘
    
private:
    
    unsigned int charpos;
    bool spaceHolderIsOn;
    float _oldPos;
    unsigned int frontlength;
    unsigned int backlength;
    CALabel *m_pText;
    CCAction *m_pCursorAction;
    std::string m_sInputString;
    CAImageView *m_pMark;
    bool status;
    void initMarkSprite();
    
protected:
    
    std::string m_pPlaceHolderString;
    
protected:
    
    virtual void draw();
    virtual bool canAttachWithIME();
    virtual bool canDetachWithIME();
    virtual void insertText(const char * text, int len);
    virtual void deleteBackward();
    virtual const char * getContentText();
};

NS_CC_END

#endif /* defined(__project__CATextField__) */
