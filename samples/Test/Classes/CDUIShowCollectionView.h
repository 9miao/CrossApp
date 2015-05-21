//
//  CDUIShowCollectionView.h
//  Test
//
//  Created by renhongguang on 15/4/7.
//
//

#ifndef __Test__CDUIShowCollectionView__
#define __Test__CDUIShowCollectionView__

#include "CrossApp.h"

class CDUIShowCollectionView : public CAViewController,
public CACollectionViewDelegate,
public CACollectionViewDataSource,
public CAScrollViewDelegate
{
public:
    
    CDUIShowCollectionView();
    
    virtual ~CDUIShowCollectionView();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
    virtual unsigned int numberOfSections(CACollectionView *collectionView);
    
    virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
    virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
public:
    
    void refreshData(float interval);
    
private:
    
    CADipSize size;
    CACollectionView* p_Conllection;
    CAPullToRefreshView* headerRefreshView;
    CAPullToRefreshView* footerRefreshView;
    std::vector<CAColor4B> colorArr;
    std::vector<std::string> UINAME;
};
#endif /* defined(__Test__CDUIShowCollectionView__) */
