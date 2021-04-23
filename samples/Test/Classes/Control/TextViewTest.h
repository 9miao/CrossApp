
#ifndef __Test__TextViewTest__
#define __Test__TextViewTest__

#include "RootWindow.h"

class TextViewTest: public CAViewController, public CATextViewDelegate
{
    
public:

	TextViewTest();
    
	virtual ~TextViewTest();
    
    CREATE_FUNC(TextViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
};


#endif /* defined(__HelloCpp__ViewController__) */
