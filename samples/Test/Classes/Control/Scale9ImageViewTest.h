
#ifndef __Test__Scale9ImageViewTest__
#define __Test__Scale9ImageViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class Scale9ImageViewTest: public CAViewController
{
    
public:

	Scale9ImageViewTest();
    
	virtual ~Scale9ImageViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:

    DSize winSize;
    DRect winRect;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
