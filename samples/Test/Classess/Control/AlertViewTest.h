
#ifndef __Test__AlertViewTest__
#define __Test__AlertViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class AlertViewTest: public CAViewController, public CAPageViewDelegate
{
    
public:

	AlertViewTest();
    
	virtual ~AlertViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAPageView* p_PageViewVec;
    
    int showIndex;
    int showNum;
    int pageViewIndex;
    
    CAPageControl* pageControl;
    CAVector<CAView* > VIEWLIST;
    
public:
    
    void alertButtonCallBack(CAControl* btn,DPoint point);
    
    void alertCallBack(int btnIndex);
    
    void buttonControlCallBack(CAControl* btn, DPoint point);
    
    void showUI();
    
    void pageViewDidBeginTurning(CAPageView* pageView);
    
    void pageViewDidEndTurning(CAPageView* pageView);
    
    void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
