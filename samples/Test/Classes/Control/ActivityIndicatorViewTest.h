
#ifndef __Test__ActivityIndicatorViewTest__
#define __Test__ActivityIndicatorViewTest__

#include <iostream>
#include "RootWindow.h"

class ActivityIndicatorViewTest: public CAViewController
{
    
public:

	ActivityIndicatorViewTest();
    
	virtual ~ActivityIndicatorViewTest();
    
    CREATE_FUNC(ActivityIndicatorViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
};


#endif /* defined(__HelloCpp__ViewController__) */
