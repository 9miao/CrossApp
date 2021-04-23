
#ifndef __Test__RenderImageTest__
#define __Test__RenderImageTest__

#include "RootWindow.h"

class RenderImageTest: public CAViewController
{
    
public:

	RenderImageTest();
    
	virtual ~RenderImageTest();
    
    CREATE_FUNC(RenderImageTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

};


#endif /* defined(__HelloCpp__ViewController__) */
