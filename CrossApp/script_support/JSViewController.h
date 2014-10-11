//
//  JSViewController.h
//  JavaScriptTest
//
//  Created by lihui on 14-8-15.
//
//

#ifndef __JavaScriptTest__JSViewController__
#define __JavaScriptTest__JSViewController__

#include <iostream>
#include "controller/CAViewController.h"

class CC_DLL JSViewController:public CrossApp::CAViewController
{
public:
    
    JSViewController();
    
    virtual ~JSViewController();
    
public:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
};
#endif /* defined(__JavaScriptTest__JSViewController__) */
