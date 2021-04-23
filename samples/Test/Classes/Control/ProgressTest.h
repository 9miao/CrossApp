
#ifndef __Test__ProgressTest__
#define __Test__ProgressTest__

#include "RootWindow.h"

class ProgressTest: public CAViewController
{
    
public:

	ProgressTest();
    
	virtual ~ProgressTest();
    
    CREATE_FUNC(ProgressTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
};


#endif /* defined(__HelloCpp__ViewController__) */
