//
//  PageControlTest.h
//  Test
//
//  Created by ly on 2017/3/2.
//
//

#ifndef PageControlTest_h
#define PageControlTest_h

#include "RootWindow.h"

class PageControlTest: public CAViewController
{
    
public:
    
    PageControlTest();
    
    virtual ~PageControlTest();
    
    CREATE_FUNC(PageControlTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
};

#endif /* PageControlTest_h */
