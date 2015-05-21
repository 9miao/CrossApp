//
//  CDUIShowCollectionView.cpp
//  Test
//
//  Created by renhongguang on 15/4/7.
//
//
#define CAColor_blueStyle ccc4(51,204,255,255)
#include "CDUIShowCollectionView.h"
#include "CDUIShowView.h"
#include "CDWebViewController.h"
#include "RootWindow.h"
#include "CDData.h"

#define VIEWCONTROLLER_SWITCH(p_Contrller,index)											\
p_Contrller* p_ViewContrller = new p_Contrller();										\
p_ViewContrller->init();\
p_ViewContrller->setNavigationBarItem(CANavigationBarItem::create(UINAME.at(index))); \
p_ViewContrller->autorelease();															\
RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);         \
RootWindow::getInstance()->getRootNavigationController()->pushViewController(p_ViewContrller, true);\
p_ViewContrller->initUIWithIndex(index);

CDUIShowCollectionView::CDUIShowCollectionView()
{
    UINAME.push_back("AlertView");
    UINAME.push_back("Button");
    UINAME.push_back("ImageView");
    UINAME.push_back("Scale9ImageView");
    UINAME.push_back("IndicatorView");
    UINAME.push_back("Label");
    UINAME.push_back("Progress");
    UINAME.push_back("Slider");
    UINAME.push_back("Switch");
    UINAME.push_back("TextField");
    UINAME.push_back("TextView");
    UINAME.push_back("SegmentedControl");
    UINAME.push_back("TabBar");
    UINAME.push_back("PageView");
    UINAME.push_back("TableView");
    UINAME.push_back("ListView");
    UINAME.push_back("WebView");
    UINAME.push_back("CollectionView");
    UINAME.push_back("ScrollView");
}

CDUIShowCollectionView::~CDUIShowCollectionView()
{
    //    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    //    drawer->setTouchMoved(true);
}

void CDUIShowCollectionView::viewDidLoad()
{
    size = this->getView()->getBounds().size;

//    headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
//    footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    
    p_Conllection = CACollectionView::createWithFrame(this->getView()->getBounds());
    p_Conllection->setAllowsSelection(true);
    p_Conllection->setAllowsMultipleSelection(true);
    p_Conllection->setCollectionViewDelegate(this);
    p_Conllection->setCollectionViewDataSource(this);
    p_Conllection->setScrollViewDelegate(this);
//    p_Conllection->setHeaderRefreshView(headerRefreshView);
//    p_Conllection->setFooterRefreshView(footerRefreshView);
    p_Conllection->setHoriInterval(_px(3));
    p_Conllection->setVertInterval(_px(3));
    
    this->getView()->addSubview(p_Conllection);
}

void CDUIShowCollectionView::viewDidUnload()
{
    
}

void CDUIShowCollectionView::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    int tempIndex = row * 3 + item;
    VIEWCONTROLLER_SWITCH(CDUIShowView, tempIndex);
    
}

void CDUIShowCollectionView::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    int tempIndex = row * 3 + item;
    VIEWCONTROLLER_SWITCH(CDUIShowView, tempIndex);
}

CACollectionViewCell* CDUIShowCollectionView::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    if (row * 3 + item >= UINAME.size())
    {
        return NULL;
    }
    
    CADipSize _size = cellSize;
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithFrame(CADipRect(0, 0, _size.width, _size.height));
        itemImage->setTag(99);
        p_Cell->addSubview(itemImage);
        
        CADipSize itemSize = itemImage->getBounds().size;
        CALabel* itemText = CALabel::createWithCenter(CADipRect(itemSize.width/2, itemSize.height-40, itemSize.width,40));
        itemText->setTag(100);
        itemText->setFontSize(_px(24));
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        p_Cell->addSubview(itemText);
        
        CAImageView* icon = CAImageView::createWithCenter(CADipRect(itemSize.width/2,itemSize.height/3,189,123));
        icon->setTag(101);
        icon->setScale(0.6f);
        p_Cell->addSubview(icon);
    }
    CAView* itemImageView = p_Cell->getSubviewByTag(99);
    itemImageView->setColor(ccc4(244, 243, 243, 255));
    CCLog("%d", row * 3 + item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d,%d)", section, row, item);
    CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(100);
    itemText->setText(UINAME.at(row*3+item));
    itemText->setColor(ccc4(34,151,254,255));
    
    CAImageView* icon = (CAImageView*)p_Cell->getSubviewByTag(101);
    icon->setImage(CAImage::create(iconTag[row*3+item]));
    
    return p_Cell;
}

unsigned int CDUIShowCollectionView::numberOfSections(CACollectionView *collectionView)
{
    return 1;
}

unsigned int CDUIShowCollectionView::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    return UINAME.size() % 3 == 0 ? UINAME.size() / 3 : UINAME.size() / 3 + 1;
}

unsigned int CDUIShowCollectionView::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return 3;
}

unsigned int CDUIShowCollectionView::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    return (this->getView()->getBounds().size.width - _px(10) * 4) / 3;
}

void CDUIShowCollectionView::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    //CAScheduler::schedule(schedule_selector(CDUIShowCollectionView::refreshData), this, 0.1, 0, CCRANDOM_0_1() * 2, false);
}

void CDUIShowCollectionView::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    //CAScheduler::schedule(schedule_selector(CDUIShowCollectionView::refreshData), this, 0.1, 0, CCRANDOM_0_1() * 2, false);
}

void CDUIShowCollectionView::refreshData(float interval)
{
    p_Conllection->reloadData();
}