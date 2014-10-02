#ifndef _Second_ViewController_h_
#define _Second_ViewController_h_

#include <iostream>
#include "CrossApp.h"
#include "CrossAppExt.h"
#include "Info.h"

USING_NS_CC_EXT;

using namespace CSJson;
#define NUM 8
class SecondViewController : public CAViewController, CATableViewDelegate, CATableViewDataSource,CAScrollViewDelegate
{   
public:
	SecondViewController();
	virtual ~SecondViewController();
    
protected:
    void viewDidLoad();
    void viewDidUnload();
	void loadJsonData(void);

public:
	virtual void reshapeViewRectDidFinish();

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

	virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
	virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);

public:
	void refreshTableViewData(float interval);
	void switchCellListInSection(CAControl* btn,CCPoint point);
	void closeCellListInSection(CAControl* btn, CCPoint point);

private:
	CADipSize size;
	CATableView* p_TableView;
	int sect[NUM];
	CADeque<Info*> personList;
	bool isPullUpRefresh;
};
#endif 
