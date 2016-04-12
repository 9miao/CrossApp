
#ifndef __Test__SliderTestFirst__
#define __Test__SliderTestFirst__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class SliderTestFirst: public CAViewController
{
    
public:
    
    SliderTestFirst();
    
    virtual ~SliderTestFirst();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CALabel* sliderValue;
    CASlider* slider;
    
public:
    
    void sliderValueChange(CAControl* btn, DPoint point);
    
};

#endif /* defined(__HelloCpp__ViewController__) */