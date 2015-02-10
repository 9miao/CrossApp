#ifndef _Fourth_ViewController_h_
#define _Fourth_ViewController_h_

#include <iostream>
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
	virtual void viewDidAppear();

public:
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

private:
	CADipSize size;
	CADipPoint originalPoint;
	CAImageView* imageView;
    CAImageView* imageView2;
};

#endif