//
//  CDTempViewController.cpp
//  Test
//
//  Created by renhongguang on 15/7/30.
//
//

#include "CDTempViewController.h"

CDTempViewController::CDTempViewController()
{
    
}

CDTempViewController::~CDTempViewController()
{
    
}

void CDTempViewController::viewDidLoad()
{
    size = this->getView()->getBounds().size;
    this->getView()->setColor(CAColor_clear);
}

void CDTempViewController::viewDidUnload()
{
    
}