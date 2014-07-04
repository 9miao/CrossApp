
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

	virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int row, unsigned int item);

	virtual unsigned int numberOfRowsInCollectionView(CACollectionView *collectionView);

	virtual unsigned int numberOfItemsInRows(CACollectionView *collectionView, unsigned int row);

	virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int row);
    
    void touchUpInSide();
    
    void touchUpInSide2();
    
   
    void setAllowsSelection(CAButton* btn, CCPoint point);

	CACollectionView* m_pCollectionView;
};

#endif /* defined(__project__ThirdViewController__) */
