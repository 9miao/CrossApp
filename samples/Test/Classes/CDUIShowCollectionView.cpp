//
//  CDUIShowCollectionView.cpp
//  Test
//
//  Created by renhongguang on 15/4/7.
//
//
#define CAColor_blueStyle ccc4(51,204,255,255)
#include "CDUIShowCollectionView.h"

#include "CDWebViewController.h"
//#include "CDUIShowView.h"
#include "AlertViewTest.h"
#include "ButtonTest.h"
#include "ImageViewTest.h"
#include "Scale9ImageViewTest.h"
#include "ActivityIndicatorViewTest.h"
#include "LabelTest.h"
#include "ProgressTest.h"
#include "SliderTest.h"
#include "SwitchTest.h"
#include "TextFieldTest.h"
#include "TextViewTest.h"
#include "SegmentedControlTest.h"
#include "TabBarControllerTest.h"
#include "PageViewTest.h"
#include "TableViewTest.h"
#include "ListViewTest.h"
#include "CollectionViewTest.h"
#include "ScrollViewTest.h"
#include "WebViewTest.h"
#include "FlashViewTest.h"
#include "GifViewTest.h"
#include "PickerViewTest.h"
#include "StepperTest.h"
#include "RenderImageTest.h"
#include "ViewAnimationTest.h"
#include "VideoPlayerControlViewTest.h"


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
    m_vTitle.push_back("Animation");
    m_vTitle.push_back("VideoPlayer");
}

CDUIShowCollectionView::~CDUIShowCollectionView()
{
    
}

void CDUIShowCollectionView::viewDidLoad()
{
    size = this->getView()->getBounds().size;

//    CACollectionView* p_Conllection = CACollectionView::createWithFrame(this->getView()->getBounds());
    CACollectionView* p_Conllection = CACollectionView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    p_Conllection->setAllowsSelection(true);
    p_Conllection->setCollectionViewDelegate(this);
    p_Conllection->setCollectionViewDataSource(this);
    p_Conllection->setScrollViewDelegate(this);
    p_Conllection->setHoriInterval(3);
    p_Conllection->setVertInterval(3);
    
    this->getView()->addSubview(p_Conllection);
}

void CDUIShowCollectionView::viewDidUnload()
{

}

