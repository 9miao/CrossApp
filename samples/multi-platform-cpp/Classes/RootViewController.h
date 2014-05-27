//
//  RootViewController.h
//  project
//
//  Created by ff on 14-4-11.
//
//

#ifndef __project__RootViewController__
#define __project__RootViewController__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class RootViewController: public CAViewController, public CATableViewDataSource, public CATableViewDelegate
{
    
    CATableView* tableView;
    
    CAProgress* progress;
    
public:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual CATableViewCell* tableCellAtIndex(CATableView *table, unsigned int section, unsigned int row);
    
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, unsigned int section);
    
    virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, unsigned int section);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int numberOfSections(CATableView *table);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    
    virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidShowPullDownView(CATableView* table);
    
    virtual void tableViewDidShowPullUpView(CATableView* table);
    
    bool setAllowsSelection(CAButton* btn, CCPoint point);
    
    bool setAllowsMultipleSelection(CAButton* btn, CCPoint point);
    
    void updateProgress(float dt);
};




#endif /* defined(__project__RootViewController__) */
