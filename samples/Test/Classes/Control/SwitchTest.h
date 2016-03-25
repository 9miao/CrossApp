
#ifndef __Test__SwitchTest__
#define __Test__SwitchTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class SwitchTest: public CAViewController
{
    
public:

	SwitchTest();
    
	virtual ~SwitchTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    void switchStateChange(CAControl* btn, DPoint point);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
