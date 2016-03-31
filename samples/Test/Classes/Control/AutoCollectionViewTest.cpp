
#include "AutoCollectionViewTest.h"

AutoCollectionViewTest::AutoCollectionViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
    
    colorArr.clear();
}

AutoCollectionViewTest::~AutoCollectionViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void AutoCollectionViewTest::viewDidLoad()
{
    //随机出颜色
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        
        //将随机的ccc4对象放入到容器里
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    
    headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    
    p_AutoCollection = CAAutoCollectionView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    p_AutoCollection->setAllowsSelection(true);
    p_AutoCollection->setCollectionViewDelegate(this);
    p_AutoCollection->setCollectionViewDataSource(this);
    p_AutoCollection->setHeaderRefreshView(headerRefreshView);
    p_AutoCollection->setFooterRefreshView(footerRefreshView);
    p_AutoCollection->setScrollViewDelegate(this);
    p_AutoCollection->setHoriCellInterval(30);
    p_AutoCollection->setVertCellInterval(30);
    
    this->getView()->addSubview(p_AutoCollection);
    
    p_AutoCollection->reloadData();
    p_AutoCollection->startPullToHeaderRefreshView();
}

void AutoCollectionViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void AutoCollectionViewTest::refreshData1(float interval)
{
    colorArr.clear();
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    p_AutoCollection->reloadData();
}

void AutoCollectionViewTest::refreshData2(float interval)
{
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    p_AutoCollection->reloadData();
}

void AutoCollectionViewTest::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(AutoCollectionViewTest::refreshData1), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void AutoCollectionViewTest::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(AutoCollectionViewTest::refreshData2), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

//选中
void AutoCollectionViewTest::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    //选中
    CCLog("选中");
}

//取消选中
void AutoCollectionViewTest::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    //取消选中
    CCLog("取消选中");
}

//获取指定cell
CACollectionViewCell* AutoCollectionViewTest::collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    //根据标识获得CACollectionViewCell
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");

    //如果没有找到相应的CACollectionViewCell则新建一个
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        
        //生成Item背景
        CAView* itemImage = CAView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
        itemImage->setTag(99);
        p_Cell->addSubview(itemImage);
        
        //生成itemCALabel
        CALabel* itemText = CALabel::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        itemImage->addSubview(itemText);
    }

    //设置Item背景颜色
    CAView* itemImageView = p_Cell->getSubviewByTag(99);
    itemImageView->setColor(colorArr.at(item));
    CCLog("row = %d", item);
    
    //设置itme文本显示
    char pos[20] = "";
    sprintf(pos, "(%d,%d)", section, item);
    CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(99)->getSubviewByTag(100);
    itemText->setText(pos);
    return  p_Cell;
    
}

//项目大小
DSize AutoCollectionViewTest::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(240, 240);
}

//每个Section中Item的个数
unsigned int AutoCollectionViewTest::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)colorArr.size();
}

//section的个数
unsigned int AutoCollectionViewTest::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 1;
}
