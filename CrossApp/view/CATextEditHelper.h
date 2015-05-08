//
//  CATextEditHelper.h
//  CrossApp
//
//  Created by Zhujian on 15-5-5.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp_CATextEditHelper__
#define __CrossApp_CATextEditHelper__

#include "CAView.h"
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

typedef void (CAObject::*SEL_CATextTooBarBtnEvent)();
#define CATextToolBarView_selector(_SELECTOR) (SEL_CATextTooBarBtnEvent)(&_SELECTOR)

typedef struct CallbackTarget
{
	CallbackTarget(CAObject* t, SEL_CATextTooBarBtnEvent s, const std::string& szBtnText)
	: target(t)
	, selector(s)
	, cszButtonText(szBtnText)
	{

	}
	CAObject* target;
	SEL_CATextTooBarBtnEvent selector;
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

	void addButton(const std::string& strBtnText, CAObject* target, SEL_CATextTooBarBtnEvent selector);
	void show();

protected:
	bool init();
	void addGrayLine(int y);
	void alertViewCallback(CAControl* btn, CCPoint point);
	bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);

private:
	std::vector<CallbackTarget> m_CallbackTargets;

	CAClippingView *m_pBackView;
};


NS_CC_END

#endif //__CrossApp_CATextEditHelper__