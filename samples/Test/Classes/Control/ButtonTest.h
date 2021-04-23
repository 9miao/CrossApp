
#ifndef __Test__ButtonTest__
#define __Test__ButtonTest__

#include "RootWindow.h"

class ButtonTest: public CAViewController
{
    
public:

	ButtonTest();
    
	virtual ~ButtonTest();
    
    CREATE_FUNC(ButtonTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
};


#endif /* defined(__HelloCpp__ViewController__) */
