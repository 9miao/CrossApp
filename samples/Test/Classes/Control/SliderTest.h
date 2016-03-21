
#ifndef __Test__SliderTest__
#define __Test__SliderTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class SliderTest: public CAViewController, public CAPageViewDelegate
{
    
public:

	SliderTest();
    
	virtual ~SliderTest();
    
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
    
    CALabel* sliderValue1;
    CALabel* sliderValue2;
    CASlider* slider;
    
public:
    
    void sliderValueChange(CAControl* btn, DPoint point);
    
    void showUI();
    
    void buttonControlCallBack(CAControl* btn, DPoint point);
    
    void pageViewDidBeginTurning(CAPageView* pageView);
    
    void pageViewDidEndTurning(CAPageView* pageView);
    
    void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
