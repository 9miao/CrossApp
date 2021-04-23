
#ifndef __Test__ListViewTest__
#define __Test__ListViewTest__

#include "RootWindow.h"

class ListViewTest: public CAViewController
{
    
public:

	ListViewTest();
    
	virtual ~ListViewTest();
    
    CREATE_FUNC(ListViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

public:
    
    void didSelectCellAtIndex(unsigned int index);
    void didDeselectCellAtIndex(unsigned int index);
    unsigned int numberOfIndex();
    unsigned int heightForIndex(unsigned int index);
    CAListViewCell* cellAtIndex(const DSize& cellSize, unsigned int index);
    
    CAListView* m_pListViewHor;
    CAListView* m_pListViewVer;
    
    std::set<unsigned int> m_sDidSelectCellAtIndexs;
};


#endif /* defined(__HelloCpp__ViewController__) */
