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
    
public:

	static cocos2d::CAWindow* createWindow();
    
	FirstViewController();
    
	virtual ~FirstViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
};


#endif /* defined(__HelloCpp__ViewController__) */
