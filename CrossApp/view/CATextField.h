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

#include <vector>
#include <utility>
#include "dispatcher/CAIMEDispatcher.h"
#include "control/CAControl.h"
#include "view/CAScale9ImageView.h"
#include "support/ccUTF8.h"
#include "platform/CAFTFontCache.h"
#include "support/ConvertUTF.h"

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

enum eKeyBoardReturnType
{
    KEY_BOARD_RETURN_DONE = 21,
    KEY_BOARD_RETURN_SEARCH,
    KEY_BOARD_RETURN_SEND,
    KEY_BOARD_RETURN_ENTER,
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
    
    virtual bool getKeyBoardHeight(int height)
    {
        return false;
    }
    
    virtual bool keyBoardCallBack(CATextField *sender)
    {
        return false;
    }
};


class CC_DLL CATextField
: public CAView
, public CAIMEDelegate
{
public:
    CATextField();
    
    virtual ~CATextField();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual bool resignFirstResponder();
    
    virtual bool becomeFirstResponder();
    
    static CATextField* createWithFrame(const CCRect& frame);
    
    static CATextField* createWithCenter(const CCRect& rect);
    
    bool init();
    
    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);
    
    CC_PROPERTY_PASS_BY_REF(std::string, m_sPlaceHolder, PlaceHolder);
    
    CC_PROPERTY_PASS_BY_REF(std::string, m_sText, Text);
    
    CC_SYNTHESIZE_READONLY(int, m_nCharCount, CharCount);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cSpaceHolderColor, SpaceHolderColor);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cTextColor, TextColor);
    
    CC_SYNTHESIZE(CATextFieldDelegate*, m_pDelegate, Delegate);
    
    CC_PROPERTY(int, m_iFontSize, FontSize);
    
    CC_PROPERTY(eKeyBoardInputType, m_nInputType, InputType);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cCursorColor, CursorColor);
    
	CC_PROPERTY(int, m_iHoriMargins, HoriMargins);
	CC_PROPERTY(int, m_iVertMargins, VertMargins);
    
    inline void setKeyboardType (eKeyBoardType type) {m_keyboardType = type; }
    
    inline int getKeyboardType () {return m_keyboardType; }
    
    inline void setKeyboardReturnType (eKeyBoardReturnType type) {m_keyBoardReturnType = type; }
    
    inline int getKeyboardReturnType () {return m_keyBoardReturnType; }
    
	void analyzeString(const char * text, int len);
    
    virtual void setImageRect(const CCRect& rect);
    
    virtual void updateImageRect();
    


protected:
    void updateImage();
    int getCursorX();
    int getStringViewLength();
    virtual bool attachWithIME();
    virtual bool detachWithIME();
    virtual bool canAttachWithIME();
    virtual bool canDetachWithIME();
    int getStringLength(const std::string &var);
    static int getStringCharCount(const std::string &var);
    virtual void setContentSize(const CCSize& var);
    
    void initMarkSprite();
    void showCursorMark();
    void hideCursorMark();
    
	void calculateSelChars(const CCPoint& point, int& l, int& r, int& p);
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void insertText(const char * text, int len);
    virtual void willInsertText(const char* text,int len);
    virtual void AndroidWillInsertText(int start,const char* str,int before,int count);
    virtual void deleteBackward();
    virtual void getKeyBoardHeight(int height);
    virtual void getKeyBoradReturnCallBack();
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);

    void adjustCursorMoveBackward();
    void adjustCursorMoveForward();

    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info);
    
	CCRect getZZCRect(bool* l=NULL, bool* r=NULL);
	bool execCurSelCharRange();

    virtual void selectAll();
	virtual void cursorMoveBackward();
	virtual void cursorMoveForward();
	virtual void moveSelectChars(bool isLeftBtn, const CCPoint& pt);
	virtual void moveSelectCharsCancel(const CCPoint& pt);
	virtual void moveArrowBtn(const CCPoint& pt);

	virtual void copyToClipboard();
	virtual void cutToClipboard();
	virtual void pasteFromClipboard();

    virtual const char* getContentText();
    virtual int getCursorPos();

private:
	std::vector<TextAttribute> m_vTextFiledChars;
	std::pair<int, int> m_curSelCharRange;
	int m_iCurPos;

	int m_iLabelWidth;
	int m_iString_left_offX;
	int m_iString_l_length;
	int m_iString_r_length;
	int m_iFontHeight;

    bool m_isTouchInSide;
	CAView* m_pCursorMark;
	CAView* m_pTextViewMark;
	CCSize m_cImageSize;
	eKeyBoardType m_keyboardType;
    eKeyBoardReturnType m_keyBoardReturnType;
};

NS_CC_END

#endif /* defined(__project__CATextField__) */
