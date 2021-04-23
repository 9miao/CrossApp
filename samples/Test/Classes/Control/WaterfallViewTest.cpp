#include "WaterfallViewTest.h"

WaterfallViewTest::WaterfallViewTest()
{
    this->setTitle("CAWaterfallView");
    
    for (int i = 0; i < 12; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        m_vColors.push_back(ccc4(r, g, b, 255));
        m_vHeights.push_back(CCRANDOM_0_1() * 300 + 300);
    }
}

WaterfallViewTest::~WaterfallViewTest()
{
}

void WaterfallViewTest::viewDidLoad()
{
    m_pWaterfallView = CAWaterfallView::createWithLayout(DLayoutFill);
    m_pWaterfallView->setItemMargin(10);
    m_pWaterfallView->setColumnMargin(10);
    m_pWaterfallView->setColumnCount(2);
    m_pWaterfallView->setAllowsSelection(true);
    m_pWaterfallView->setHeaderRefreshView(CAPullToRefreshView::create(CAPullToRefreshView::Type::Header));
    m_pWaterfallView->setFooterRefreshView(CAPullToRefreshView::create(CAPullToRefreshView::Type::Footer));
    this->getView()->addSubview(m_pWaterfallView);
    
    m_pWaterfallView->onCellAtIndexPath(CALLBACK_BIND_2(WaterfallViewTest::waterfallCellAtIndex, this));
    m_pWaterfallView->onCellHeightAtIndexPath(CALLBACK_BIND_1(WaterfallViewTest::waterfallViewHeightForItemAtIndex, this));
    m_pWaterfallView->onNumberOfItemsAtIndexPath(CALLBACK_BIND_0(WaterfallViewTest::numberOfItems, this));
    
    m_pWaterfallView->onDidSelectCellAtIndexPath(CALLBACK_BIND_1(WaterfallViewTest::waterfallViewDidSelectCellAtIndexPath, this));
    m_pWaterfallView->onDidDeselectCellAtIndexPath(CALLBACK_BIND_1(WaterfallViewTest::waterfallViewDidDeselectCellAtIndexPath, this));
    
    m_pWaterfallView->onHeaderBeginRefreshing(CALLBACK_BIND_0(WaterfallViewTest::scrollViewHeaderBeginRefreshing, this));
    m_pWaterfallView->onFooterBeginRefreshing(CALLBACK_BIND_0(WaterfallViewTest::scrollViewFooterBeginRefreshing, this));
}

void WaterfallViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void WaterfallViewTest::scrollViewHeaderBeginRefreshing()
{
    CAScheduler::getScheduler()->schedule([&](float dt){
        
        m_vColors.clear();
        m_vHeights.clear();
        for (int i = 0; i < 12; i++)
        {
            char r = CCRANDOM_0_1() * 255;
            char g = CCRANDOM_0_1() * 255;
            char b = CCRANDOM_0_1() * 255;
            m_vColors.push_back(CAColor4B(r, g, b, 255));
            m_vHeights.push_back(CCRANDOM_0_1() * 300 + 300);
        }
        m_pWaterfallView->reloadData();
    }, "scrollViewHeaderBeginRefreshing", this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void WaterfallViewTest::scrollViewFooterBeginRefreshing()
{
    CAScheduler::getScheduler()->schedule([&](float dt){
        
        for (int i = 0; i < 12; i++)
        {
            char r = CCRANDOM_0_1() * 255;
            char g = CCRANDOM_0_1() * 255;
            char b = CCRANDOM_0_1() * 255;
            m_vColors.push_back(CAColor4B(r, g, b, 255));
            m_vHeights.push_back(CCRANDOM_0_1() * 300 + 300);
        }
        m_pWaterfallView->reloadData();
    }, "scrollViewFooterBeginRefreshing", this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void WaterfallViewTest::waterfallViewDidSelectCellAtIndexPath(unsigned int itemIndex)
{
    //选中
    CAWaterfallViewCell* cell = m_pWaterfallView->cellForRowAtIndexPath(itemIndex);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("");
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
    CCLog("选中");
}

void WaterfallViewTest::waterfallViewDidDeselectCellAtIndexPath(unsigned int itemIndex)
{
    CCLog("取消选中");
}

//Necessary
CAWaterfallViewCell* WaterfallViewTest::waterfallCellAtIndex(const DSize& cellSize, unsigned int itemIndex)
{
    CAWaterfallViewCell* cell = m_pWaterfallView->dequeueReusableCellWithIdentifier("CrossApp");
    if (cell == nullptr)
    {
        cell = CAWaterfallViewCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(24);
        itemText->setTextAlignment(CATextAlignment::Center);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        cell->getContentView()->addSubview(itemText);
    }
    
    //设置Item背景颜色
    CAView* itemImageView = cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(m_vColors.at(itemIndex));
    
    char pos[20] = "";
    sprintf(pos, "(%d)", itemIndex);
    
    CALabel* itemText = (CALabel*)cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    
    return  cell;
    
}
//Necessary
unsigned int WaterfallViewTest::waterfallViewHeightForItemAtIndex(unsigned int itemIndex)
{
    return m_vHeights.at(itemIndex);
}
//Necessary
unsigned int WaterfallViewTest::numberOfItems()
{
    return (unsigned int)m_vColors.size();
}

