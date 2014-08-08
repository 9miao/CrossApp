#ifndef _Extensions_Test_h_
#define _Extensions_Test_h_

#include <iostream>
#include "CrossApp.h"
#include "CrossAppExt.h"

USING_NS_CC_EXT;

using namespace CSJson;

typedef enum LABELTAG
{
	NAME=50,
	NUM,
	GENDER,
	OCCUPATION
};

class ExtensionsTest :public CAViewController, public CATableViewDelegate, public CATableViewDataSource
{
public:
	ExtensionsTest();
	virtual ~ExtensionsTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

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
	void loadJsonData();
	void nextViewController(CAControl* btn, CCPoint point);

private:
	CCSize size;
	CATableView* table;
	CCDictionary* personList;
	CAButton* next;

};

#endif