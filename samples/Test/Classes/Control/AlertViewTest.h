
#ifndef __Test__AlertViewTest__
#define __Test__AlertViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class AlertViewTest: public CAViewController
{
    
public:

	AlertViewTest();
    
	virtual ~AlertViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    void alertButtonCallBack(CAControl* btn,DPoint point);
    
    void alertCallBack(int btnIndex);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
