//
//  CheckboxTest.h
//  Test
//
//  Created by ly on 2017/3/1.
//
//

#ifndef CheckboxTest_h
#define CheckboxTest_h

#include "RootWindow.h"

class CheckboxTest: public CAViewController
{
    
public:
    
    CheckboxTest();
    
    virtual ~CheckboxTest();
    
    CREATE_FUNC(CheckboxTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
};

#endif /* CheckboxTest_h */
