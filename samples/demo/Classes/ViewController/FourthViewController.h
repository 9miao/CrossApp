#ifndef _Fourth_ViewController_h_
#define _Fourth_ViewController_h_

#include "CrossApp.h"

USING_NS_CC;

class FourthViewController :public CAViewController
{
public:
	FourthViewController();
	virtual ~FourthViewController();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

private:
	CCSize size;
	CCPoint originalPoint;
	CAImageView* imageView;

};

#endif