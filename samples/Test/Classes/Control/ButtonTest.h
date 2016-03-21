
#ifndef __Test__ButtonTest__
#define __Test__ButtonTest__

#include "CrossApp.h"

USING_NS_CC;

class ButtonTest: public CAViewController, public CAPageViewDelegate
{
    
public:

	ButtonTest();
    
	virtual ~ButtonTest();
    
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
    
    void Buttoncallback(CAControl* btn, DPoint point);
    
    void showUI();
    
    void buttonControlCallBack(CAControl* btn, DPoint point);
    
    void pageViewDidBeginTurning(CAPageView* pageView);
    
    void pageViewDidEndTurning(CAPageView* pageView);
    
    void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    
    
    
};


#endif /* defined(__HelloCpp__ViewController__) */
