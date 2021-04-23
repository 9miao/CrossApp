

#ifndef __HelloCpp__RootWindow__
#define __HelloCpp__RootWindow__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class RootWindow: public CAWindow
{
public:

    static RootWindow* getInstance();
    
    RootWindow();
    
    virtual ~RootWindow();
    
    virtual bool init();

    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pRootNavigationController, RootNavigationController);
    
    CC_SYNTHESIZE_READONLY(CADrawerController*, m_pRootDrawerController, DrawerController);
};


#endif /* defined(__HelloCpp__ViewController__) */
