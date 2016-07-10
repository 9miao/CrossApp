
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
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    
    headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    
    p_Conllection = CACollectionView::createWithLayout(DLayoutFill);
    p_Conllection->setAllowsSelection(true);
//    p_Conllection->setAllowsMultipleSelection(true);
    p_Conllection->setCollectionViewDelegate(this);
    p_Conllection->setCollectionViewDataSource(this);
    p_Conllection->setScrollViewDelegate(this);
    p_Conllection->setHeaderRefreshView(headerRefreshView);
    p_Conllection->setFooterRefreshView(footerRefreshView);
    p_Conllection->setHoriInterval(20);
    p_Conllection->setVertInterval(20);
    this->getView()->addSubview(p_Conllection);
    
    p_Conllection->reloadData();
    p_Conllection->startPullToHeaderRefreshView();
    
}

void CollectionViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void CollectionViewTest::refreshData1(float interval)
{
    colorArr.clear();
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    p_Conllection->reloadData();
}

void CollectionViewTest::refreshData2(float interval)
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

void CollectionViewTest::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(CollectionViewTest::refreshData1), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void CollectionViewTest::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(CollectionViewTest::refreshData2), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}


void CollectionViewTest::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    CACollectionViewCell* cell = collectionView->cellForRowAtIndexPath(section, row, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("", NULL);
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
    
    p_Conllection->startPullToHeaderRefreshView();
}

void CollectionViewTest::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{

}

CACollectionViewCell* CollectionViewTest::collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    if (row * 3 + item >= colorArr.size())
    {
        return NULL;
    }
    
    DSize _size = cellSize;
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        p_Cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(50, 50)));
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        p_Cell->getContentView()->addSubview(itemText);
    }
    
    CAView* itemImageView = p_Cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(colorArr.at(row * 3 + item));
    CCLog("row = %d", item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d,%d)", section, row, item);
    CALabel* itemText = (CALabel*)p_Cell->getContentView()->getSubviewByTag(100);
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
    return (this->getView()->getBounds().size.width - 20 * 4) / 3;
}
