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
    void doAction();
    void refreshView(bool animation);
    void willStartAction(const string& animationID, void* context);
    void didStopAction(const string& animationID, void* context);
    
public:

    void buttonCallback(CAControl* btn, CCPoint point);
    void playAnimation(CAControl* btn, CCPoint point);
private:
	CADipSize size;
    CADipSize viewSize;
	CADipPoint originalPoint;
	CAView* animationView1;
    CAScale9ImageView* animationView2;
    int action_index;
    bool flag;
    CAView* defaultView;
    CALabel* defaultLable;
    bool isDoAction;
    CAButton* defaultBtnL;
    CAButton* defaultBtnR;
    CAButton* playAnimationBtn;
};
#endif