//
//  MenuViewController.h
//  Test
//
//  Created by renhongguang on 15/4/3.
//
//

#ifndef __Test__MenuViewController__
#define __Test__MenuViewController__

#include "RootWindow.h"

class MenuViewController : public CAViewController
{
public:
    
    MenuViewController();
    
    virtual ~MenuViewController();
    
    CREATE_FUNC(MenuViewController);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void buttonCallBack(CAControl* btn,CCPoint point);
private:
    DSize size;
    bool isinitComplete;
};

#endif /* defined(__Test__MenuViewController__) */
