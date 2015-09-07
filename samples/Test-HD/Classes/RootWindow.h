

#ifndef __HelloCpp__RootWindow__
#define __HelloCpp__RootWindow__

#include <iostream>
#include "CDData.h"
#include "CASplitViewController.h"

class RootWindow: public CAWindow, public CAKeypadDelegate
{
    
public:

    static RootWindow* getInstance();
    
    RootWindow();
    
    virtual ~RootWindow();
    
    virtual bool init();
    virtual void draw();
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pSplitNavigationController, SplitNavigationController);
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pRootNavigationController, RootNavigationController);
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pRootNavigationController1, RootNavigationController1);
    
    virtual void keyBackClicked();
    CADipSize winSize;
    CASplitViewController* _splitViewController;
    CAVector<CrossApp::CAViewController *> _vec;
};


#endif /* defined(__HelloCpp__ViewController__) */
