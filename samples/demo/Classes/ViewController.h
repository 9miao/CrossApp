//
//  ViewController.h
//  project
//
//  Created by ff on 14-4-16.
//
//

#ifndef __project__ViewController__
#define __project__ViewController__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ViewController: public CAViewController, public CAScrollViewDelegate
{
    
    CAScrollView* scroll;
    
    CAImageView* v;
    
public:
    
    ViewController();
    
    virtual ~ViewController();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    void touchUpInSide(CAControl* control, CCPoint point);
};

#endif /* defined(__project__ViewController__) */
