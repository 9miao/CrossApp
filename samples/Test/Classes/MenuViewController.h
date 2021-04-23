//
//  MenuViewController.h
//  Test
//
//  Created by renhongguang on 15/4/3.
//
//

#ifndef __Test__MenuViewController__
#define __Test__MenuViewController__

#include "RootWindow.h"

class MenuViewController : public CAViewController
{
public:
    
    MenuViewController();
    
    virtual ~MenuViewController();
    
    CREATE_FUNC(MenuViewController);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void changeStatusBarOrientation(CAObject* obj);
    
public:
    void tableViewDidSelectRowAtIndexPath(unsigned int section, unsigned int row);
    
    CATableViewCell* tableCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int row);
    unsigned int numberOfRowsInSection(unsigned int section);
    unsigned int tableViewHeightForRowAtIndexPath(unsigned int section, unsigned int row);
private:

    CATableView* m_pTableView;
    
    CAImageView* m_pLogo;
};

#endif /* defined(__Test__MenuViewController__) */
