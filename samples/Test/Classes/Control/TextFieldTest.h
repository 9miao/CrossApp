
#ifndef __Test__ViewController__
#define __Test__ViewController__

#include "RootWindow.h"

class TextFieldTest: public CAViewController, public CATextFieldDelegate
{
    
public:

	TextFieldTest();
    
	virtual ~TextFieldTest();
    
    CREATE_FUNC(TextFieldTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
};


#endif /* defined(__HelloCpp__ViewController__) */
