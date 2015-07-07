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


CDUIShowCollectionView::CDUIShowCollectionView()
{
	m_vTitle.push_back("AlertView");
    m_vTitle.push_back("Button");
    m_vTitle.push_back("ImageView");
    m_vTitle.push_back("Scale9ImageView");
    m_vTitle.push_back("IndicatorView");
    m_vTitle.push_back("Label");
    m_vTitle.push_back("Progress");
    m_vTitle.push_back("Slider");
    m_vTitle.push_back("Switch");
    m_vTitle.push_back("TextField");
    m_vTitle.push_back("TextView");
    m_vTitle.push_back("SegmentedControl");
    m_vTitle.push_back("TabBar");
    m_vTitle.push_back("PageView");
    m_vTitle.push_back("TableView");
    m_vTitle.push_back("ListView");
    m_vTitle.push_back("CollectionView");
    m_vTitle.push_back("ScrollView");
    m_vTitle.push_back("WebView");
    m_vTitle.push_back("FlashView");
    m_vTitle.push_back("GifView");
    m_vTitle.push_back("PickerView");
    m_vTitle.push_back("Stepper");
    m_vTitle.push_back("RenderImage");
    //m_vTitle.push_back("Video");
}

CDUIShowCollectionView::~CDUIShowCollectionView()
{
    
}

void CDUIShowCollectionView::viewDidLoad()
{
    size = this->getView()->getBounds().size;

    CACollectionView* p_Conllection = CACollectionView::createWithFrame(this->getView()->getBounds());
    p_Conllection->setAllowsSelection(true);
    p_Conllection->setCollectionViewDelegate(this);
    p_Conllection->setCollectionViewDataSource(this);
    p_Conllection->setScrollViewDelegate(this);
    p_Conllection->setHoriInterval(_px(3));
    p_Conllection->setVertInterval(_px(3));
    
    this->getView()->addSubview(p_Conllection);
}

void CDUIShowCollectionView::viewDidUnload()
{

}

void CDUIShowCollectionView::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    int index = row * 3 + item;
    
    CDUIShowView* viewContrller = new CDUIShowView();
    viewContrller->init();
    viewContrller->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
    viewContrller->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(viewContrller, true);
    viewContrller->showUiWithIndex(index);
}

void CDUIShowCollectionView::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{

}

CACollectionViewCell* CDUIShowCollectionView::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    if (row * 3 + item >= m_vTitle.size())
    {
        return NULL;
    }
    
    CADipSize _size = cellSize;
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        p_Cell->setAllowsSelected(false);
        
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
    
    int index = row * 3 + item;
    
    CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(100);
    itemText->setText(m_vTitle.at(index));
    itemText->setColor(ccc4(34,151,254,255));
    
    CAImageView* icon = (CAImageView*)p_Cell->getSubviewByTag(101);
    icon->setImage(CAImage::create(iconTag[index])->generate_mipmaps());

    return p_Cell;
}

unsigned int CDUIShowCollectionView::numberOfSections(CACollectionView *collectionView)
{
    return 1;
}

unsigned int CDUIShowCollectionView::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    return m_vTitle.size() % 3 == 0 ? m_vTitle.size() / 3 : m_vTitle.size() / 3 + 1;
}

unsigned int CDUIShowCollectionView::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return 3;
}

unsigned int CDUIShowCollectionView::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    return (this->getView()->getBounds().size.width - _px(10) * 4) / 3;
}