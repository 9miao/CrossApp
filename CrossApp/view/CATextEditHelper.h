//
//  CATextEditHelper.h
//  CrossApp
//
//  Created by Zhujian on 15-5-5.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp_CATextEditHelper__
#define __CrossApp_CATextEditHelper__

#include "control/CAControl.h"
#include "CAImageView.h"


NS_CC_BEGIN


class CC_DLL CATouchView : public CAView
{
public:
	CATouchView();
	virtual ~CATouchView() {}

	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchPress(CATouch *pTouch, CAEvent *pEvent) = 0;

protected:
	void ccTouchTimer(float interval);

private:
	CATouch *m_pCurTouch;
	CAEvent *m_pCurEvent;
};


typedef struct CallbackTarget
{
	CallbackTarget(CAObject* t, SEL_CallFunc s, const std::string& szBtnText)
	: target(t)
	, selector(s)
	, cszButtonText(szBtnText)
	{

	}
	CAObject* target;
	SEL_CallFunc selector;
	std::string cszButtonText;

} CallbackTarget;


class CAControl;
class CAClippingView;


class CC_DLL CATextToolBarView : public CAView
{
public:
	CATextToolBarView();
	virtual ~CATextToolBarView();

	static CATextToolBarView *create();
	static bool isTextToolBarShow();
	static void hideTextToolBar();
    
	void addButton(const std::string& strBtnText, CAObject* target, SEL_CallFunc selector);
	void show(CAView* pView=NULL);

protected:
	bool init();
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
	void addGrayLine(int x);
	void alertViewCallback(CAControl* btn, CCPoint point);

private:
	std::vector<CallbackTarget> m_CallbackTargets;

	CAClippingView *m_pBackView;
    CAView* m_pControlView;
};


class CATextSelectView : public CAView
{
public:
	CATextSelectView();
	virtual ~CATextSelectView();

	static CATextSelectView *create();
	static void hideTextSelectView();

	void showTextSelView(const CCRect& rect, CAView* pControlView, bool showLeft = true, bool showRight = true);

protected:
	virtual bool init();
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
	
	void hideTextSelView();
	void ccCopyToClipboard();
	void ccCutToClipboard();
	void ccPasteFromClipboard();
	
private:

	CAImageView* m_pCursorMarkL;
	CAImageView* m_pCursorMarkR;
	
	CAView* m_pTextViewMask;
	CAView* m_pControlView;
	int m_iSelViewTouchPos;
};

class CATextSelViewEx : public CAControl
{
public:
	CATextSelViewEx();
	virtual ~CATextSelViewEx();

	static CATextSelViewEx *create();

	void showTextSelView(CAView* pControlView, const std::vector<CCRect>& vt, float iLineHeight);
	void hideTextSelView();
	void showTextViewMark(const std::vector<CCRect>& vt);
	void hideTextViewMark();
    bool isTextViewShow();
    
protected:
	virtual bool init();
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	bool touchSelectText(CATouch *pTouch);

private:
	CAImageView* m_pCursorMarkL;
	CAImageView* m_pCursorMarkR;
	CAView* m_pControlView;

	int m_iSelViewTouchPos;
	std::vector<CAView*> m_pTextViewMask;
};


class CATextArrowView : public CAView
{
public:
	CATextArrowView();
	virtual ~CATextArrowView();

	static CATextArrowView *create();

	void showTextArrView(const CCPoint& pt);
	void hideTextArrView();


protected:
	virtual bool init();
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	void ccTouchTimer(float interval);

private:
	bool m_isBtnPress;
	CAImageView* m_pArrowView;
};

class CC_DLL CATextResponder
{
public:
	CATextResponder();
	virtual ~CATextResponder();

	virtual void resignResponder() = 0;

	void resignAllResponder(CATextResponder* pCurExcept);
private:
	static std::vector<CATextResponder*> s_AllTextResponder;
};


NS_CC_END

#endif //__CrossApp_CATextEditHelper__