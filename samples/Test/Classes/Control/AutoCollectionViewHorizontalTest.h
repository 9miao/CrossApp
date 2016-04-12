
#ifndef __Test__AutoCollectionViewHorizontalTest__
#define __Test__AutoCollectionViewHorizontalTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class AutoCollectionViewHorizontalTest: public CAViewController, public CAScrollViewDelegate, CAAutoCollectionViewDataSource,CAAutoCollectionViewDelegate
{
    
public:
    
    AutoCollectionViewHorizontalTest();
    
    virtual ~AutoCollectionViewHorizontalTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAAutoCollectionView* p_AutoCollection;
    
    std::vector<CAColor4B> colorArr;
    
public:
    
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item);

    virtual DSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);

    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);

    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);

};


#endif /* defined(__HelloCpp__ViewController__) */
