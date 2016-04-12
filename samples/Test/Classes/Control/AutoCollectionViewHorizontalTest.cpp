
#include "AutoCollectionViewHorizontalTest.h"

AutoCollectionViewHorizontalTest::AutoCollectionViewHorizontalTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
    
    colorArr.clear();
}

AutoCollectionViewHorizontalTest::~AutoCollectionViewHorizontalTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
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
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    
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
    itemImageView->setColor(colorArr.at(item));
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
    return (unsigned int)colorArr.size();
}

//section的个数
unsigned int AutoCollectionViewHorizontalTest::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 1;
}
