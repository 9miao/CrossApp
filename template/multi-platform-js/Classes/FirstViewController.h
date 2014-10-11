
#ifndef __HelloCpp__ViewController__
#define __HelloCpp__ViewController__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class FirstViewController: public CAViewController
{
    
public:

	FirstViewController();
    
	virtual ~FirstViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
};


#endif /* defined(__HelloCpp__ViewController__) */
