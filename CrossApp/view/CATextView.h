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

	virtual bool onTextViewAttachWithIME(CATextView* sender) { return false; }

	//If the sender doesn't want to detach from the IME, return true;
	virtual bool onTextViewDetachWithIME(CATextView* sender) { return false; }

	//If the sender doesn't want to insert the text, return true;
	virtual bool onTextViewInsertText(CATextView* sender, const char * text, int nLen) { return false; }

	//If the sender doesn't want to delete the delText, return true;
	virtual bool onTextViewDeleteBackward(CATextView* sender, const char * delText, int nLen) { return false; }

	virtual bool getKeyBoardHeight(int height) { return false; }

	virtual bool keyBoardCallBack(CATextView *sender) { return false; }
};


class CC_DLL CATextView : public CAScrollView, public CAIMEDelegate
{
public:
	CATextView();
	virtual ~CATextView();

	virtual void onEnterTransitionDidFinish();

	virtual bool resignFirstResponder();

	virtual bool becomeFirstResponder();

	static CATextView* createWithFrame(const CCRect& rect);

	static CATextView* createWithCenter(const CCRect& rect);

	bool initWithFrame(const CCRect& frame);

	bool initWithCenter(const CCRect& rect);

	virtual bool init();
	virtual bool canAttachWithIME();
	virtual bool canDetachWithIME();
	virtual void insertText(const char * text, int len);
	virtual void willInsertText(const char* text, int len);
	virtual void AndroidWillInsertText(int start, const char* str, int before, int count);
	virtual void deleteBackward();
	virtual void getKeyBoardHeight(int height);
	virtual void getKeyBoradReturnCallBack();
	virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
	virtual const char* getContentText();
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

	std::pair<int, int> getLineAndPos(int iPos);

	std::vector<CCRect> getZZCRect();

	void showTextViewMark(const std::vector<CCRect>& vt);
	void hideTextViewMark();

    inline virtual float maxSpeed(float dt);
    
    inline virtual float maxSpeedCache(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
public:

	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);

	virtual bool attachWithIME();

	virtual bool detachWithIME();
    
	virtual void selectAll();
	virtual void cursorMoveBackward();
	virtual void cursorMoveForward();
	virtual void moveSelectChars(bool isLeftBtn, const CCPoint& pt);
	virtual void moveSelectCharsCancel(const CCPoint& pt);
	virtual void moveArrowBtn(const CCPoint& pt);

	virtual void copyToClipboard();
	virtual void cutToClipboard();
	virtual void pasteFromClipboard();
    
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info);
    
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info);

private:
	CAView* m_pCursorMark;

	CAScale9ImageView* m_pBackgroundView;

	CAImageView* m_pImageView;
	std::vector<CAImageView*> m_pTextViewMarkVect;

	int m_iCurPos;
	int m_iLineHeight;
	std::vector<TextViewLineInfo> m_vLinesTextView;
	std::pair<int, int> m_curSelCharRange;
    bool m_isTouchInSide;
	bool m_bUpdateImage;
    eKeyBoardType m_keyboardType;
    eKeyBoardReturnType m_keyBoardReturnType;
};


NS_CC_END

#endif