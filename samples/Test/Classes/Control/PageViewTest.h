
#ifndef __Test__PageViewTest__
#define __Test__PageViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class PageViewTest: public CAViewController, public CAPageViewDelegate
{
    
public:

	PageViewTest();
    
	virtual ~PageViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAPageView* p_PageViewVec;
    
    int showIndex;
    int showNum;
    int pageViewIndex;
    
    CAVector<CAView* > VIEWLIST;
    
    CAPageView* p_pageView;
    CAPageControl* pageControl;
    
public:
    
    void pageControlCallBack(CrossApp::CAControl *btn, CrossApp::DPoint point);
    
    void buttonControlCallBack(CAControl* btn, DPoint point);
    
    void pageViewDidBeginTurning(CAPageView* pageView);
    
    void pageViewDidEndTurning(CAPageView* pageView);
    
    void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);

};


#endif /* defined(__HelloCpp__ViewController__) */
