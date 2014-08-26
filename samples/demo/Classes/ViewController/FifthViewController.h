#ifndef _Fifth_ViewController_h_
#define _Fifth_ViewController_h_

#include "CrossApp.h"

USING_NS_CC;

class FifthViewController :public CAViewController
{
public:
	FifthViewController();
	virtual ~FifthViewController();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

public:
	void hiddenViewController(CAControl* btn,CCPoint point);
	void isExecuteEndFunc(float intervalTime);

private:
	CCSize size;
	CCSize viewOriginalSize;
	CCPoint originalPoint;
	CAImageView* gestureView;
	std::vector<CCPoint> point_Vector;
	clock_t firstClick, secondClick, invalidClick;

	CCArray* pointSet;
	float isTouchEnd;
	float touchTime;

};

#endif