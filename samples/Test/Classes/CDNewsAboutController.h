//
//  CDNewsAboutController.h
//  Test
//
//  Created by renhongguang on 15/4/17.
//
//

#ifndef __Test__CDNewsAboutController__
#define __Test__CDNewsAboutController__

#include "RootWindow.h"

class CDNewsAboutTableCell : public CATableViewCell
{
public:
    CDNewsAboutTableCell();
    virtual ~CDNewsAboutTableCell();
    static CDNewsAboutTableCell* create(const std::string& identifier, const CCRect& _rect = CADipRectZero);
    virtual void highlightedTableViewCell();
    virtual void selectedTableViewCell();
public:
    void initWithCell();
};

class CDNewsAboutController : public CAViewController,CATableViewDelegate,CATableViewDataSource
{
public:
    CDNewsAboutController();
    virtual ~CDNewsAboutController();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    CREATE_FUNC(CDNewsAboutController);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section);
    virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section);
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section);
public:
    CCSize winSize;
    CATableView* p_TableView;
};

#endif /* defined(__Test__CDNewsAboutController__) */
