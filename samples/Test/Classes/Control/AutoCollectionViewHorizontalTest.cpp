
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

void EAutoCollectionViewHorizontal::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    CACollectionViewCell* cell = collectionView->cellForRowAtIndexPath(section, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("", NULL);
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
}

void EAutoCollectionViewHorizontal::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
}

CACollectionViewCell* EAutoCollectionViewHorizontal::collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    ECollectionViewHorizontalCell* Cell = dynamic_cast<ECollectionViewHorizontalCell*>(collectionView->dequeueReusableCellWithIdentifier("CrossApp"));
    
    if (Cell == NULL)
    {
        Cell = ECollectionViewHorizontalCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        Cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        Cell->getContentView()->addSubview(itemText);
    }
    
    CAView* itemImageView = Cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(HorizontalcolorArr.at(item));
    CCLog("row = %d", item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d)", section, item);
    CALabel* itemText = (CALabel*)Cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    
    return Cell;
}

DSize EAutoCollectionViewHorizontal::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(CCRANDOM_0_1() * 200 + 130, CCRANDOM_0_1() * 200 + 130);
}

unsigned int EAutoCollectionViewHorizontal::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)HorizontalcolorArr.size();
}

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
    for (int i = 0; i < 60; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        
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

void AutoCollectionViewHorizontalTest::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    CACollectionViewCell* cell = collectionView->cellForRowAtIndexPath(section, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("", NULL);
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
}

void AutoCollectionViewHorizontalTest::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
}

CACollectionViewCell* AutoCollectionViewHorizontalTest::collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");

    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        p_Cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        p_Cell->getContentView()->addSubview(itemText);
    }

    CAView* itemImageView = p_Cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(HorizontalcolorArr.at(item));
    CCLog("row = %d", item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d)", section, item);
    CALabel* itemText = (CALabel*)p_Cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    return  p_Cell;
    
}

DSize AutoCollectionViewHorizontalTest::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(230, 230);
}

unsigned int AutoCollectionViewHorizontalTest::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)HorizontalcolorArr.size();
}

unsigned int AutoCollectionViewHorizontalTest::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 1;
}
