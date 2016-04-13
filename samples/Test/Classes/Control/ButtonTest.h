
#ifndef __Test__ButtonTest__
#define __Test__ButtonTest__

#include "CrossApp.h"

USING_NS_CC;

class ButtonTest: public CAViewController
{
    
public:

	ButtonTest();
    
	virtual ~ButtonTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    void Buttoncallback(CAControl* btn, DPoint point);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