void CDUIShowCollectionView::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    int index = row * 3 + item;
    
    switch (index)
    {
        case 0:
        {
            AlertViewTest* ViewContrllerAlertViewTest = new AlertViewTest();
            ViewContrllerAlertViewTest->init();
            ViewContrllerAlertViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerAlertViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerAlertViewTest, true);
             break;
        }
        case 1:
        {
            ButtonTest* ViewContrllerButton = new ButtonTest();
            ViewContrllerButton->init();
            ViewContrllerButton->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerButton->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerButton, true);
            break;
        }
        case 2:
        {
            ImageViewTest* ViewContrllerImageViewTest = new ImageViewTest();
            ViewContrllerImageViewTest->init();
            ViewContrllerImageViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerImageViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerImageViewTest, true);
            break;
        }
        case 3:
        {
            Scale9ImageViewTest* ViewContrllerScale9ImageViewTest = new Scale9ImageViewTest();
            ViewContrllerScale9ImageViewTest->init();
            ViewContrllerScale9ImageViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerScale9ImageViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerScale9ImageViewTest, true);
            break;
        }
        case 4:
        {
            ActivityIndicatorViewTest* ViewContrllerIndicatorViewTest = new ActivityIndicatorViewTest();
            ViewContrllerIndicatorViewTest->init();
            ViewContrllerIndicatorViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerIndicatorViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerIndicatorViewTest, true);
            break;
        }
        case 5:
        {
            LabelTest* ViewContrllerLabelTest = new LabelTest();
            ViewContrllerLabelTest->init();
            ViewContrllerLabelTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerLabelTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerLabelTest, true);
            break;
        }
        case 6:
        {
            ProgressTest* ViewContrllerProgressTest = new ProgressTest();
            ViewContrllerProgressTest->init();
            ViewContrllerProgressTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerProgressTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerProgressTest, true);
            break;
        }
        case 7:
        {
            SliderTest* ViewContrllerSliderTest = new SliderTest();
            ViewContrllerSliderTest->init();
            ViewContrllerSliderTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerSliderTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerSliderTest, true);
            break;
        }
        case 8:
        {
            SwitchTest* ViewContrllerSwitchTest = new SwitchTest();
            ViewContrllerSwitchTest->init();
            ViewContrllerSwitchTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerSwitchTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerSwitchTest, true);
            break;
        }
        case 9:
        {
            TextFieldTest* ViewContrllerTextFieldTest = new TextFieldTest();
            ViewContrllerTextFieldTest->init();
            ViewContrllerTextFieldTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerTextFieldTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerTextFieldTest, true);
            break;
        }
        case 10:
        {
            TextViewTest* ViewContrllerTextViewTest = new TextViewTest();
            ViewContrllerTextViewTest->init();
            ViewContrllerTextViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerTextViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerTextViewTest, true);
            break;
        }
        case 11:
        {
            SegmentedControlTest* ViewContrllerSegmentedControlTest = new SegmentedControlTest();
            ViewContrllerSegmentedControlTest->init();
            ViewContrllerSegmentedControlTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerSegmentedControlTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerSegmentedControlTest, true);
            break;
        }
        case 12:
        {
            TabBarControllerTest* ViewContrllerTabBarControllerTest = new TabBarControllerTest();
            ViewContrllerTabBarControllerTest->init();
            ViewContrllerTabBarControllerTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerTabBarControllerTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerTabBarControllerTest, true);
            break;
        }
        case 13:
        {
            PageViewTest* ViewContrllerPageViewTest = new PageViewTest();
            ViewContrllerPageViewTest->init();
            ViewContrllerPageViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerPageViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerPageViewTest, true);
            break;
        }
        case 14:
        {
            TableViewTest* ViewContrllerTableViewTest = new TableViewTest();
            ViewContrllerTableViewTest->init();
            ViewContrllerTableViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerTableViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerTableViewTest, true);
            break;
        }
        case 15:
        {
            ListViewTest* ViewContrllerListViewTest = new ListViewTest();
            ViewContrllerListViewTest->init();
            ViewContrllerListViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerListViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerListViewTest, true);
            break;
        }
        case 16:
        {
            CollectionViewTest* ViewContrllerCollectionViewTest = new CollectionViewTest();
            ViewContrllerCollectionViewTest->init();
            ViewContrllerCollectionViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerCollectionViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerCollectionViewTest, true);
            break;
        }
        case 17:
        {
            ScrollViewTest* ViewContrllerScrollViewTest = new ScrollViewTest();
            ViewContrllerScrollViewTest->init();
            ViewContrllerScrollViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerScrollViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerScrollViewTest, true);
            break;
        }
        case 18:
        {
            WebViewTest* ViewContrllerWebViewTest = new WebViewTest();
            ViewContrllerWebViewTest->init();
            ViewContrllerWebViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerWebViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerWebViewTest, true);
            break;
        }
        case 19:
        {
            FlashViewTest* ViewContrllerFlashViewTest = new FlashViewTest();
            ViewContrllerFlashViewTest->init();
            ViewContrllerFlashViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerFlashViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerFlashViewTest, true);
            break;
        }
        case 20:
        {
            GifViewTest* ViewContrllerGifViewTest = new GifViewTest();
            ViewContrllerGifViewTest->init();
            ViewContrllerGifViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerGifViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerGifViewTest, true);
            break;
        }
        case 21:
        {
            PickerViewTest* ViewContrllerPickerViewTest = new PickerViewTest();
            ViewContrllerPickerViewTest->init();
            ViewContrllerPickerViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerPickerViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerPickerViewTest, true);
            break;
        }
        case 22:
        {
            StepperTest* ViewContrllerStepperTest = new StepperTest();
            ViewContrllerStepperTest->init();
            ViewContrllerStepperTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerStepperTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerStepperTest, true);
            break;
        }
        case 23:
        {
            RenderImageTest* ViewContrllerRenderImageTest = new RenderImageTest();
            ViewContrllerRenderImageTest->init();
            ViewContrllerRenderImageTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerRenderImageTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerRenderImageTest, true);
            break;
        }
        case 24:
        {
            ViewAnimationTest* ViewContrllerViewAnimationTest = new ViewAnimationTest();
            ViewContrllerViewAnimationTest->init();
            ViewContrllerViewAnimationTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerViewAnimationTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerViewAnimationTest, true);
            break;
        }
        case 25:
        {
            VideoPlayerControlViewTest* ViewContrllerVideoPlayerControlViewTest = new VideoPlayerControlViewTest();
            ViewContrllerVideoPlayerControlViewTest->init();
            ViewContrllerVideoPlayerControlViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
            ViewContrllerVideoPlayerControlViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerVideoPlayerControlViewTest, true);
            break;
        }
        default:
            break;
    }

//    CDUIShowView* viewContrller = new CDUIShowView();
//    viewContrller->init();
//    viewContrller->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(index)));
//    viewContrller->autorelease();
//    RootWindow::getInstance()->getRootNavigationController()->pushViewController(viewContrller, true);
//    viewContrller->showUiWithIndex(index);
}

void CDUIShowCollectionView::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{

}

CACollectionViewCell* CDUIShowCollectionView::collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    if (row * 3 + item >= m_vTitle.size())
    {
        return NULL;
    }
    
    DSize _size = cellSize;
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        p_Cell->setAllowsSelected(false);
        
//        CAView* itemImage = CAView::createWithFrame(DRect(0, 0, _size.width, _size.height));
        CAView* itemImage = CAView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
        itemImage->setTag(99);
        p_Cell->addSubview(itemImage);
        
        DSize itemSize = itemImage->getBounds().size;
//        CALabel* itemText = CALabel::createWithCenter(DRect(itemSize.width/2, itemSize.height-40, itemSize.width,40));
        CALabel* itemText = CALabel::createWithLayout(DRectLayout(0,0,120,0,DRectLayout::L_R_T_B));
        itemText->setTag(100);
        itemText->setFontSize(24);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        p_Cell->addSubview(itemText);
        
//        CAImageView* icon = CAImageView::createWithCenter(DRect(itemSize.width/2,itemSize.height/3,189,123));
        CAImageView* icon = CAImageView::createWithLayout(DRectLayout(30,30,20,90,DRectLayout::L_R_T_B));
        icon->setTag(101);
        icon->setScale(0.6f);
        p_Cell->addSubview(icon);
    }
    
    CAView* itemImageView = p_Cell->getSubviewByTag(99);
    itemImageView->setColor(ccc4(244, 243, 243, 255));
    
    int index = row * 3 + item;
    
    CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(100);
    itemText->setText(m_vTitle.at(index));
	itemText->setColor(ccc4(34, 151, 254, 255));
    
    CAImageView* icon = (CAImageView*)p_Cell->getSubviewByTag(101);
    icon->setImage(CAImage::create(iconTag[index]));

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
    return (this->getView()->getBounds().size.width - 10 * 4) / 3;
}




