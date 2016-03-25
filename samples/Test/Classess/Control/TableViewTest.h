
#ifndef __Test__TableViewTest__
#define __Test__TableViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CDETableViewDelegate
{
public:
    
    virtual ~CDETableViewDelegate(){};
    
    virtual void etableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row){};
    
    virtual void etableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row){};
};

class ETableViewCell : public CATableViewCell
{
public:
    ETableViewCell();
    virtual ~ETableViewCell();
    static ETableViewCell* create(const std::string& identifier);
    virtual void highlightedTableViewCell();
    virtual void selectedTableViewCell();
public:
    void initWithCell();
};

class ETableView : public CAView,CATableViewDelegate, CATableViewDataSource
{
public:
    ETableView();
    virtual ~ETableView();
    
    virtual bool init(const CrossApp::DRect &rect);
    static ETableView* createWithLayout(const DRectLayout& layout);
    
public:
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section);
    
    CC_SYNTHESIZE(CDETableViewDelegate*, m_pETableViewDelegate, ETableViewDelegate);
public:
    CATableView* p_TableView;
    DSize size;
};

class TableViewTest: public CAViewController, public CATableViewDataSource, public CDETableViewDelegate, public CATableViewDelegate
{
    
public:

	TableViewTest();
    
	virtual ~TableViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAPageView* p_PageViewVec;
    
    int showIndex;
    int showNum;
    int pageViewIndex;
    
    CAPageControl* pageControl;
    CAVector<CAView* > VIEWLIST;
    
    CATableView* p_TableView;
    
public:
    
    //TableView
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section);
    virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, const DSize& viewSize, unsigned int section);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section);
    
    virtual void etableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    void pageControlCallBack(CrossApp::CAControl *btn, CrossApp::DPoint point);
    
};




#endif /* defined(__HelloCpp__ViewController__) */
