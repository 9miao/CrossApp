
#ifndef __Test__AutoCollectionViewVerticalTest__
#define __Test__AutoCollectionViewVerticalTest__

#include <iostream>
#include "RootWindow.h"

class AutoCollectionViewVerticalTest: public CAViewController
{
    
public:
    
    AutoCollectionViewVerticalTest();
    
    virtual ~AutoCollectionViewVerticalTest();
    
    CREATE_FUNC(AutoCollectionViewVerticalTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void scrollViewHeaderBeginRefreshing();
    
    void scrollViewFooterBeginRefreshing();
    
    virtual void collectionViewDidSelectCellAtIndexPath(unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int item);
    
    virtual DSize collectionViewCellSizeAtIndexPath(unsigned int section, unsigned int item);
    
    virtual unsigned int numberOfItemsInSection(unsigned int section);
    
private:
    
    CAAutoCollectionView* m_pAutoCollection;
    
    std::vector<CAColor4B> m_vColors;
};


#endif /* defined(__HelloCpp__ViewController__) */
