
#ifndef __Test__ImageViewTest__
#define __Test__ImageViewTest__

#include "RootWindow.h"

class ImageViewTest: public CAViewController
{
    
public:

	ImageViewTest();
    
	virtual ~ImageViewTest();
    
    CREATE_FUNC(ImageViewTest);
    
    CC_SYNTHESIZE(ssize_t, m_iIndex, Index);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

};


#endif /* defined(__HelloCpp__ViewController__) */
