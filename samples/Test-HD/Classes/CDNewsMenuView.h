//
//  CDNewsMenuView.hpp
//  Test-HD
//
//  Created by renhongguang on 15/8/7.
//
//

#ifndef CDNewsMenuView_cpp
#define CDNewsMenuView_cpp

#include "CrossApp.h"
#include "RootWindow.h"

class CDNewsMenuTableCell : public CATableViewCell
{
public:
    CDNewsMenuTableCell(int row);
    virtual ~CDNewsMenuTableCell();
    static CDNewsMenuTableCell* create(const std::string& identifier, const DRect& _rect = DRectZero,const int row = 1);
    virtual void highlightedTableViewCell();
    virtual void selectedTableViewCell();
    virtual void normalTableViewCell();
    void setImageID(int _id);
public:
    int imageid;
};

class CDNewsMenuView : public CAViewController, CATableViewDelegate,CATableViewDataSource
{
public:
    
    CDNewsMenuView();
    
    virtual ~CDNewsMenuView();
    
    CREATE_FUNC(CDNewsMenuView);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void btn_callBack(CAControl* btn,CCPoint point);
    
public:
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
private:
    DSize size;
    CATableView* tableView;
};
#endif /* CDNewsMenuView_cpp */
