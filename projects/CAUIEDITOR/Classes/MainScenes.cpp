//
//  MainScenes.cpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/13.
//
//

#include "MainScenes.h"
#include "SqlManage.h"
#include "RootWindow.h"
#include "PropertyView.h"

MainScenes::MainScenes()
{
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
}

MainScenes::~MainScenes()
{
    
}

void MainScenes::viewDidLoad()
{
    winsize = this->getView()->getBounds().size;
    //菜单栏
    CAImageView* title_bg = CAImageView::createWithFrame(CADipRect(0,0,winsize.width,50));
    title_bg->setImage(CAImage::create("image/navbg.jpg"));
    this->getView()->addSubview(title_bg);
    initMenuBar();
    
    //控件栏
    p_control = CAView::createWithFrame(CADipRect(winsize.width-300,winsize.height/2,300,winsize.height/2));
    p_Conllection = CAAutoCollectionView::createWithFrame(CADipRect(0,0,300,winsize.height/2));
    p_Conllection->setAllowsSelection(true);
    p_Conllection->setCollectionViewDelegate(this);
    p_Conllection->setCollectionViewDataSource(this);
    p_Conllection->setCollectionViewDelegate(this);
    p_Conllection->setCollectionViewOrientation(CACollectionViewOrientationVertical);
    p_control->addSubview(p_Conllection);
    this->getView()->addSubview(p_control);
    //属性栏
    p_property = new PropertyView(ViewType::ViewTypeCAButton);
    p_property->initWithFrame(CADipRect(winsize.width-300,50,300,winsize.height/2-50));
    this->getView()->addSubview(p_property);
    //场景
    scence_view = new CanvasScenes();
    scence_view->initWithFrame(CADipRect(300,50,winsize.width-600,winsize.height));
    scence_view->setColor(CAColor_gray);
//    p_ScrollView = CAScrollView::createWithFrame(CADipRect(0,0,winsize.width-300,winsize.height));
//    p_ScrollView->setMinimumZoomScale(1.0f);
//    p_ScrollView->setMaximumZoomScale(2.5f);
//    p_ScrollView->setBackGroundColor(CAColor_clear);
//    p_ScrollView->setShowsScrollIndicators(false);
//    scence_view->addSubview(p_ScrollView);
    
    p_view = new CanvasView();
    p_view->initWithCenter(CCRect(scence_view->getBounds().size.width/2,scence_view->getBounds().size.height/2,640,960));
    scence_view->addSubview(p_view);
    this->getView()->addSubview(scence_view);
    scence_view->release();
    p_view->release();
}

void MainScenes::viewDidUnload()
{

}

void MainScenes::initMenuBar()
{
    p_MenuView = CAMenuView::create(CCRect(0,0,winsize.width,50),5);
    p_MenuView->setDelegate(this);
    this->getView()->addSubview(p_MenuView);
    
    p_MenuView->setMenuWithIndex("File", CCRect(0,0,100,50), 1);
    p_MenuView->setMenuWithIndex("Edit", CCRect(100,0,100,50), 2);
    p_MenuView->setMenuWithIndex("Product", CCRect(200,0,100,50), 3);
    p_MenuView->setMenuWithIndex("Help", CCRect(300,0,100,50), 4);
}

void MainScenes::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    
}

void MainScenes::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    
}

CACollectionViewCell* MainScenes::collectionCellAtIndex(CAAutoCollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int item)
{
    CADipSize _size = cellSize;
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        p_Cell->setAllowsSelected(false);
    }
    p_Cell->removeAllSubviews();

    CAView* itemImage = CAView::createWithFrame(CADipRect(0, 0, _size.width, _size.height));
    p_Cell->addSubview(itemImage);
    
    CADipSize itemSize = itemImage->getBounds().size;
    CALabel* itemText = CALabel::createWithCenter(CADipRect(itemSize.width/2, itemSize.height-20, itemSize.width,40));
    itemText->setTag(100);
    itemText->setFontSize(_px(14));
    itemText->setTextAlignment(CATextAlignmentCenter);
    itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    itemText->setText(m_vTitle.at(item));
    itemText->setColor(ccc4(34,151,254,255));
    p_Cell->addSubview(itemText);
    
    CAButton* btn = CAButton::createWithCenter(CADipRect(itemSize.width/2,itemSize.height/3,189/2,123/2),CAButtonTypeCustom);
    btn->setImageForState(CAControlStateAll, CAImage::create(iconTag[item]));
    btn->setTag(item);
    btn->addTarget(this, CAControl_selector(MainScenes::addControl), CAControlEventTouchDown);
    btn->addTarget(this, CAControl_selector(MainScenes::movedControl), CAControlEventTouchMoved);
    btn->addTarget(this, CAControl_selector(MainScenes::movedControl), CAControlEventTouchMovedOutSide);
    btn->addTarget(this, CAControl_selector(MainScenes::selectedControl), CAControlEventTouchUpInSide);
    btn->addTarget(this, CAControl_selector(MainScenes::putDownControl), CAControlEventTouchUpOutSide);
    btn->addTarget(this, CAControl_selector(MainScenes::cancelControl), CAControlEventTouchCancelled);
    p_Cell->addSubview(btn);
    return p_Cell;
}

