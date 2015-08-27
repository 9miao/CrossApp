//
//  CDTempViewController.hpp
//  Test
//
//  Created by renhongguang on 15/7/30.
//
//

#ifndef CDTempViewController_cpp
#define CDTempViewController_cpp

#include "RootWindow.h"

class CDTempViewController : public CAViewController
{
public:
    
    CDTempViewController();
    
    virtual ~CDTempViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void buttonCallBack(CAControl* btn,CCPoint point);
private:
    CADipSize size;
};

#endif /* CDTempViewController_cpp */
