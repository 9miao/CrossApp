
#ifndef __Test__TextViewTest__
#define __Test__TextViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class TextViewTest: public CAViewController, public CATextViewDelegate
{
    
public:

	TextViewTest();
    
	virtual ~TextViewTest();
    
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

    
};


#endif /* defined(__HelloCpp__ViewController__) */
