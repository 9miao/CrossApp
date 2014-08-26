//
//  CATextField.h
//  CrossApp
//
//  Created by lh on 14-5-15.
//  Modified by zhujian on 14-8-4
//
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __project__CATextField__
#define __project__CATextField__

#include <iostream>
#include "dispatcher/CAIMEDispatcher.h"
#include "control/CAControl.h"
#include <vector>
#include "view/CAScale9ImageView.h"
#include "support/ccUTF8.h"
NS_CC_BEGIN


enum eKeyBoardType
{
    KEY_BOARD_TYPE_NORMAL = 0,
    KEY_BOARD_TYPE_NUMBER,
    KEY_BOARD_TYPE_ALPHABET,
};

enum eKeyBoardInputType
{
    KEY_BOARD_INPUT_NORMAL = 1,
    KEY_BOARD_INPUT_PASSWORD,
};

class CATextField;
class CC_DLL CATextFieldDelegate
{
public:
    virtual bool onTextFieldAttachWithIME(CATextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
	//If the sender doesn't want to detach from the IME, return true;
    virtual bool onTextFieldDetachWithIME(CATextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
	//If the sender doesn't want to insert the text, return true;
    virtual bool onTextFieldInsertText(CATextField * sender, const char * text, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(text);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
    //If the sender doesn't want to delete the delText, return true;
    virtual bool onTextFieldDeleteBackward(CATextField * sender, const char * delText, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(delText);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
};

typedef struct _TextAttribute
{
    int charSize;
    float charlength;
 
}TextAttribute;


class CC_DLL CATextField
: public CAControl
, public CAIMEDelegate
{
public:
    CATextField();
    
    virtual ~CATextField();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual bool resignFirstResponder();
    
    virtual bool becomeFirstResponder();
    
    void setBackGroundImage(CAImage *image);
    
    CAImage *getBackGroundImage();
    
    static CATextField* createWithFrame(const CCRect& frame);
    
    static CATextField* createWithCenter(const CCRect& rect);
    
    bool initWithFrame(const CCRect& frame);
    
    bool initWithCenter(const CCRect& rect);
    
    CC_PROPERTY(std::string, m_sPlaceHolder, PlaceHolder);
    
    CC_PROPERTY(std::string, m_sText, Text);
    
    CC_SYNTHESIZE_READONLY(int, m_nCharCount, CharCount);
    
    CC_PROPERTY(CAColor4B, m_cSpaceHolderColor, SpaceHolderColor);
    
    CC_PROPERTY(CAColor4B, m_cTextColor, TextColor);
    
    CC_SYNTHESIZE(CATextFieldDelegate*, m_pDelegate, Delegate);
    
    CC_PROPERTY(int, m_iFontSize, FontSize);
    
    CC_PROPERTY(eKeyBoardInputType, m_nInputType, InputType);
    
    CC_PROPERTY(CAColor4B, m_cCursorColor, CursorColor);

	CC_PROPERTY(int, m_iHoriMargins, HoriMargins);
	CC_PROPERTY(int, m_iVertMargins, VertMargins);
    
    inline void setKeyboardType (eKeyBoardType type) {m_keyboardType = type; }
    
    inline int getKeyboardType () {return m_keyboardType; }
    
	void analyzeString(const char * text, int len);
    
    virtual void setImageRect(const CCRect& rect);
    
    virtual void updateImageRect();

protected:
    void updateImage();
    int getCursorX();
    int getStringViewLength();
    int getStringLength(const std::string &var);
    int getStringCharCount(const std::string &var);
    
    virtual void setContentSize(const CCSize& var);
    virtual bool attachWithIME();
    virtual bool detachWithIME();
    void         initMarkSprite();
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual bool canAttachWithIME();
    virtual bool canDetachWithIME();
    virtual void insertText(const char * text, int len);
    virtual void willInsertText(const char* text,int len);
    virtual void AndroidWillInsertText(int start,const char* str,int before,int count);
    virtual void deleteBackward();
    virtual const char* getContentText();
private:
	std::vector<TextAttribute> m_vTextFiledChars;
	int m_iCurPos;
	CAScale9ImageView* m_pBackgroundView;
	int m_iLabelWidth;
	int m_iString_left_offX;
	int m_iString_l_length;
	int m_iString_r_length;
	int m_iFontHeight;

	CCAction* m_pCursorAction;
	CAView* m_pCursorMark;
	CCSize m_cImageSize;
	eKeyBoardType m_keyboardType;
};

NS_CC_END

#endif /* defined(__project__CATextField__) */
