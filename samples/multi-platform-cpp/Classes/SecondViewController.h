//
//  SecondViewController.h
//  HelloCpp
//
//  Created by MAC on 14-6-14.
//
//

#ifndef __HelloCpp__SecondViewController__
#define __HelloCpp__SecondViewController__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class SecondViewController: public CAViewController
{
    
    CAImageView* imageView;
    
public:
    
	SecondViewController();
    
	virtual ~SecondViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void touchUpInSide(CAControl* sender, CCPoint point);
};
#endif /* defined(__HelloCpp__SecondViewController__) */
