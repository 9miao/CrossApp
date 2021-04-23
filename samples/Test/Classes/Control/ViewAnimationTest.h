
#ifndef __Test__ViewAnimationTest__
#define __Test__ViewAnimationTest__

#include "RootWindow.h"

class ViewAnimationTest: public CAViewController
{
    
public:

	ViewAnimationTest();
    
	virtual ~ViewAnimationTest();
    
    CREATE_FUNC(ViewAnimationTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void showIndex(ssize_t index);
};


#endif /* defined(__HelloCpp__ViewController__) */
