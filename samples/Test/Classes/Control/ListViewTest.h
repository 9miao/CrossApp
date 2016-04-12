
#ifndef __Test__ListViewTest__
#define __Test__ListViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CDListViewTest : public CAView ,CAListViewDelegate,CAListViewDataSource
{
public:
    CDListViewTest();
    virtual ~CDListViewTest();
    
    virtual bool init();
    static CDListViewTest* createWithLayout(const DLayout& layout);
    
public:
    virtual void listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index);
    virtual void listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index);
    virtual unsigned int numberOfIndex(CAListView *listView);
    virtual unsigned int listViewHeightForIndex(CAListView *listView, unsigned int index);
    virtual CAListViewCell* listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index);

public:
    std::vector<std::string> tempList;
};

class ListViewTest: public CAViewController, public CAListViewDelegate, public CAListViewDataSource
{
    
public:

	ListViewTest();
    
	virtual ~ListViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    DSize winSize;
    
    CAListView* p_ListView;
    
public:
    
    void listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index);
    void listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index);
    unsigned int numberOfIndex(CAListView *listView);
    unsigned int listViewHeightForIndex(CAListView *listView, unsigned int index);
    CAListViewCell* listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
