
#ifndef __Test__SegmentedControlTest__
#define __Test__SegmentedControlTest__

#include "RootWindow.h"

class SegmentedControlTest: public CAViewController
{
    
public:

	SegmentedControlTest();
    
	virtual ~SegmentedControlTest();
    
    CREATE_FUNC(SegmentedControlTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
};


#endif /* defined(__HelloCpp__ViewController__) */
