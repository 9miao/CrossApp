
#ifndef __Test__ImageViewTest__
#define __Test__ImageViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

#define MAX_COUNT 5  

class ImageViewTest: public CAViewController, public CAPageViewDelegate
{
    
public:

	ImageViewTest();
    
	virtual ~ImageViewTest();
    
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
    
    CAImageView* AnimationImages();
    
    void showUI();
    
    void buttonControlCallBack(CAControl* btn, DPoint point);
    
    void pageViewDidBeginTurning(CAPageView* pageView);
    
    void pageViewDidEndTurning(CAPageView* pageView);
    
    void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
};


#endif /* defined(__HelloCpp__ViewController__) */
