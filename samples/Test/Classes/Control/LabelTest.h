
#ifndef __Test__LabelTest__
#define __Test__LabelTest__

#include "RootWindow.h"

class LabelTest: public CAViewController
{
    
public:

	LabelTest();
    
	virtual ~LabelTest();
    
    CREATE_FUNC(LabelTest);
    
    CC_SYNTHESIZE(ssize_t, m_iIndex, Index);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
};


#endif /* defined(__HelloCpp__ViewController__) */
