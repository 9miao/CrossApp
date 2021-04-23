
#include "AutoCollectionViewHorizontalTest.h"
#include "FirstViewController.h"


AutoCollectionViewHorizontalTest::AutoCollectionViewHorizontalTest()
{
    this->setTitle("CAAutoCollectionView");
    
    for (int i = 0; i < 60; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        
        m_vColors.push_back(CAColor4B(r, g, b, 255));
    }
}

AutoCollectionViewHorizontalTest::~AutoCollectionViewHorizontalTest()
{

}

void AutoCollectionViewHorizontalTest::viewDidLoad()
{
    m_pAutoCollection = CAAutoCollectionView::createWithLayout(DLayoutFill);
    m_pAutoCollection->setAllowsSelection(true);
    m_pAutoCollection->setAllowsMultipleSelection(true);
    m_pAutoCollection->setOrientation(CAAutoCollectionView::Orientation::Horizontal);
    m_pAutoCollection->setHoriCellInterval(20);
    m_pAutoCollection->setVertCellInterval(20);
    m_pAutoCollection->setReachBoundaryHandOverToSuperview(false);
    this->getView()->addSubview(m_pAutoCollection);
    
    m_pAutoCollection->onCellAtIndexPath(CALLBACK_BIND_3(AutoCollectionViewHorizontalTest::collectionCellAtIndex, this));
    m_pAutoCollection->onNumberOfItemsAtIndexPath(CALLBACK_BIND_1(AutoCollectionViewHorizontalTest::numberOfItemsInSection, this));
    m_pAutoCollection->onCellSizeAtIndexPath(CALLBACK_BIND_2(AutoCollectionViewHorizontalTest::collectionViewCellSizeAtIndexPath, this));
    
    m_pAutoCollection->onDidSelectCellAtIndexPath(CALLBACK_BIND_2(AutoCollectionViewHorizontalTest::collectionViewDidSelectCellAtIndexPath, this));
    
}

void AutoCollectionViewHorizontalTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

CACollectionViewCell* AutoCollectionViewHorizontalTest::collectionCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int item)
{
    CACollectionViewCell* cell = m_pAutoCollection->dequeueReusableCellWithIdentifier("CrossApp");

    if (cell == NULL)
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

DSize AutoCollectionViewHorizontalTest::collectionViewCellSizeAtIndexPath(unsigned int section, unsigned int item)
{
    return DSize(230, 230);
}

unsigned int AutoCollectionViewHorizontalTest::numberOfItemsInSection(unsigned int section)
{
    return (unsigned int)m_vColors.size();
}

void AutoCollectionViewHorizontalTest::collectionViewDidSelectCellAtIndexPath(unsigned int section, unsigned int item)
{
    CACollectionViewCell* cell = m_pAutoCollection->cellForRowAtIndexPath(section, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("");
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
}
