
#ifndef __Test__ProgressTest__
#define __Test__ProgressTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ProgressTest: public CAViewController
{
    
public:

	ProgressTest();
    
	virtual ~ProgressTest();
    
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
    
    CALabel* progress_value;
    CAProgress* progress;
    
public:
    
    void progressValueChange(float _t);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
