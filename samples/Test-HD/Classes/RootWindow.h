

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
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pRootNavigationController, RootNavigationController);
    CC_SYNTHESIZE_READONLY(CASplitViewController*, m_pRootDrawerController, DrawerController);

    void initUIView();
    void intNewsView();
    
    virtual void keyBackClicked();
    void buttonCallBack(CAControl* btn,CCPoint point);
};


#endif /* defined(__HelloCpp__ViewController__) */
