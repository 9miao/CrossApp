
#include "AutoCollectionViewHorizontalTest.h"
#include "CDUIShowAutoCollectionView.h"

std::vector<CAColor4B> HorizontalcolorArr;

ECollectionViewHorizontalCell::ECollectionViewHorizontalCell()
{
    this->setAllowsSelected(false);
}

ECollectionViewHorizontalCell::~ECollectionViewHorizontalCell()
{
    
}

ECollectionViewHorizontalCell* ECollectionViewHorizontalCell::create(const std::string& reuseIdentifier)
{
    ECollectionViewHorizontalCell* cell = new ECollectionViewHorizontalCell();
    if (cell && cell->initWithReuseIdentifier(reuseIdentifier))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

void ECollectionViewHorizontalCell::initWithCell()
{
    CALabel* test = CALabel::createWithLayout(DLayoutFill);
    test->setColor(ccc4(34, 151, 254, 255));
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(28);
    test->setTag(100);
    this->addSubview(test);
}

void ECollectionViewHorizontalCell::highlightedCollectionViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_clear));
}

void ECollectionViewHorizontalCell::selectedCollectionViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_clear));
}

EAutoCollectionViewHorizontal::EAutoCollectionViewHorizontal()
{
    
}

EAutoCollectionViewHorizontal::~EAutoCollectionViewHorizontal()
{
    
}

bool EAutoCollectionViewHorizontal::init()
{
    p_AutoCollection = CAAutoCollectionView::createWithLayout(DLayoutFill);
    p_AutoCollection->setAllowsSelection(true);
    p_AutoCollection->setAllowsMultipleSelection(true);
    p_AutoCollection->setCollectionViewDelegate(this);
    p_AutoCollection->setCollectionViewDataSource(this);
    p_AutoCollection->setOrientation(CAAutoCollectionView::Horizontal);
    p_AutoCollection->setCellHoriAlign(CAAutoCollectionView::HoriAlignCenter);
    p_AutoCollection->setHoriCellInterval(20);
    p_AutoCollection->setVertCellInterval(20);
    this->addSubview(p_AutoCollection);
    return true;
}

EAutoCollectionViewHorizontal* EAutoCollectionViewHorizontal::createWithLayout(const DLayout& layout)
{
    EAutoCollectionViewHorizontal* collectionView = new EAutoCollectionViewHorizontal();
    if (collectionView && collectionView->initWithLayout(layout))
    {
        collectionView->autorelease();
        return collectionView;
    }
    CC_SAFE_DELETE(collectionView);
    return NULL;
}

void EAutoCollectionViewHorizontal::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
//    CAScheduler::schedule(schedule_selector(AutoCollectionViewVerticalTest::refreshData1), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
    CAView* viewColor = createWithColor(CAColor_blue);
    viewColor->setLayout(DLayoutFill);
    
    this->addSubview(viewColor);
}

void EAutoCollectionViewHorizontal::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
//    CAScheduler::schedule(schedule_selector(AutoCollectionViewVerticalTest::refreshData2), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
    
}

//选中
void EAutoCollectionViewHorizontal::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    //选中
    CACollectionViewCell* cell = collectionView->cellForRowAtIndexPath(section, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("", NULL);
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
    CCLog("选中");
}

//取消选中
void EAutoCollectionViewHorizontal::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    //取消选中
    CCLog("取消选中");
}

//获取指定cell
CACollectionViewCell* EAutoCollectionViewHorizontal::collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    //根据标识获得CACollectionViewCell
    ECollectionViewHorizontalCell* Cell = dynamic_cast<ECollectionViewHorizontalCell*>(collectionView->dequeueReusableCellWithIdentifier("CrossApp"));
    
    //如果没有找到相应的CACollectionViewCell则新建一个
    if (Cell == NULL)
    {
        Cell = ECollectionViewHorizontalCell::create("CrossApp");
        
        //生成Item背景
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        Cell->getContentView()->addSubview(itemImage);
        
        //生成itemCALabel
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        Cell->getContentView()->addSubview(itemText);
    }
    
    //设置Item背景颜色
    CAView* itemImageView = Cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(HorizontalcolorArr.at(item));
    CCLog("row = %d", item);
    
    //设置itme文本显示
    char pos[20] = "";
    sprintf(pos, "(%d,%d)", section, item);
    CALabel* itemText = (CALabel*)Cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    
    return Cell;
}

//项目大小
DSize EAutoCollectionViewHorizontal::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(CCRANDOM_0_1() * 200 + 130, CCRANDOM_0_1() * 200 + 130);
}

//每个Section中Item的个数
unsigned int EAutoCollectionViewHorizontal::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)HorizontalcolorArr.size();
}

