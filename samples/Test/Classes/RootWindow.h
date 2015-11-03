

#ifndef __HelloCpp__RootWindow__
#define __HelloCpp__RootWindow__

#include <iostream>
#include "CDData.h"

class RootWindow: public CAWindow, public CAKeypadDelegate
{
    
public:

    static RootWindow* getInstance();
    
    RootWindow();
    
    virtual ~RootWindow();
    
    virtual bool init();
    virtual void draw();
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pRootNavigationController, RootNavigationController);
    CC_SYNTHESIZE_READONLY(CADrawerController*, m_pRootDrawerController, DrawerController);

    void initUIView();
    void intNewsView();
    
    virtual void keyBackClicked();
    void buttonCallBack(CAControl* btn,DPoint point);
};


#endif /* defined(__HelloCpp__ViewController__) */
