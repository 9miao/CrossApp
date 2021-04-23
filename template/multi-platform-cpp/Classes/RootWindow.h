

#ifndef __HelloCpp__RootWindow__
#define __HelloCpp__RootWindow__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class RootWindow: public CAWindow
{
    
public:
    
    static RootWindow* getInstance();
    
    static void destroyInstance();
    
    RootWindow();
    
    virtual ~RootWindow();
    
    virtual bool init();
    
private:
    
    
};


#endif /* defined(__HelloCpp__ViewController__) */
