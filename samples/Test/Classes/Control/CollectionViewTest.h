
#ifndef __Test__CollectionViewTest__
#define __Test__CollectionViewTest__

#include <iostream>
#include "RootWindow.h"

class CollectionViewTest: public CAViewController
{
    
public:

	CollectionViewTest();
    
	virtual ~CollectionViewTest();
    
    CREATE_FUNC(CollectionViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void scrollViewHeaderBeginRefreshing();
    
    void scrollViewFooterBeginRefreshing();
    
    virtual void collectionViewDidSelectCellAtIndexPath(unsigned int section, unsigned int row, unsigned int item);
        
    virtual CACollectionViewCell* collectionCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
    virtual unsigned int numberOfRowsInSection(unsigned int section);
    
    virtual unsigned int numberOfItemsInRowsInSection(unsigned int section, unsigned int row);
    
    virtual unsigned int collectionViewHeightForRowAtIndexPath(unsigned int section, unsigned int row);
    
private:
    
    CACollectionView* m_pCollection;
    
    std::vector<CAColor4B> m_vColors;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
