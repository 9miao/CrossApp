
#ifndef __Test__SegmentedControlTest__
#define __Test__SegmentedControlTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class SegmentedControlTest: public CAViewController
{
    
public:

	SegmentedControlTest();
    
	virtual ~SegmentedControlTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    void segmentCallback(CASegmentedControl* btn, int index);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
