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
#include "dispatcher/CAIMEDispatcher.h"
#include "control/CATextField.h"

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
};


class CC_DLL CATextView : public CAScrollView, public CAIMEDelegate
{
public:
	CATextView();
	virtual ~CATextView();

	virtual bool resignFirstResponder();

	virtual bool becomeFirstResponder();

	static CATextView* createWithFrame(const CCRect& rect);

	static CATextView* createWithCenter(const CCRect& rect);

	bool initWithFrame(const CCRect& frame);

	bool initWithCenter(const CCRect& rect);

	void setBackGroundImage(CAImage *image);

	CAImage *getBackGroundImage();

	virtual bool init();
	virtual bool canAttachWithIME();
	virtual bool canDetachWithIME();
	virtual void insertText(const char * text, int len);
	virtual void willInsertText(const char* text, int len);
	virtual void AndroidWillInsertText(int start, const char* str, int before, int count);
	virtual void deleteBackward();
	virtual void visit();

    
	CC_SYNTHESIZE(CATextViewDelegate*, m_pTextViewDelegate, TextViewDelegate);

	CC_PROPERTY(CAColor4B, m_cCursorColor, CursorColor);

	CC_PROPERTY(int, m_iFontSize, FontSize);

	CC_PROPERTY(bool, m_bWordWrap, WordWrap);

	CC_PROPERTY(unsigned int, m_iLineSpacing, LineSpacing);

	CC_PROPERTY_PASS_BY_REF(std::string, m_szFontName, FontName);
    
protected:

	void initMarkSprite();

	void updateImage();

	void calcCursorPosition();

	int getStringLength(const std::string &var);

    inline virtual float maxSpeed(float dt);
    
    inline virtual float maxSpeedCache(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
public:

	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);

	virtual bool attachWithIME();

	virtual bool detachWithIME();

private:
	CAView* m_pCursorMark;

	CAScale9ImageView* m_pBackgroundView;

	CAImageView* m_pImageView;

	std::string m_szText;
	int m_iCurPos;

	int m_iLineHeight;
	std::vector<TextViewLineInfo> m_vLinesTextView;

	bool m_bUpdateImage;
};


NS_CC_END

#endif