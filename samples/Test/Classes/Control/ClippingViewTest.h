
#ifndef __Test__ClippingViewTest__
#define __Test__ClippingViewTest__

#include "RootWindow.h"

class ClippingViewTest: public CAViewController
{
    
public:

	ClippingViewTest();
    
	virtual ~ClippingViewTest();
    
    CREATE_FUNC(ClippingViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
};


#endif /* defined(__Test__ClippingViewTest__) */
