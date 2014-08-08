#include "CollectionViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

CollectionViewTest::CollectionViewTest()
{
	
}

CollectionViewTest::~CollectionViewTest()
{

}

void CollectionViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	p_Conllection = CACollectionView::createWithCenter(CCRect(size.width*0.5,size.height*0.5,size.width,size.height));
	p_Conllection->setAllowsSelection(true);
	p_Conllection->setCollectionViewDelegate(this);
	p_Conllection->setCollectionViewDataSource(this);
	this->getView()->addSubview(p_Conllection);
}

void CollectionViewTest::viewDidUnload()
{

}

void CollectionViewTest::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int row, unsigned int item)
{
	
}

void CollectionViewTest::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int row, unsigned int item)
{
	
}

CACollectionViewCell* CollectionViewTest::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
	CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
	if (p_Cell == NULL)
	{
		p_Cell = CACollectionViewCell::create("CrossApp");
		CAImageView* itemView = CAImageView::createWithImage(CAImage::create("logo.png"));
		itemView->setCenter(CCRect(cellSize.width*0.5,cellSize.height*0.5,cellSize.height * 0.90,cellSize.height * 0.90));
		itemView->setTag(99);
		p_Cell->addSubview(itemView);

		CALabel* itemText = CALabel::createWithCenter(CCRect(itemView->getBounds().size.width*0.5, 
															itemView->getBounds().size.height*0.5, 
															itemView->getBounds().size.width*0.6, 
															itemView->getBounds().size.width*0.5));
		itemText->setTag(100);
		itemText->setFontSize(29 * CROSSAPP_ADPTATION_RATIO);
		itemText->setTextAlignment(CATextAlignmentCenter);
		itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		itemView->addSubview(itemText);

	}
	char pos[20] = "";
	sprintf(pos, "(%d,%d,%d)",section, row, item);
	CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(99)->getSubviewByTag(100);
	itemText->setText(pos);

	return p_Cell;
}

unsigned int CollectionViewTest::numberOfSectionsInCollectioView()
{
	return 1;
}

unsigned int CollectionViewTest::numberOfRowsInSectionCollectionView(CACollectionView *collectionView, unsigned int section)
{
	return 20;
}

unsigned int CollectionViewTest::collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section)
{
	return 1;
}

unsigned int CollectionViewTest::collectionViewHeightForFooterInSection(CACollectionView *collectionView, unsigned int section)
{
	return 1;
}

CAView* CollectionViewTest::collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
{
	CAView* headerView = CAView::createWithColor(CAColor_gray);

	return headerView;
}

CAView* CollectionViewTest::collectionViewSectionViewForFooterInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
{
	CAView* footerView = CAView::createWithColor(CAColor_gray);

	return footerView;
}

unsigned int CollectionViewTest::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
	return 3;
}

unsigned int CollectionViewTest::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
	return size.height*0.25;
}
