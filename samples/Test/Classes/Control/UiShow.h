
#ifndef __Test__UiShow__
#define __Test__UiShow__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class UiShow: public CAViewController, public CAPageViewDelegate
{
    
public:
    
    UiShow();
    
    virtual ~UiShow();
    
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
    
    void showUI();

    void buttonControlCallBack(CAControl* btn, DPoint point);
    
    void pageViewDidBeginTurning(CAPageView* pageView);
    
    void pageViewDidEndTurning(CAPageView* pageView);
    
    void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);

};





#endif /* defined(__HelloCpp__ViewController__) */