#ifndef _CollectionView_Test_h_
#define _CollectionView_Test_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CollectionViewTest :public CAViewController, CACollectionViewDelegate, CACollectionViewDataSource, CAScrollViewDelegate
{
public:
	CollectionViewTest();
	virtual ~CollectionViewTest();

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
};

#endif