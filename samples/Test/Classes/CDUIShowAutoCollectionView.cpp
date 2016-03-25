
#define CAColor_blueStyle ccc4(51,204,255,255)
#include "CDUIShowAutoCollectionView.h"

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


CDUIShowAutoCollectionView::CDUIShowAutoCollectionView()
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

CDUIShowAutoCollectionView::~CDUIShowAutoCollectionView()
{
    
}

void CDUIShowAutoCollectionView::viewDidLoad()
{
    //    CACollectionView* p_Conllection = CACollectionView::createWithFrame(this->getView()->getBounds());
    p_AutoCollection = CAAutoCollectionView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    p_AutoCollection->setAllowsSelection(true);
    p_AutoCollection->setCollectionViewDelegate(this);
    p_AutoCollection->setCollectionViewDataSource(this);
    p_AutoCollection->setScrollViewDelegate(this);
    p_AutoCollection->setHoriCellInterval(3);
    p_AutoCollection->setVertCellInterval(3);
    
    this->getView()->addSubview(p_AutoCollection);
}

void CDUIShowAutoCollectionView::viewDidUnload()
{
    
}

//选中
void CDUIShowAutoCollectionView::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    switch (item)
    {
        case 0:
        {
            AlertViewTest* ViewContrllerAlertViewTest = new AlertViewTest();
            ViewContrllerAlertViewTest->init();
            ViewContrllerAlertViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerAlertViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerAlertViewTest, true);
            break;
        }
        case 1:
        {
            ButtonTest* ViewContrllerButton = new ButtonTest();
            ViewContrllerButton->init();
            ViewContrllerButton->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerButton->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerButton, true);
            break;
        }
        case 2:
        {
            ImageViewTest* ViewContrllerImageViewTest = new ImageViewTest();
            ViewContrllerImageViewTest->init();
            ViewContrllerImageViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerImageViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerImageViewTest, true);
            break;
        }
        case 3:
        {
            Scale9ImageViewTest* ViewContrllerScale9ImageViewTest = new Scale9ImageViewTest();
            ViewContrllerScale9ImageViewTest->init();
            ViewContrllerScale9ImageViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerScale9ImageViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerScale9ImageViewTest, true);
            break;
        }
        case 4:
        {
            ActivityIndicatorViewTest* ViewContrllerIndicatorViewTest = new ActivityIndicatorViewTest();
            ViewContrllerIndicatorViewTest->init();
            ViewContrllerIndicatorViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerIndicatorViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerIndicatorViewTest, true);
            break;
        }
        case 5:
        {
            LabelTest* ViewContrllerLabelTest = new LabelTest();
            ViewContrllerLabelTest->init();
            ViewContrllerLabelTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerLabelTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerLabelTest, true);
            break;
        }
        case 6:
        {
            ProgressTest* ViewContrllerProgressTest = new ProgressTest();
            ViewContrllerProgressTest->init();
            ViewContrllerProgressTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerProgressTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerProgressTest, true);
            break;
        }
        case 7:
        {
            SliderTest* ViewContrllerSliderTest = new SliderTest();
            ViewContrllerSliderTest->init();
            ViewContrllerSliderTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerSliderTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerSliderTest, true);
            break;
        }
        case 8:
        {
            SwitchTest* ViewContrllerSwitchTest = new SwitchTest();
            ViewContrllerSwitchTest->init();
            ViewContrllerSwitchTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerSwitchTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerSwitchTest, true);
            break;
        }
        case 9:
        {
            TextFieldTest* ViewContrllerTextFieldTest = new TextFieldTest();
            ViewContrllerTextFieldTest->init();
            ViewContrllerTextFieldTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerTextFieldTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerTextFieldTest, true);
            break;
        }
        case 10:
        {
            TextViewTest* ViewContrllerTextViewTest = new TextViewTest();
            ViewContrllerTextViewTest->init();
            ViewContrllerTextViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerTextViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerTextViewTest, true);
            break;
        }
        case 11:
        {
            SegmentedControlTest* ViewContrllerSegmentedControlTest = new SegmentedControlTest();
            ViewContrllerSegmentedControlTest->init();
            ViewContrllerSegmentedControlTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerSegmentedControlTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerSegmentedControlTest, true);
            break;
        }
        case 12:
        {
            TabBarControllerTest* ViewContrllerTabBarControllerTest = new TabBarControllerTest();
            ViewContrllerTabBarControllerTest->init();
            ViewContrllerTabBarControllerTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerTabBarControllerTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerTabBarControllerTest, true);
            break;
        }
        case 13:
        {
            PageViewTest* ViewContrllerPageViewTest = new PageViewTest();
            ViewContrllerPageViewTest->init();
            ViewContrllerPageViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerPageViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerPageViewTest, true);
            break;
        }
        case 14:
        {
            TableViewTest* ViewContrllerTableViewTest = new TableViewTest();
            ViewContrllerTableViewTest->init();
            ViewContrllerTableViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerTableViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerTableViewTest, true);
            break;
        }
        case 15:
        {
            ListViewTest* ViewContrllerListViewTest = new ListViewTest();
            ViewContrllerListViewTest->init();
            ViewContrllerListViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerListViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerListViewTest, true);
            break;
        }
        case 16:
        {
            CollectionViewTest* ViewContrllerCollectionViewTest = new CollectionViewTest();
            ViewContrllerCollectionViewTest->init();
            ViewContrllerCollectionViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerCollectionViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerCollectionViewTest, true);
            break;
        }
        case 17:
        {
            ScrollViewTest* ViewContrllerScrollViewTest = new ScrollViewTest();
            ViewContrllerScrollViewTest->init();
            ViewContrllerScrollViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerScrollViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerScrollViewTest, true);
            break;
        }
        case 18:
        {
            WebViewTest* ViewContrllerWebViewTest = new WebViewTest();
            ViewContrllerWebViewTest->init();
            ViewContrllerWebViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerWebViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerWebViewTest, true);
            break;
        }
        case 19:
        {
            FlashViewTest* ViewContrllerFlashViewTest = new FlashViewTest();
            ViewContrllerFlashViewTest->init();
            ViewContrllerFlashViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerFlashViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerFlashViewTest, true);
            break;
        }
        case 20:
        {
            GifViewTest* ViewContrllerGifViewTest = new GifViewTest();
            ViewContrllerGifViewTest->init();
            ViewContrllerGifViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerGifViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerGifViewTest, true);
            break;
        }
        case 21:
        {
            PickerViewTest* ViewContrllerPickerViewTest = new PickerViewTest();
            ViewContrllerPickerViewTest->init();
            ViewContrllerPickerViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerPickerViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerPickerViewTest, true);
            break;
        }
        case 22:
        {
            StepperTest* ViewContrllerStepperTest = new StepperTest();
            ViewContrllerStepperTest->init();
            ViewContrllerStepperTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerStepperTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerStepperTest, true);
            break;
        }
        case 23:
        {
            RenderImageTest* ViewContrllerRenderImageTest = new RenderImageTest();
            ViewContrllerRenderImageTest->init();
            ViewContrllerRenderImageTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerRenderImageTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerRenderImageTest, true);
            break;
        }
        case 24:
        {
            ViewAnimationTest* ViewContrllerViewAnimationTest = new ViewAnimationTest();
            ViewContrllerViewAnimationTest->init();
            ViewContrllerViewAnimationTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerViewAnimationTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerViewAnimationTest, true);
            break;
        }
        case 25:
        {
            VideoPlayerControlViewTest* ViewContrllerVideoPlayerControlViewTest = new VideoPlayerControlViewTest();
            ViewContrllerVideoPlayerControlViewTest->init();
            ViewContrllerVideoPlayerControlViewTest->setNavigationBarItem(CANavigationBarItem::create(m_vTitle.at(item)));
            ViewContrllerVideoPlayerControlViewTest->autorelease();
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(ViewContrllerVideoPlayerControlViewTest, true);
            break;
        }
        default:
            break;
    }
}

