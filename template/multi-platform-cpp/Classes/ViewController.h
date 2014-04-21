//
//  ViewController.h
//  HelloCpp
//
//  Created by 栗元峰 on 14-4-21.
//
//

#ifndef __HelloCpp__ViewController__
#define __HelloCpp__ViewController__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class ViewController: public CAViewController
{
    
public:

    static CAWindow* createWindow();
    
    ViewController();
    
    virtual ~ViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
};


#endif /* defined(__HelloCpp__ViewController__) */
