
#ifndef __Test__AlertViewTest__
#define __Test__AlertViewTest__

#include <iostream>
#include "RootWindow.h"

class AlertViewTest: public CAViewController
{
    
public:

	AlertViewTest();
    
	virtual ~AlertViewTest();
    
    CREATE_FUNC(AlertViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    void alertCallBack(int btnIndex);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
