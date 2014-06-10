//
//  ViewController.h
//  HelloCpp
//
//  Created by liyuanfeng on 14-4-21.
//
//

#ifndef __HelloCpp__ViewController__
#define __HelloCpp__ViewController__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class FirstViewController: public CAViewController
{
    
    CALabel* label;
    
public:

	FirstViewController();
    
	virtual ~FirstViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void updateSlider(CAControl* sender, CCPoint point);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
