#include "WaterfallViewTest.h"

WaterfallViewTest::WaterfallViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
    
    colorArr.clear();
}

WaterfallViewTest::~WaterfallViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void WaterfallViewTest::viewDidLoad()
{
    for (int i = 0; i < 12; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    
    headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    
    Waterfall = CAWaterfallView::createWithLayout(DLayoutFill);
    Waterfall->setItemMargin(10);
    Waterfall->setColumnMargin(10);
    Waterfall->setColumnCount(2);
    Waterfall->setAllowsSelection(true);
    Waterfall->setScrollViewDelegate(this);
    Waterfall->setWaterfallViewDelegate(this);
    Waterfall->setWaterfallViewDataSource(this);
    Waterfall->setHeaderRefreshView(headerRefreshView);
    Waterfall->setFooterRefreshView(footerRefreshView);
    this->getView()->addSubview(Waterfall);
}

void WaterfallViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void WaterfallViewTest::refreshData1(float interval)
{
    for (int i = 0; i < 12; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    Waterfall->reloadData();
}

void WaterfallViewTest::refreshData2(float interval)
{
    for (int i = 0; i < 12; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    Waterfall->reloadData();
}

void WaterfallViewTest::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(WaterfallViewTest::refreshData1), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2, false);
}

void WaterfallViewTest::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(WaterfallViewTest::refreshData2), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2, false);
}

void WaterfallViewTest::waterfallViewDidSelectCellAtIndexPath(CAWaterfallView *waterfallView, unsigned int itemIndex)
{
    CCLog("WaterfallViewTest%d", itemIndex);
}

void WaterfallViewTest::waterfallViewDidDeselectCellAtIndexPath(CAWaterfallView *waterfallView, unsigned int itemIndex)
{
    CCLog("CellAtIndexPath%d", itemIndex);
}

//Necessary
CAWaterfallViewCell* WaterfallViewTest::waterfallCellAtIndex(CAWaterfallView *waterfallView, const DSize& cellSize, unsigned int itemIndex)
{
    p_Cell = Waterfall->dequeueReusableCellWithIdentifier("CrossApp");
    if (p_Cell == NULL)
    {
        p_Cell = CAWaterfallViewCell::create("CrossApp");
        p_Cell->setAllowsSelected(false);
        p_Cell->setBackgroundView(CAView::create());
        
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(24);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        p_Cell->addSubview(itemText);
    }
    
    p_Cell->getBackgroundView()->setColor(colorArr.at(itemIndex));
    
    char pos[20] = "";
    sprintf(pos, "(%d)", itemIndex);
    
    CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(100);
    itemText->setText(pos);
    
    return  p_Cell;
    
}
//Necessary
unsigned int WaterfallViewTest::waterfallViewHeightForItemAtIndex(CAWaterfallView *waterfallView, unsigned int itemIndex)
{
    return CCRANDOM_0_1() * 300 + 300;
}
//Necessary
unsigned int WaterfallViewTest::numberOfItems(CAWaterfallView *waterfallView)
{
    return (unsigned int)colorArr.size();
}

CAView* WaterfallViewTest::waterfallViewSectionViewForHeader(CAWaterfallView *waterfallView, const DSize& viewSize)
{
    return NULL;
}

unsigned int WaterfallViewTest::waterfallViewHeightForHeader(CAWaterfallView *waterfallView)
{
    return 0;
}

CAView* WaterfallViewTest::waterfallViewSectionViewForFooter(CAWaterfallView *waterfallView, const DSize& viewSize)
{
    return NULL;
}

unsigned int WaterfallViewTest::waterfallViewHeightForFooter(CAWaterfallView *waterfallView)
{
    return 0;
}

void WaterfallViewTest::waterfallViewWillDisplayCellAtIndex(CAWaterfallView* waterfallView, CAWaterfallViewCell* cell, unsigned int itemIndex)
{
    
}