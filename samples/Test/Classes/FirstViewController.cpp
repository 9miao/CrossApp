
#define CAColor_blueStyle CAColor4B(51,204,255,255)
#include "FirstViewController.h"

#include "MyWebViewController.h"
#include "AlertViewTest.h"
#include "ButtonTest.h"
#include "CheckboxTest.h"
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
#include "TabBarTest.h"
#include "PageViewTest.h"
#include "PageControlTest.h"
#include "TableViewTest.h"
#include "ListViewTest.h"
#include "CollectionViewTest.h"
#include "ScrollViewTest.h"
#include "WebViewTest.h"
#include "GifViewTest.h"
#include "PickerViewTest.h"
#include "StepperTest.h"
#include "RenderImageTest.h"
#include "ViewAnimationTest.h"
#include "AVPlayerViewTest.h"
#include "AutoCollectionViewHorizontalTest.h"
#include "AutoCollectionViewVerticalTest.h"
#include "WaterfallViewTest.h"
#include "ClippingViewTest.h"
#include "DrawViewTest.h"
#include "ImagePickerControllerTest.h"


static const char* iconTag[33] =
{
    "image/AlertView.png",
    "image/button.png",
    "image/Checkbox.png",
    
    "image/SegmentedControl.png",
    "image/ImageView.png",
    "image/Scale9ImageView.png",
    
    "image/indicatorView.png",
    "image/Progress.png",
    "image/slider.png",
    
    "image/Switch.png",
    "image/Stepper.png",
    "image/Label.png",
    
    "image/TextField.png",
    "image/TextView.png",
    "image/TabBar.png",
    
    "image/PageView.png",
    "image/PageControl.png",
    "image/ScrollView.png",
    
    "image/CollectionView.png",
    "image/CollectionView.png",
    "image/CollectionView.png",
    
    "image/CollectionView.png",
    "image/ListView.png",
    "image/TableView.png",
    
    "image/PickerView.png",
    "image/WebView.png",
    "image/GifView.png",
    
    "image/AVPlayer.png",
    "image/Render.png",
    "image/Animation.png",
    
    "image/Painting.png",
    "image/Clip.png",
    "image/Camera.png",

};


FirstViewController::FirstViewController()
{
    m_vTitles.push_back("AlertView");
    m_vTitles.push_back("Button");
    m_vTitles.push_back("Checkbox");
    
    m_vTitles.push_back("SegmentedControl");
    m_vTitles.push_back("ImageView");
    m_vTitles.push_back("Scale9ImageView");
    
    m_vTitles.push_back("IndicatorView");
    m_vTitles.push_back("Progress");
    m_vTitles.push_back("Slider");
    
    m_vTitles.push_back("Switch");
    m_vTitles.push_back("Stepper");
    m_vTitles.push_back("Label");
    
    m_vTitles.push_back("TextField");
    m_vTitles.push_back("TextView");
    m_vTitles.push_back("TabBar");
    
    m_vTitles.push_back("PageView");
    m_vTitles.push_back("PageControl");
    m_vTitles.push_back("ScrollView");
    
    m_vTitles.push_back("AutoCollectionView (Vertical)");
    m_vTitles.push_back("AutoCollectionView (Horizontal)");
    m_vTitles.push_back("CollectionView");
    
    m_vTitles.push_back("WaterfallView");
    m_vTitles.push_back("ListView");
    m_vTitles.push_back("TableView");
    
    m_vTitles.push_back("PickerView");
    m_vTitles.push_back("WebView");
    m_vTitles.push_back("GifView");
    
    m_vTitles.push_back("AVPlayer");
    m_vTitles.push_back("RenderImage");
    m_vTitles.push_back("Animation");
    
    m_vTitles.push_back("DrawView");
    m_vTitles.push_back("ClippingView");
    m_vTitles.push_back("ImagePickerController");
}

FirstViewController::~FirstViewController()
{
    
}

void FirstViewController::viewDidLoad()
{
    m_pAutoCollection = CAAutoCollectionView::createWithLayout(DLayoutFill);
    m_pAutoCollection->setAllowsSelection(true);
    m_pAutoCollection->setHoriCellInterval(3);
    m_pAutoCollection->setVertCellInterval(3);
    this->getView()->addSubview(m_pAutoCollection);
    
    m_pAutoCollection->onCellAtIndexPath(CALLBACK_BIND_3(FirstViewController::collectionCellAtIndex, this));
    m_pAutoCollection->onNumberOfItemsAtIndexPath(CALLBACK_BIND_1(FirstViewController::numberOfItemsInSection, this));
    m_pAutoCollection->onCellSizeAtIndexPath(CALLBACK_BIND_2(FirstViewController::collectionViewCellSizeAtIndexPath, this));
    
    m_pAutoCollection->onDidSelectCellAtIndexPath(CALLBACK_BIND_2(FirstViewController::collectionViewDidSelectCellAtIndexPath, this));
    
    ///*
    CGSpriteFrameCache::getInstance()->addSpriteFramesWithFile("animation/monster_12000.plist");
    
    CAVector<CGSpriteFrame*> vec;
    for (int i=0; i<7; i++)
    {
        CGSpriteFrame* spriteFrame = CGSpriteFrameCache::getInstance()->getSpriteFrameByName(crossapp_format_string("monster_12000_2_2_0%d.png", i+1));
        vec.pushBack(spriteFrame);
    }
    
    CGSprite* sprite = CGSprite::create();
    this->getView()->insertSubview(sprite, 1000);
    sprite->setPosition(DPoint(100, 600));
    sprite->setScale(1.5);

    Animation* movie = Animation::createWithSpriteFrames(vec, 0.18, 100);
    Animate* animate = Animate::create(movie);
    RepeatForever* repeatForever = RepeatForever::create(animate);
    sprite->runAction(repeatForever);
    
    MoveBy* moveBy = MoveBy::create(8, DPoint(500, 0));
    sprite->runAction(moveBy);
     //*/
    auto winsize = CAApplication::getApplication()->getWinSize();
    CCLog("%f %f", winsize.width, winsize.height);
}

