#ifndef _CollectionView_Test_h_
#define _CollectionView_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CollectionViewTest :public CAViewController,CACollectionViewDelegate,CACollectionViewDataSource
{
public:
	CollectionViewTest();
	virtual ~CollectionViewTest();

protected:
	void viewDidLoad();
	void viewDidUnload();

public:
	virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int row, unsigned int item);
	virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int row, unsigned int item);

	virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
	virtual unsigned int numberOfSectionsInCollectioView();
	virtual unsigned int numberOfRowsInSectionCollectionView(CACollectionView *collectionView, unsigned int section);
	virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section);
	virtual unsigned int collectionViewHeightForFooterInSection(CACollectionView *collectionView, unsigned int section);
	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section);
	virtual CAView* collectionViewSectionViewForFooterInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section);
	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
	virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);

private:
	CCSize size;
	CACollectionView* p_Conllection;
	CAImageView* itemView;

};

#endif