//取消选中
void CDUIShowAutoCollectionView::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    //取消选中
    CCLog("取消选中");
}

//获取指定cell
CACollectionViewCell* CDUIShowAutoCollectionView::collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    DSize _size = cellSize;
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        p_Cell->setAllowsSelected(false);
        
        //        CAView* itemImage = CAView::createWithFrame(DRect(0, 0, _size.width, _size.height));
        CAView* itemImage = CAView::createWithLayout(DRectLayout(0, 0, 0, 0,DRectLayout::L_R_T_B));
        itemImage->setTag(99);
        p_Cell->addSubview(itemImage);
        
        DSize itemSize = itemImage->getBounds().size;
        CALabel* itemText = CALabel::createWithLayout(DRectLayout(0, 0, 0, 40,DRectLayout::L_R_B_H));
        itemText->setTag(100);
        itemText->setFontSize(24);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        p_Cell->addSubview(itemText);
        
        CAImageView* icon = CAImageView::createWithLayout(DRectLayout(30, 30, 20, 123,DRectLayout::L_R_T_H));
        icon->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        icon->setTag(101);
        icon->setScale(0.6f);
        p_Cell->addSubview(icon);
    }
    
    CAView* itemImageView = p_Cell->getSubviewByTag(99);
    itemImageView->setColor(ccc4(244, 243, 243, 255));
    
    CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(100);
    itemText->setText(m_vTitle.at(item));
    itemText->setColor(ccc4(34, 151, 254, 255));
    
    CAImageView* icon = (CAImageView*)p_Cell->getSubviewByTag(101);
    icon->setImage(CAImage::create(iconTag[item]));
    
    return p_Cell;
}

//项目大小
DSize CDUIShowAutoCollectionView::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(240, 240);
}

//每个Section中Item的个数
unsigned int CDUIShowAutoCollectionView::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)m_vTitle.size();
}

//section的个数
unsigned int CDUIShowAutoCollectionView::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 1;
}


