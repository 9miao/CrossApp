#ifndef _ListView_h_
#define _ListView_h_
#include "CrossApp.h"

USING_NS_CC;

class ListViewTest :public CAViewController,CAListViewDelegate,CAListViewDataSource
{
public:
	ListViewTest();
	virtual ~ListViewTest();

public:
	virtual void listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index);
	virtual void listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index);
	virtual unsigned int numberOfIndex(CAListView *listView);
	virtual unsigned int listViewHeightForIndex(CAListView *listView, unsigned int index);
	virtual CAListViewCell* listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index);

protected:
	void viewDidLoad();
	void viewDidUnload();

private:
	CADipSize size;
};

#endif
