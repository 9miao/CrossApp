#ifndef _TableView_Test_h_
#define _TableView_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class TableViewTest :public CAViewController,public CATableViewDelegate ,public CATableViewDataSource
{
public:
	TableViewTest();
	virtual ~TableViewTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	void cellBtnCallback(CAControl* btn,CCPoint point);

public:
	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
	virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section);
	virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
	virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section);

private:
	CADipSize size;
	CATableView* p_TableView;

};

#endif