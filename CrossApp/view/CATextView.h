//
//  CATextView.h
//  CrossApp
//
//  Created by Zhujian on 14-9-3.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CATextView__
#define __CrossApp__CATextView__

#include "view/CAView.h"
#include "view/CAScrollView.h"
#include "view/CAScale9ImageView.h"
#include "view/CATextField.h"
#include "dispatcher/CAIMEDispatcher.h"


#include <vector>

NS_CC_BEGIN


class CATextView;

class CATextViewDelegate
{
public:
	virtual ~CATextViewDelegate(){};

	virtual bool onTextViewAttachWithIME(CATextView* sender)
    {
        CC_UNUSED_PARAM(sender);
        return true;
    }

	//If the sender doesn't want to detach from the IME, return true;
	virtual bool onTextViewDetachWithIME(CATextView* sender)
    {
        CC_UNUSED_PARAM(sender);
        return true;
    }

	//If the sender doesn't want to insert the text, return true;
	virtual bool onTextViewInsertText(CATextView* sender, const char * text, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(text);
        CC_UNUSED_PARAM(nLen);
        return false;
    }

	//If the sender doesn't want to delete the delText, return true;
	virtual bool onTextViewDeleteBackward(CATextView* sender, const char * delText, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(delText);
        CC_UNUSED_PARAM(nLen);
        return false;
    }

	virtual void getKeyBoardHeight(int height) {}

	virtual bool keyBoardCallBack(CATextView *sender) { return true; }
};


class CC_DLL CATextView : public CATouchView, public CAIMEDelegate
{
	friend class CATextSelViewEx;
public:
	CATextView();
	virtual ~CATextView();

	virtual void onEnterTransitionDidFinish();

    virtual void onExitTransitionDidStart();
    
	virtual bool resignFirstResponder();

	virtual bool becomeFirstResponder();

	static CATextView* createWithFrame(const CCRect& rect);

	static CATextView* createWithCenter(const CCRect& rect);

    void setBackGroundImage(CAImage* image);
    
    void setBackGroundColor(const CAColor4B &color);
    
protected:
	virtual bool init();
	virtual bool canAttachWithIME();
	virtual bool canDetachWithIME();
	virtual void didDetachWithIME();
	virtual void didAttachWithIME();
    
	virtual void insertText(const char * text, int len);
	virtual void willInsertText(const char* text, int len);
	virtual void AndroidWillInsertText(int start, const char* str, int before, int count);
	virtual void deleteBackward();
	virtual void getKeyBoardHeight(int height);
	virtual void getKeyBoradReturnCallBack();
	virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
	virtual const char* getContentText();
	virtual int getCursorPos();
	virtual void visit();

    
	CC_SYNTHESIZE(CATextViewDelegate*, m_pTextViewDelegate, TextViewDelegate);

	CC_PROPERTY_PASS_BY_REF(std::string, m_sPlaceHolder, PlaceHolder);

	CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cSpaceHolderColor, SpaceHolderColor);

	CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cTextColor, TextColor);

	CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cCursorColor, CursorColor);

	CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cFontColor, FontColor);

	CC_PROPERTY_PASS_BY_REF(std::string, m_szText, Text);

	CC_PROPERTY(int, m_iFontSize, FontSize);

	CC_PROPERTY(bool, m_bWordWrap, WordWrap);

	CC_PROPERTY(unsigned int, m_iLineSpacing, LineSpacing);

	CC_PROPERTY_PASS_BY_REF(std::string, m_szFontName, FontName);
    
    CC_SYNTHESIZE(eKeyBoardInputType, m_nInputType, InputType);
    
    inline void setKeyboardType (eKeyBoardType type) {m_keyboardType = type; }
    
    inline int getKeyboardType () {return m_keyboardType; }
    
    inline void setKeyboardReturnType (eKeyBoardReturnType type) {m_keyBoardReturnType = type; }
    
    inline int getKeyboardReturnType () {return m_keyBoardReturnType; }
    
protected:

	void initMarkSprite();

    void showCursorMark();
    
    void hideCursorMark();
    
	void updateImage();

	void calcCursorPosition();

	int getStringLength(const std::string &var);

	int getCurrentByPointY(int y);

	void calculateSelChars(const CCPoint& point, int& l, int& r, int& p);

	bool execCurSelCharRange();

	void ccStartSelect();
	void ccSelectAll() { selectAll(); }
	void ccPasteFromClipboard() { pasteFromClipboard(); }
    void ccCopyToClipboard() { copyToClipboard(); }
    void ccCutToClipboard() { cutToClipboard(); }
	int getStringCharCount(const std::string &var);

	std::pair<int, int> getLineAndPos(int iPos);

	std::vector<CCRect> getZZCRect();

protected:
	virtual void setContentSize(const CCSize& var);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchPress(CATouch *pTouch, CAEvent *pEvent);

	virtual bool attachWithIME();
	virtual bool detachWithIME();
    
	virtual void selectAll();
	virtual void cursorMoveBackward();
	virtual void cursorMoveForward();
	virtual void moveSelectChars(bool isLeftBtn, const CCPoint& pt);
	virtual void moveArrowBtn(const CCPoint& pt);

	virtual void copyToClipboard();
	virtual void cutToClipboard();
	virtual void pasteFromClipboard();
    
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info);

private:
    
	CAScrollView* m_pContainerView;

	CAView* m_pCursorMark;

	CAScale9ImageView* m_pBackgroundView;

	CAImageView* m_pImageView;

	bool m_bKeyboardOpen;
    bool m_bMoved;
	int m_iCurPos;
	int m_iLineHeight;
	std::vector<TextViewLineInfo> m_vLinesTextView;
	std::pair<int, int> m_curSelCharRange;
    bool m_isTouchInSide;
	bool m_bUpdateImage;
    eKeyBoardType m_keyboardType;
    eKeyBoardReturnType m_keyBoardReturnType;

	CATextSelViewEx* m_pTextSelView;
	CATextArrowView* m_pTextArrView;
};


NS_CC_END

#endif