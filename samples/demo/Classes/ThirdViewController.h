
#ifndef __project__ThirdViewController__
#define __project__ThirdViewController__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ThirdViewController
	: public CAViewController
	, public CAScrollViewDelegate
	, public CACollectionViewDelegate
	, public CACollectionViewDataSource
{
    
public:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();


	virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);

	virtual unsigned int numberOfSectionsInCollectioView();

	virtual unsigned int numberOfRowsInSectionCollectionView(CACollectionView *collectionView, unsigned int section);

	virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section);

	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section);

	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);

	virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);

    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
	virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    
    void touchUpInSide();
    
    void touchUpInSide2();
    
   
    void setAllowsSelection(CAButton* btn, CCPoint point);

	CACollectionView* m_pCollectionView;
};

#endif /* defined(__project__ThirdViewController__) */
