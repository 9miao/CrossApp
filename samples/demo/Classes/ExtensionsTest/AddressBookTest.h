#ifndef _Address_Book_Test_h_
#define _Address_Book_Test_h_

#include <iostream>
#include "CrossApp.h"
#include "CrossAppExt.h"

USING_NS_CC_EXT;

typedef enum
{
	NOTICE=200,
	FULLNAME,
	NUMBER,
	PROVINCE,
	NICKNAME
}ADDRESSBOOKLIST_TAG;

class AddressBookTest :public CAViewController, public CATableViewDelegate, public CATableViewDataSource
{
public:
	AddressBookTest();
	virtual ~AddressBookTest();

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
	void previousViewController(CAControl* btn, CCPoint point);
	void nextViewController(CAControl* btn, CCPoint point);
	void loadingView(void);
	void getAddressBookList(void);
	void addressBookLoadProgress(float interval);

private:
	CADipSize size;
	CATableView* table;
	std::vector<CAAddressBookRecord> addressBookList;
	CAImageView* loadIamge;
	int cout;
};

#endif