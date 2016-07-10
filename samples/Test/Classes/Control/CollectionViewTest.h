
#ifndef __Test__CollectionViewTest__
#define __Test__CollectionViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CollectionViewTest: public CAViewController, CACollectionViewDelegate, CACollectionViewDataSource, public CAScrollViewDelegate
{
    
public:

	CollectionViewTest();
    
	virtual ~CollectionViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAPullToRefreshView* headerRefreshView;
    CAPullToRefreshView* footerRefreshView;
    
    CAPageView* p_PageViewVec;
    
    int showIndex;
    int showNum;
    int pageViewIndex;
    
    CAVector<CAView* > VIEWLIST;
    
public:
    
    void refreshData1(float interval);
    
    void refreshData2(float interval);
    
    void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
    virtual unsigned int numberOfSections(CACollectionView *collectionView);
    
    virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
    virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);
    
public:
    
    DSize size;
    
    CACollectionView* p_Conllection;
    
    std::vector<CAColor4B> colorArr;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
