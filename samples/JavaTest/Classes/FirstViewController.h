
#ifndef __HelloCpp__ViewController__
#define __HelloCpp__ViewController__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class FirstViewController: public CAViewController,public CATableViewDataSource
{
    
public:

	FirstViewController();
    
	virtual ~FirstViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
};


#endif /* defined(__HelloCpp__ViewController__) */