void MainScenes::addControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CCLog("addControl");
    p_Conllection->setVerticalScrollEnabled(false);
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    CCSize temp_size;
    point = btn->convertToWorldSpace(point);
    
    ViewType viewtype;
    switch (tag) {
        case 0:
            viewtype = ViewTypeCAButton;
            temp_size = CCSize(200,50);
            break;
        case 1:
            viewtype = ViewTypeCAImageView;
            temp_size = CCSize(200,200);
            break;
        case 2:
            viewtype = ViewTypeCAScale9ImageView;
            temp_size = CCSize(200,200);
            break;
        case 3:
            viewtype = viewTypeCAIndicator;
            temp_size = CCSize(50,50);
            break;
        case 4:
            viewtype = ViewTypeCALabel;
            temp_size = CCSize(100,50);
            break;
        case 5:
            viewtype = viewTypeCAProgress;
            temp_size = CCSize(400,0);
            break;
        case 6:
            viewtype = viewTypeCASlider;
            temp_size = CCSize(400,10);
            break;
        case 7:
            viewtype = viewTypeCASwitch;
            temp_size = CCSize(20,10);
            break;
        case 8:
            viewtype = viewTypeCATextField;
            temp_size = CCSize(200,50);
            break;
        case 9:
            viewtype = viewTypeCATextView;
            temp_size = CCSize(200,100);
            break;
        case 10:
            viewtype = ViewTypeCASegmentControl;
            temp_size = CCSize(300,30);
            break;
        case 11:
            viewtype = ViewTypeCATabBar;
            temp_size = CCSize(p_view->getFrame().size.width,80);
            break;
        case 12:
            viewtype = viewTypeCAPageView;
            break;
        case 13:
            viewtype = viewTypeCATableView;
            break;
        case 14:
            viewtype = viewTypeCAListView;
            break;
        case 15:
            viewtype = viewTypeCAColectionView;
            break;
        case 16:
            viewtype = viewTypeCAScrollView;
            break;
        case 17:
            viewtype = viewTypeCAWebView;
            temp_size = CCSize(300,300);
            break;
        case 18:
            viewtype = viewTypeCAFlashView;
            temp_size = CCSize(200,200);
            break;
        case 19:
            viewtype = viewTypeCAGifView;
            temp_size = CCSize(200,200);
            break;
        case 20:
            viewtype = viewTypeCAPickerView;
            temp_size = CCSize(p_view->getFrame().size.width,280);
            break;
        case 21:
            viewtype = viewTypeCAStepper;
            temp_size = CCSize(200,50);
            break;
    }

    p_tempView = new AutoView(viewtype,false);
    p_tempView->initWithFrame(CCRect(point.x,point.y,temp_size.width,temp_size.height));
    p_tempView->setTitleFontSize(18);
    this->getView()->addSubview(p_tempView);
    p_tempView->release();
}

void MainScenes::movedControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    point = btn->convertToWorldSpace(point);
    if (p_tempView) {
        p_tempView->setCenterOrigin(point);
    }
    
}

void MainScenes::selectedControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    this->getView()->removeSubview(p_tempView);
    p_tempView = NULL;
}

void MainScenes::putDownControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    
    if (p_tempView) {
        CCRect temp_rect = scence_view->convertRectToNodeSpace(p_tempView->getFrame());
        if (p_view->getFrame().intersectsRect(temp_rect)) {
            AutoView* av = new AutoView(p_tempView->getViewType(),false);
            av->initWithFrame(p_view->convertRectToNodeSpace(p_tempView->getFrame()));
            av->setAutoViewDelegate(this);
            av->setTitleFontSize(18);
            p_view->addSubview(av);
            av->setTag(100);
            
            //SqlManage::insertCAViewsLayout(crossapp_format_string("%.f",av->m_DM.ix), crossapp_format_string("%.f",av->m_DM.iy), crossapp_format_string("%.f",av->m_DM.iw), crossapp_format_string("%.f",av->m_DM.ih));
            av->release();
        }
    }
    this->getView()->removeSubview(p_tempView);
    p_tempView = NULL;
}

void MainScenes::cancelControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    p_Conllection->setVerticalScrollEnabled(false);
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    this->getView()->removeSubview(p_tempView);
    p_tempView = NULL;
}

unsigned int MainScenes::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 1;
}

unsigned int MainScenes::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return m_vTitle.size();
}

CCSize MainScenes::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return CCSize(95,100);
}

bool MainScenes::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return true;
}

void MainScenes::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CCLog("sds");
}

void MainScenes::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{

}

void MainScenes::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void MainScenes::AutoViewChangeFrameStart()
{
   
}

void MainScenes::AutoViewChangeFrameEnd()
{
    
}

void MainScenes::AutoViewMoveToPosition(CCPoint& p)
{
}

void MainScenes::selectedMenuViewItem(CrossApp::CAMenuView *menuView, int tag, std::string titleName)
{
    
}