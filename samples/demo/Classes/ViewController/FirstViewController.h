#ifndef _First_ViewController_h_
#define _First_ViewController_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

enum DemoList
{
	AlERTVIEWTEST=0,
	BUTTONTEST,
	COLLECTIONVIEWTEST,
	IMAGEVIEWTEST,
	LABELTEST,
	NAVIGATIONBARTEST,
	PROGRESSTEST,
	SCROLLVIEWTEST,
	SEGMENTEDCONTROLTEST,
	SLIDERTEST,
	SWITCHTEST,
	TABBARTEST,
	TABVIEWTEST,
	TEXTFIELDTEST,
	EXTENSIONSTEST
};

class FirstViewController: public CAViewController,public CATableViewDelegate,CATableViewDataSource
{  
public:
	FirstViewController();
	virtual ~FirstViewController();
    
protected:
    void viewDidLoad();
    void viewDidUnload();

public:
	virtual void reshapeViewRectDidFinish();

	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidShowPullDownView(CATableView* table);
	virtual void tableViewDidShowPullUpView(CATableView* table);

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
	virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, unsigned int section);
	virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, unsigned int section);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
	virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section);

private:
	CCSize size;
	CATableView* tableView;
	std::string testList[15];
};


#endif