void FirstViewController::viewDidUnload()
{
}

#define PUSH_VIEW_CONTROLLER(_VIEWCONTROLLER_) \
RootWindow::getInstance()->getRootNavigationController()->pushViewController(_VIEWCONTROLLER_::create(), true)

void FirstViewController::collectionViewDidSelectCellAtIndexPath(unsigned int section, unsigned int item)
{
    switch (item)
    {
        case 0:
            PUSH_VIEW_CONTROLLER(AlertViewTest);
            break;
        case 1:
            PUSH_VIEW_CONTROLLER(ButtonTest);
            break;
        case 2:
            PUSH_VIEW_CONTROLLER(CheckboxTest);
            break;
        case 3:
            PUSH_VIEW_CONTROLLER(SegmentedControlTest);
            break;
        case 4:
            PUSH_VIEW_CONTROLLER(ImageViewTest);
            break;
        case 5:
            PUSH_VIEW_CONTROLLER(Scale9ImageViewTest);
            break;
        case 6:
            PUSH_VIEW_CONTROLLER(ActivityIndicatorViewTest);
            break;
        case 7:
            PUSH_VIEW_CONTROLLER(ProgressTest);
            break;
        case 8:
            PUSH_VIEW_CONTROLLER(SliderTest);
            break;
        case 9:
            PUSH_VIEW_CONTROLLER(SwitchTest);
            break;
        case 10:
            PUSH_VIEW_CONTROLLER(StepperTest);
            break;
        case 11:
            PUSH_VIEW_CONTROLLER(LabelTest);
            break;
        case 12:
            PUSH_VIEW_CONTROLLER(TextFieldTest);
            break;
        case 13:
            PUSH_VIEW_CONTROLLER(TextViewTest);
            break;
        case 14:
            PUSH_VIEW_CONTROLLER(TabBarTest);
            break;
        case 15:
            PUSH_VIEW_CONTROLLER(PageViewTest);
            break;
        case 16:
            PUSH_VIEW_CONTROLLER(PageControlTest);
            break;
        case 17:
            PUSH_VIEW_CONTROLLER(ScrollViewTest);
            break;
        case 18:
            PUSH_VIEW_CONTROLLER(AutoCollectionViewVerticalTest);
            break;
        case 19:
            PUSH_VIEW_CONTROLLER(AutoCollectionViewHorizontalTest);
            break;
        case 20:
            PUSH_VIEW_CONTROLLER(CollectionViewTest);
            break;
        case 21:
            PUSH_VIEW_CONTROLLER(WaterfallViewTest);
            break;
        case 22:
            PUSH_VIEW_CONTROLLER(ListViewTest);
            break;
        case 23:
            PUSH_VIEW_CONTROLLER(TableViewTest);
            break;
        case 24:
            PUSH_VIEW_CONTROLLER(PickerViewTest);
            break;
        case 25:
            PUSH_VIEW_CONTROLLER(WebViewTest);
            break;
        case 26:
            PUSH_VIEW_CONTROLLER(GifViewTest);
            break;
        case 27:
            PUSH_VIEW_CONTROLLER(AVPlayerViewTest);
            break;
        case 28:
            PUSH_VIEW_CONTROLLER(RenderImageTest);
            break;
        case 29:
            PUSH_VIEW_CONTROLLER(ViewAnimationTest);
            break;
        case 30:
            PUSH_VIEW_CONTROLLER(DrawViewTest);
            break;
        case 31:
            PUSH_VIEW_CONTROLLER(ClippingViewTest);
            break;
        case 32:
            PUSH_VIEW_CONTROLLER(ImagePickerControllerTest);
            break;
        default:
            break;
    }
}

CACollectionViewCell* FirstViewController::collectionCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int item)
{
    auto cell = m_pAutoCollection->dequeueReusableCellWithIdentifier("CrossApp");
    if (cell == nullptr)
    {
        cell = CACollectionViewCell::create("CrossApp");
        cell->setAllowsSelected(false);
        
        CAView* itemImage = CAView::createWithLayout(DLayout(DLayoutFill));
        itemImage->setTag(99);
        cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(0,0), DVerticalLayout_B_H(5,80)));
        itemText->setTag(100);
        itemText->setFontSize(24);
        itemText->setTextAlignment(CATextAlignment::Center);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        cell->getContentView()->addSubview(itemText);
        
        CAImageView* icon = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(30,30),DVerticalLayout_T_H(20, 123)));
        icon->setScaleType(CAImageView::ScaleType::FitImageInside);
        icon->setTag(101);
        icon->setScale(0.6f);
        cell->getContentView()->addSubview(icon);
    }
    
    CAView* itemImageView = cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(CAColor4B(244, 243, 243, 255));
    
    CALabel* itemText = (CALabel*)cell->getContentView()->getSubviewByTag(100);
    itemText->setText(m_vTitles.at(item));
    itemText->setColor(CAColor4B(34, 151, 254, 255));
    
    CAImageView* icon = (CAImageView*)cell->getContentView()->getSubviewByTag(101);
//    icon->setImage(CAImage::create(iconTag[item]));
    icon->setImageAsyncWithFile(iconTag[item]);
    return cell;
}

DSize FirstViewController::collectionViewCellSizeAtIndexPath(unsigned int section, unsigned int item)
{
    return DSize(230, 230);
}

unsigned int FirstViewController::numberOfItemsInSection(unsigned int section)
{
    return (unsigned int)m_vTitles.size();
}



