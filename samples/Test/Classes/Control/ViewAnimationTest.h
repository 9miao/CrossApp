
#ifndef __Test__ViewAnimationTest__
#define __Test__ViewAnimationTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ViewAnimationTest: public CAViewController
{
    
public:

	ViewAnimationTest();
    
	virtual ~ViewAnimationTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    int heart_index;
    
    DSize winSize;
    DRect winRect;
    
    //Animation
    CAImageView* animation_1_view;
    CATextField* animation_2_textfield;
    CAButton* animation_2_btn_search;
    CAButton* animation_2_btn_cancel;
    CAImageView* animation_3_imageview;
    
public:
    
    void endAction();
    
    void doAction(CAControl* btn,DPoint point);

};


#endif /* defined(__HelloCpp__ViewController__) */
