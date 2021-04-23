
#ifndef __Test__TableViewTest__
#define __Test__TableViewTest__

#include "RootWindow.h"

class TableViewTest: public CAViewController
{
    
public:

	TableViewTest();
    
	virtual ~TableViewTest();
    
    CREATE_FUNC(TableViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    //TableView
    void tableViewDidSelectRowAtIndexPath(unsigned int section, unsigned int row);
    void tableViewDidDeselectRowAtIndexPath(unsigned int section, unsigned int row);
    
    CATableViewCell* tableCellAtIndex(DSize cellSize, unsigned int section, unsigned int row);
    CAView* tableViewSectionViewForHeaderInSection(DSize viewSize, unsigned int section);
    CAView* tableViewSectionViewForFooterInSection(DSize viewSize, unsigned int section);
    unsigned int numberOfRowsInSection(unsigned int section);
    unsigned int numberOfSections();
    unsigned int tableViewHeightForRowAtIndexPath(unsigned int section, unsigned int row);
    unsigned int tableViewHeightForHeaderInSection(unsigned int section);
    unsigned int tableViewHeightForFooterInSection(unsigned int section);
    
private:

    CATableView* m_pTableView;
    
    std::vector<std::string> m_pSectionTitle;
    
};




#endif /* defined(__HelloCpp__ViewController__) */
