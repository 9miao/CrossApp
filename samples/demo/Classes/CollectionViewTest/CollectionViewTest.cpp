#include "CollectionViewTest.h"

CollectionViewTest::CollectionViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

CollectionViewTest::~CollectionViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void CollectionViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	for (int i = 0; i < 40; i++)
	{
		char r = CCRANDOM_0_1() * 255;
		char g = CCRANDOM_0_1() * 255;
		char b = CCRANDOM_0_1() * 255;
		colorArr.push_back(ccc4(r, g, b, 255));
	}

	headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
	footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);

	p_Conllection = CACollectionView::createWithFrame(this->getView()->getBounds());
	p_Conllection->setAllowsSelection(true);
	//p_Conllection->setAllowsMultipleSelection(true);
	p_Conllection->setCollectionViewDelegate(this);
	p_Conllection->setCollectionViewDataSource(this);
	p_Conllection->setScrollViewDelegate(this);
	p_Conllection->setHeaderRefreshView(headerRefreshView);
	p_Conllection->setFooterRefreshView(footerRefreshView);
	p_Conllection->setHoriInterval(_px(40));
	p_Conllection->setVertInterval(_px(40));
	this->getView()->addSubview(p_Conllection);
}

void CollectionViewTest::viewDidUnload()
{

}

void CollectionViewTest::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{

}

void CollectionViewTest::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{

}

CACollectionViewCell* CollectionViewTest::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
	if (row * 3 + item >= colorArr.size())
	{
		return NULL;
	}

	CADipSize _size = cellSize;
	CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
	if (p_Cell == NULL)
	{
		p_Cell = CACollectionViewCell::create("CrossApp");

		CAView* itemImage = CAView::createWithFrame(CADipRect(0, 0, _size.width, _size.height));
		itemImage->setTag(99);
		p_Cell->addSubview(itemImage);

		CADipSize itemSize = itemImage->getBounds().size;
		CALabel* itemText = CALabel::createWithCenter(CADipRect(itemSize.width*0.5, itemSize.height*0.5, 150, 40));
		itemText->setTag(100);
		itemText->setFontSize(_px(29));
		itemText->setTextAlignment(CATextAlignmentCenter);
		itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		itemImage->addSubview(itemText);
	}
	CAView* itemImageView = p_Cell->getSubviewByTag(99);
	itemImageView->setColor(colorArr.at(row * 3 + item));
	CCLog("%d", row * 3 + item);

	char pos[20] = "";
	sprintf(pos, "(%d,%d,%d)", section, row, item);
	CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(99)->getSubviewByTag(100);
	itemText->setText(pos);

	return p_Cell;
}

unsigned int CollectionViewTest::numberOfSections(CACollectionView *collectionView)
{
	return 1;
}

unsigned int CollectionViewTest::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
	return (unsigned int)(colorArr.size() % 3 == 0 ? colorArr.size() / 3 : colorArr.size() / 3 + 1);
}

unsigned int CollectionViewTest::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
	return 3;
}

unsigned int CollectionViewTest::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
	return (this->getView()->getBounds().size.width - _px(40) * 4) / 3;
}

void CollectionViewTest::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
	colorArr.clear();
	CAScheduler::schedule(schedule_selector(CollectionViewTest::refreshData), this, 0.1, 0, CCRANDOM_0_1() * 2, false);
}

void CollectionViewTest::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
	CAScheduler::schedule(schedule_selector(CollectionViewTest::refreshData), this, 0.1, 0, CCRANDOM_0_1() * 2, false);
}

void CollectionViewTest::refreshData(float interval)
{
	for (int i = 0; i < 40; i++)
	{
		char r = CCRANDOM_0_1() * 255;
		char g = CCRANDOM_0_1() * 255;
		char b = CCRANDOM_0_1() * 255;
		colorArr.push_back(ccc4(r, g, b, 255));
	}
	p_Conllection->reloadData();
}