//section的个数
unsigned int EAutoCollectionViewHorizontal::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 3;
}

CAView* EAutoCollectionViewHorizontal::collectionViewSectionViewForHeaderInSection(CAAutoCollectionView *collectionView, const DSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor_gray);
    return view;
}

unsigned int EAutoCollectionViewHorizontal::collectionViewHeightForHeaderInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return 30;
}

CAView* EAutoCollectionViewHorizontal::collectionViewSectionViewForFooterInSection(CAAutoCollectionView *collectionView, const DSize& viewSize, unsigned int section)
{
    return NULL;
}

unsigned int EAutoCollectionViewHorizontal::collectionViewHeightForFooterInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return 0;
}



int AutoCollectionViewHorizontalNum = 0;

AutoCollectionViewHorizontalTest::AutoCollectionViewHorizontalTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
    
    HorizontalcolorArr.clear();
}

AutoCollectionViewHorizontalTest::~AutoCollectionViewHorizontalTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
    AutoCollectionViewHorizontalNum = 0;
}

void CDUIShowAutoCollectionView::AutoCollectionHorizontalRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showAutoCollectionHorizontalNavigationBar >= 1)
    {
        showAutoCollectionHorizontalNavigationBar = 0;
        AutoCollectionViewHorizontalNum = showAutoCollectionHorizontalNavigationBar;
    }
    else
    {
        AutoCollectionViewHorizontalNum = ++showAutoCollectionHorizontalNavigationBar;
    }
    
    AutoCollectionViewHorizontalTest* ViewContrllerAutoCollectionViewHorizontalTest = new AutoCollectionViewHorizontalTest();
    ViewContrllerAutoCollectionViewHorizontalTest->init();
    ViewContrllerAutoCollectionViewHorizontalTest->setNavigationBarItem(AutoCollectionHorizontalNavigationBar);
    ViewContrllerAutoCollectionViewHorizontalTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerAutoCollectionViewHorizontalTest, false);
}

void AutoCollectionViewHorizontalTest::viewDidLoad()
{
    //随机出颜色
    for (int i = 0; i < 60; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        
        //将随机的ccc4对象放入到容器里
        HorizontalcolorArr.push_back(ccc4(r, g, b, 255));
    }
    
    if (AutoCollectionViewHorizontalNum == 0)
    {
        p_AutoCollection = CAAutoCollectionView::createWithLayout(DLayoutFill);
        p_AutoCollection->setAllowsSelection(true);
        p_AutoCollection->setAllowsMultipleSelection(true);
        p_AutoCollection->setCollectionViewDelegate(this);
        p_AutoCollection->setCollectionViewDataSource(this);
        p_AutoCollection->setOrientation(CAAutoCollectionView::Horizontal);
        p_AutoCollection->setScrollViewDelegate(this);
        p_AutoCollection->setHoriCellInterval(20);
        p_AutoCollection->setVertCellInterval(20);
        this->getView()->addSubview(p_AutoCollection);
    }
    else
    {
        EAutoCollectionViewHorizontal* p_AutoCollection1 = EAutoCollectionViewHorizontal::createWithLayout(DLayoutFill);
        p_AutoCollection1->init();
        p_AutoCollection1->setEAutoCollectionViewHorizontalDelegate(this);
        this->getView()->addSubview(p_AutoCollection1);
    }
}

void AutoCollectionViewHorizontalTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

//选中
void AutoCollectionViewHorizontalTest::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    //选中
    CACollectionViewCell* cell = collectionView->cellForRowAtIndexPath(section, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("", NULL);
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
    CCLog("选中");
}

//取消选中
void AutoCollectionViewHorizontalTest::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    //取消选中
    CCLog("取消选中");
}

//获取指定cell
CACollectionViewCell* AutoCollectionViewHorizontalTest::collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    //根据标识获得CACollectionViewCell
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");

    //如果没有找到相应的CACollectionViewCell则新建一个
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        
        //生成Item背景
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        p_Cell->getContentView()->addSubview(itemImage);
        
        //生成itemCALabel
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        p_Cell->getContentView()->addSubview(itemText);
    }

    //设置Item背景颜色
    CAView* itemImageView = p_Cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(HorizontalcolorArr.at(item));
    CCLog("row = %d", item);
    
    //设置itme文本显示
    char pos[20] = "";
    sprintf(pos, "(%d,%d)", section, item);
    CALabel* itemText = (CALabel*)p_Cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    return  p_Cell;
    
}

//项目大小
DSize AutoCollectionViewHorizontalTest::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(230, 230);
}

//每个Section中Item的个数
unsigned int AutoCollectionViewHorizontalTest::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)HorizontalcolorArr.size();
}

//section的个数
unsigned int AutoCollectionViewHorizontalTest::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 1;
}
