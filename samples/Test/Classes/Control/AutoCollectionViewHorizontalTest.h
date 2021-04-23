
#ifndef __Test__AutoCollectionViewHorizontalTest__
#define __Test__AutoCollectionViewHorizontalTest__

#include <iostream>
#include "RootWindow.h"

class AutoCollectionViewHorizontalTest: public CAViewController
{
    
public:
    
    AutoCollectionViewHorizontalTest();
    
    virtual ~AutoCollectionViewHorizontalTest();
    
    CREATE_FUNC(AutoCollectionViewHorizontalTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual void collectionViewDidSelectCellAtIndexPath(unsigned int section, unsigned int item);
        
    virtual CACollectionViewCell* collectionCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int item);

    virtual DSize collectionViewCellSizeAtIndexPath(unsigned int section, unsigned int item);

    virtual unsigned int numberOfItemsInSection(unsigned int section);

private:
    
    CAAutoCollectionView* m_pAutoCollection;
    
    std::vector<CAColor4B> m_vColors;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
