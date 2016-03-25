
#ifndef __Test__WebViewTest__
#define __Test__WebViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class WebViewTest: public CAViewController, public CAWebViewDelegate
{
    
public:

	WebViewTest();
    
	virtual ~WebViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    
};


#endif /* defined(__HelloCpp__ViewController__) */
