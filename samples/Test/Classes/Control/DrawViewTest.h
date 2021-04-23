
#ifndef __Test__DrawViewTest__
#define __Test__DrawViewTest__

#include "RootWindow.h"

class DrawViewTest: public CAViewController
{
    
public:

	DrawViewTest();
    
	virtual ~DrawViewTest();
    
    CREATE_FUNC(DrawViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

};


#endif /* defined(__Test__DrawViewTest__) */
