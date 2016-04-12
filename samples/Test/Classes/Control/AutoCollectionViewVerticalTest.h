
#ifndef __Test__AutoCollectionViewVerticalTest__
#define __Test__AutoCollectionViewVerticalTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class AutoCollectionViewVerticalTest: public CAViewController, public CAScrollViewDelegate, CAAutoCollectionViewDataSource,CAAutoCollectionViewDelegate
{
    
public:
    
    AutoCollectionViewVerticalTest();
    
    virtual ~AutoCollectionViewVerticalTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAAutoCollectionView* p_AutoCollection;
    
    std::vector<CAColor4B> colorArr;
    
    CAPullToRefreshView* headerRefreshView;
    CAPullToRefreshView* footerRefreshView;
    
public:
    
    void refreshData1(float interval);
    
    void refreshData2(float interval);
    
    void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item);
    
    virtual DSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);
    
    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);
    
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
