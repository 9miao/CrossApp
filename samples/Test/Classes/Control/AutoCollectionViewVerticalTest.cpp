
#include "AutoCollectionViewVerticalTest.h"
#include "FirstViewController.h"

AutoCollectionViewVerticalTest::AutoCollectionViewVerticalTest()
{
    this->setTitle("CAAutoCollectionView");
    
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        
        m_vColors.push_back(CAColor4B(r, g, b, 255));
    }
}

AutoCollectionViewVerticalTest::~AutoCollectionViewVerticalTest()
{

}

void AutoCollectionViewVerticalTest::viewDidLoad()
{
    m_pAutoCollection = CAAutoCollectionView::createWithLayout(DLayoutFill);
    m_pAutoCollection->setAllowsSelection(true);
    m_pAutoCollection->setAllowsMultipleSelection(true);
    m_pAutoCollection->setOrientation(CAAutoCollectionView::Orientation::Vertical);
    m_pAutoCollection->setHeaderRefreshView(CAPullToRefreshView::create(CAPullToRefreshView::Type::Header));
    m_pAutoCollection->setFooterRefreshView(CAPullToRefreshView::create(CAPullToRefreshView::Type::Footer));
    m_pAutoCollection->setHoriCellInterval(20);
    m_pAutoCollection->setVertCellInterval(20);
    this->getView()->addSubview(m_pAutoCollection);
    
    m_pAutoCollection->onCellAtIndexPath(CALLBACK_BIND_3(AutoCollectionViewVerticalTest::collectionCellAtIndex, this));
    m_pAutoCollection->onCellSizeAtIndexPath(CALLBACK_BIND_2(AutoCollectionViewVerticalTest::collectionViewCellSizeAtIndexPath, this));
    m_pAutoCollection->onNumberOfItemsAtIndexPath(CALLBACK_BIND_1(AutoCollectionViewVerticalTest::numberOfItemsInSection, this));
    
    m_pAutoCollection->onDidSelectCellAtIndexPath(CALLBACK_BIND_2(AutoCollectionViewVerticalTest::collectionViewDidSelectCellAtIndexPath, this));
    
    m_pAutoCollection->onHeaderBeginRefreshing(CALLBACK_BIND_0(AutoCollectionViewVerticalTest::scrollViewHeaderBeginRefreshing, this));
    m_pAutoCollection->onFooterBeginRefreshing(CALLBACK_BIND_0(AutoCollectionViewVerticalTest::scrollViewFooterBeginRefreshing, this));
    
}

void AutoCollectionViewVerticalTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void AutoCollectionViewVerticalTest::scrollViewHeaderBeginRefreshing()
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
        m_pAutoCollection->reloadData();
    }, "scrollViewHeaderBeginRefreshing", this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void AutoCollectionViewVerticalTest::scrollViewFooterBeginRefreshing()
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
        m_pAutoCollection->reloadData();
    }, "scrollViewFooterBeginRefreshing", this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void AutoCollectionViewVerticalTest::collectionViewDidSelectCellAtIndexPath(unsigned int section, unsigned int item)
{
    CACollectionViewCell* cell = m_pAutoCollection->cellForRowAtIndexPath(section, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("");
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
}

void AutoCollectionViewVerticalTest::collectionViewDidDeselectCellAtIndexPath(unsigned int section, unsigned int item)
{
}

CACollectionViewCell* AutoCollectionViewVerticalTest::collectionCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int item)
{
    CACollectionViewCell* cell = m_pAutoCollection->dequeueReusableCellWithIdentifier("CrossApp");
    
    if (cell == nullptr)
    {
        cell = CACollectionViewCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignment::Center);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        cell->getContentView()->addSubview(itemText);
    }
    
    CAView* itemImageView = cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(m_vColors.at(item));
    CCLog("row = %d", item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d)", section, item);
    CALabel* itemText = (CALabel*)cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    return  cell;
    
}

DSize AutoCollectionViewVerticalTest::collectionViewCellSizeAtIndexPath(unsigned int section, unsigned int item)
{
    return DSize(230, 230);
}

unsigned int AutoCollectionViewVerticalTest::numberOfItemsInSection(unsigned int section)
{
    return (unsigned int)m_vColors.size();
}


