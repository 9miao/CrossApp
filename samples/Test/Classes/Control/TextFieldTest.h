
#ifndef __Test__ViewController__
#define __Test__ViewController__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class TextFieldTest: public CAViewController
{
    
public:

	TextFieldTest();
    
	virtual ~TextFieldTest();
    
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
    
    CAPageControl* pageControl;
    CAVector<CAView* > VIEWLIST;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
