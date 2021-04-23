
#ifndef __Test__ScrollViewTest__
#define __Test__ScrollViewTest__

#include "RootWindow.h"

class ScrollViewTest: public CAViewController
{
    
public:

	ScrollViewTest();
    
	virtual ~ScrollViewTest();
    
    CREATE_FUNC(ScrollViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

};


#endif /* defined(__HelloCpp__ViewController__) */
