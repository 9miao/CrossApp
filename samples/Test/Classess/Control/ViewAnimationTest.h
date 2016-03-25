
#ifndef __Test__ViewAnimationTest__
#define __Test__ViewAnimationTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ViewAnimationTest: public CAViewController, public CAPageViewDelegate
{
    
public:

	ViewAnimationTest();
    
	virtual ~ViewAnimationTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAPageView* p_PageViewVec;
    
    DSize winSize;
    DRect winRect;
    int showIndex;
    int showNum;
    int pageViewIndex;
    
    CAVector<CAView* > VIEWLIST;
    
    
    int heart_index;
    
    //Animation
    CAImageView* animation_1_view;
    CATextField* animation_2_textfield;
    CAButton* animation_2_btn_search;
    CAButton* animation_2_btn_cancel;
    CAImageView* animation_3_imageview;
    
public:
    
    void endAction();
    
    void doAction(CAControl* btn,DPoint point);
    
    void showUI();
    
    void buttonControlCallBack(CAControl* btn, DPoint point);
    
    void pageViewDidBeginTurning(CAPageView* pageView);
    
    void pageViewDidEndTurning(CAPageView* pageView);
    
    void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
};


#endif /* defined(__HelloCpp__ViewController__) */
