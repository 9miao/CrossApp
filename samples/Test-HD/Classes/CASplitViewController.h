//
//  CASplitViewController.hpp
//  HelloCpp
//
//  Created by 栗元峰 on 15/7/23.
//
//

#ifndef CASplitViewController_c
#define CASplitViewController_c

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CASplitViewController: public CAViewController
{
    
public:
    
    CASplitViewController();
    
    virtual ~CASplitViewController();
    
    virtual bool initWithController(const CAVector<CAViewController*>& vec, const std::vector<unsigned int>& widths);

    CAViewController* getViewControllerAtIndex(int index);
    void resetViewControllerAtIndex(const vector<unsigned int>& widths);
    
    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);

protected:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    virtual void reshapeViewRectDidFinish();
    
    CAVector<CAView*> m_pContainers;
    
    CAVector<CAViewController*> m_pViewControllers;
    
    std::vector<unsigned int> m_iWidths;
};

#endif /* CASplitViewController_cpp */
