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
    virtual bool onTextViewInsertText(CATextView* sender, const char * text, int nLen, int nPosition)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(text);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
    //If the sender doesn't want to delete the delText, return true;
    virtual bool onTextViewDeleteBackward(CATextView* sender, const char * delText, int nLen, int nPosition)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(delText);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
	//If the sender doesn't want to insert the text, return true;
	CC_DEPRECATED_ATTRIBUTE virtual bool onTextViewInsertText(CATextView* sender, const char * text, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(text);
        CC_UNUSED_PARAM(nLen);
        return false;
    }

	//If the sender doesn't want to delete the delText, return true;
	CC_DEPRECATED_ATTRIBUTE virtual bool onTextViewDeleteBackward(CATextView* sender, const char * delText, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(delText);
        CC_UNUSED_PARAM(nLen);
        return false;
    }

	virtual void getKeyBoardHeight(int height) {}
};


class CC_DLL CATextView 
	: public CATouchView
	, public CAIMEDelegate
	, public CATextResponder
{
	friend class CATextSelViewEx;
public:
	CATextView();
	virtual ~CATextView();

	virtual void onEnterTransitionDidFinish();

    virtual void onExitTransitionDidStart();
    
	virtual bool resignFirstResponder();

	virtual bool becomeFirstResponder();

	virtual void resignResponder();

	static CATextView* createWithFrame(const DRect& rect);

	static CATextView* createWithCenter(const DRect& rect);

    void setBackGroundImage(CAImage* image);
    
    void setBackGroundColor(const CAColor4B &color);

	int getLineCount();
    
	CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);

	CC_SYNTHESIZE(CATextViewDelegate*, m_pTextViewDelegate, TextViewDelegate);

	CC_PROPERTY_PASS_BY_REF(std::string, m_sPlaceHolder, PlaceHolder);
    
	CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cPlaceHolderColor, PlaceHolderColor);

	CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cTextColor, TextColor);

	CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cCursorColor, CursorColor);

	CC_PROPERTY_PASS_BY_REF(std::string, m_szText, Text);

	CC_PROPERTY(int, m_iFontSize, FontSize);

	CC_PROPERTY(bool, m_bWordWrap, WordWrap);

	CC_PROPERTY(unsigned int, m_iLineSpacing, LineSpacing);

	CC_SYNTHESIZE(int, m_iHoriMargins, HoriMargins);
	CC_SYNTHESIZE(int, m_iVertMargins, VertMargins);

	CC_PROPERTY_PASS_BY_REF(std::string, m_szFontName, FontName);
    
    CC_SYNTHESIZE(eKeyBoardInputType, m_nInputType, InputType);
    
    inline void setKeyboardType (eKeyBoardType type) {m_keyboardType = type; }
    
    inline int getKeyboardType () {return m_keyboardType; }
    
    inline void setKeyboardReturnType (eKeyBoardReturnType type) {m_keyBoardReturnType = type; }
    
    inline int getKeyboardReturnType () {return m_keyBoardReturnType; }
    
    CC_DEPRECATED_ATTRIBUTE void setSpaceHolderColor(const CAColor4B& color)
    {
        return setPlaceHolderColor(color);
    }
    
    CC_DEPRECATED_ATTRIBUTE const CAColor4B& getSpaceHolderColor()
    {
        return getPlaceHolderColor();
    }

	void setColor(const CAColor4B& var);

	const CAColor4B& getColor();
    
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

	virtual void setContentSize(const DSize& var);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchPress(CATouch *pTouch, CAEvent *pEvent);

	virtual bool attachWithIME();
	virtual bool detachWithIME();
    
	virtual void selectAll();
	virtual void cursorMoveBackward();
	virtual void cursorMoveForward();
    virtual void cursorMoveUp();
    virtual void cursorMoveDown();
	virtual void moveSelectChars(bool isLeftBtn, const DPoint& pt);
	virtual void moveArrowBtn(const DPoint& pt);

	virtual void copyToClipboard();
	virtual void cutToClipboard();
	virtual void pasteFromClipboard();

	void initMarkSprite();
	void showCursorMark();
	void hideCursorMark();
	void updateImage();
	void calcCursorPosition();
	int getStringLength(const std::string &var);
	int getCurrentByPointY(int y);
	void calculateSelChars(const DPoint& point, int& l, int& r, int& p);
	bool execCurSelCharRange();

	void ccStartSelect();
	void ccSelectAll() { selectAll(); }
	void ccPasteFromClipboard() { pasteFromClipboard(); }
	void ccCopyToClipboard() { copyToClipboard(); }
	void ccCutToClipboard() { cutToClipboard(); }
	int getStringCharCount(const std::string &var);

	std::pair<int, int> getLineAndPos(int iPos);
	std::vector<DRect> getZZCRect();

private:
    
	CAScrollView* m_pContainerView;

	CAView* m_pCursorMark;

	CAImageView* m_pImageView;

	bool m_bKeyboardOpen;
    bool m_bMoved;
	int m_iCurPos;
	int m_iLineHeight;
    bool m_bFirstInput;
    
	std::vector<TextViewLineInfo> m_vLinesTextView;
	std::pair<int, int> m_curSelCharRange;
	bool m_bUpdateImage;
    eKeyBoardType m_keyboardType;
    eKeyBoardReturnType m_keyBoardReturnType;

	CATextSelViewEx* m_pTextSelView;
	CATextArrowView* m_pTextArrView;
    
    DPoint m_pCurPosition;
	CAColor4B m_cFontColor;
};


NS_CC_END

#endif