
#include "CollectionViewTest.h"

CollectionViewTest::CollectionViewTest()
{
    this->setTitle("CACollectionView");
    
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        m_vColors.push_back(CAColor4B(r, g, b, 255));
    }
}

CollectionViewTest::~CollectionViewTest()
{

}

void CollectionViewTest::viewDidLoad()
{
    m_pCollection = CACollectionView::createWithLayout(DLayoutFill);
    m_pCollection->setAllowsSelection(true);
    m_pCollection->setHeaderRefreshView(CAPullToRefreshView::create(CAPullToRefreshView::Type::Header));
    m_pCollection->setFooterRefreshView(CAPullToRefreshView::create(CAPullToRefreshView::Type::Footer));
    m_pCollection->setHoriInterval(20);
    m_pCollection->setVertInterval(20);
    this->getView()->addSubview(m_pCollection);
    
    m_pCollection->onCellAtIndexPath(CALLBACK_BIND_4(CollectionViewTest::collectionCellAtIndex, this));
    m_pCollection->onCellHeightAtIndexPath(CALLBACK_BIND_2(CollectionViewTest::collectionViewHeightForRowAtIndexPath, this));
    m_pCollection->onNumberOfRowsAtIndexPath(CALLBACK_BIND_1(CollectionViewTest::numberOfRowsInSection, this));
    m_pCollection->onNumberOfItemsAtIndexPath(CALLBACK_BIND_2(CollectionViewTest::numberOfItemsInRowsInSection, this));
    
    m_pCollection->onDidSelectCellAtIndexPath(CALLBACK_BIND_3(CollectionViewTest::collectionViewDidSelectCellAtIndexPath, this));
    
    m_pCollection->onHeaderBeginRefreshing(CALLBACK_BIND_0(CollectionViewTest::scrollViewHeaderBeginRefreshing, this));
    m_pCollection->onFooterBeginRefreshing(CALLBACK_BIND_0(CollectionViewTest::scrollViewFooterBeginRefreshing, this));
    
    m_pCollection->reloadData();
    m_pCollection->startPullToHeaderRefreshView();
    
}

void CollectionViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void CollectionViewTest::scrollViewHeaderBeginRefreshing()
{
    CAScheduler::getScheduler()->schedule([&](float dt)
    {
        m_vColors.clear();
        for (int i = 0; i < 40; i++)
        {
            char r = CCRANDOM_0_1() * 255;
            char g = CCRANDOM_0_1() * 255;
            char b = CCRANDOM_0_1() * 255;
            m_vColors.push_back(CAColor4B(r, g, b, 255));
        }
        m_pCollection->reloadData();
    }, "scrollViewHeaderBeginRefreshing", this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void CollectionViewTest::scrollViewFooterBeginRefreshing()
{
    CAScheduler::getScheduler()->schedule([&](float dt)
    {
        for (int i = 0; i < 40; i++)
        {
            char r = CCRANDOM_0_1() * 255;
            char g = CCRANDOM_0_1() * 255;
            char b = CCRANDOM_0_1() * 255;
            m_vColors.push_back(CAColor4B(r, g, b, 255));
        }
        m_pCollection->reloadData();
   }, "scrollViewFooterBeginRefreshing", this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}


void CollectionViewTest::collectionViewDidSelectCellAtIndexPath(unsigned int section, unsigned int row, unsigned int item)
{
    CACollectionViewCell* cell = m_pCollection->cellForRowAtIndexPath(section, row, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("");
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
}

CACollectionViewCell* CollectionViewTest::collectionCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    if (row * 3 + item >= m_vColors.size())
    {
        return nullptr;
    }
    
    CACollectionViewCell* cell = m_pCollection->dequeueReusableCellWithIdentifier("CrossApp");
    if (cell == nullptr)
    {
        cell = CACollectionViewCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(50, 50)));
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignment::Center);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        cell->getContentView()->addSubview(itemText);
    }
    
    CAView* itemImageView = cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(m_vColors.at(row * 3 + item));
    CCLog("row = %d", item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d,%d)", section, row, item);
    CALabel* itemText = (CALabel*)cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    
    return cell;
}

unsigned int CollectionViewTest::numberOfRowsInSection(unsigned int section)
{
    return (unsigned int)(m_vColors.size() % 3 == 0 ? m_vColors.size() / 3 : m_vColors.size() / 3 + 1);
}

unsigned int CollectionViewTest::numberOfItemsInRowsInSection(unsigned int section, unsigned int row)
{
    return 3;
}

unsigned int CollectionViewTest::collectionViewHeightForRowAtIndexPath(unsigned int section, unsigned int row)
{
    return (this->getView()->getBounds().size.width - 20 * 4) / 3;
}
