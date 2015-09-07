//
//  MainScenes.cpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/13.
//
//

#include "MainScenes.h"
#include "CDData.h"

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
    //控件栏
    p_Conllection = CACollectionView::createWithFrame(CADipRect(0,0,300,winsize.height));
    p_Conllection->setAllowsSelection(true);
    p_Conllection->setCollectionViewDelegate(this);
    p_Conllection->setCollectionViewDataSource(this);
    p_Conllection->setHoriInterval(_px(3));
    p_Conllection->setVertInterval(_px(3));
    p_Conllection->setCollectionViewDelegate(this);
    this->getView()->addSubview(p_Conllection);
    //场景
    scence_view = new CanvasScenes();
    scence_view->initWithFrame(CADipRect(300,0,winsize.width-300,winsize.height));
    scence_view->setColor(CAColor_gray);
    p_view = new CanvasView();
    p_view->initWithCenter(CCRect(scence_view->getBounds().size.width/2,scence_view->getBounds().size.height/2,320,480));
    scence_view->addSubview(p_view);
    this->getView()->addSubview(scence_view);
    scence_view->release();
    p_view->release();
}

void MainScenes::viewDidUnload()
{
    
}
void MainScenes::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    int index = row * 3 + item;
}

void MainScenes::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    
}

CACollectionViewCell* MainScenes::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
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
    }
    p_Cell->removeAllSubviews();
    int index = row * 3 + item;
    CAView* itemImage = CAView::createWithFrame(CADipRect(0, 0, _size.width, _size.height));
    p_Cell->addSubview(itemImage);
    
    CADipSize itemSize = itemImage->getBounds().size;
    CALabel* itemText = CALabel::createWithCenter(CADipRect(itemSize.width/2, itemSize.height-40, itemSize.width,40));
    itemText->setTag(100);
    itemText->setFontSize(_px(18));
    itemText->setTextAlignment(CATextAlignmentCenter);
    itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    itemText->setText(m_vTitle.at(index));
    itemText->setColor(ccc4(34,151,254,255));
    p_Cell->addSubview(itemText);
    
    CAButton* btn = CAButton::createWithCenter(CADipRect(itemSize.width/2,itemSize.height/3,189,123),CAButtonTypeCustom);
    btn->setImageForState(CAControlStateAll, CAImage::create(iconTag[index]));
    btn->setTag(index);
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
    point = btn->convertToWorldSpace(point);
    
    ViewType viewtype;
    switch (tag) {
        case 0:
            viewtype = ViewTypeCAButton;
            break;
        case 1:
            viewtype = ViewTypeCAImageView;
            break;
        case 3:
            viewtype = ViewTypeCAScale9ImageView;
            break;
        case 4:
            viewtype = ViewTypeCALabel;
            break;
        case 5:
            viewtype = ViewTypeCAButton;
            break;
    }

    p_tempView = new AutoView(viewtype,false);
    p_tempView->initWithFrame(CCRect(point.x,point.y,100,100));
    this->getView()->addSubview(p_tempView);
    p_tempView->release();
    //m_autoViews.push_back(p_tempView);
}

void MainScenes::movedControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CCLog("movedControl");
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    point = btn->convertToWorldSpace(point);
    
    if (p_tempView) {
        p_tempView->setCenterOrigin(point);
    }
    
}

void MainScenes::selectedControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CCLog("selectedControl");
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    
    this->getView()->removeSubview(p_tempView);
    p_tempView = NULL;
}

void MainScenes::putDownControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CCLog("putDownControl");
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    
    if (p_tempView) {
        CCRect temp_rect = scence_view->convertRectToNodeSpace(p_tempView->getFrame());
        if (p_view->getFrame().intersectsRect(temp_rect)) {
            AutoView* av = new AutoView(p_tempView->getViewType(),false);
            av->initWithFrame(p_view->convertRectToNodeSpace(p_tempView->getFrame()));
            av->setAutoViewDelegate(this);
            av->setTag(100);
            //av->setFrameOrigin(scence_view->convertToNodeSpace(p_tempView->getFrameOrigin()));
            p_view->addSubview(av);
        }
    }
    this->getView()->removeSubview(p_tempView);
    p_tempView = NULL;
}

void MainScenes::cancelControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    p_Conllection->setVerticalScrollEnabled(false);
    CCLog("cancelControl");
    CAButton* btn = (CAButton*)control;
    int tag = btn->getTag();
    
    this->getView()->removeSubview(p_tempView);
    p_tempView = NULL;
}

unsigned int MainScenes::numberOfSections(CACollectionView *collectionView)
{
    return 1;
}

unsigned int MainScenes::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    return m_vTitle.size() % 3 == 0 ? m_vTitle.size() / 3 : m_vTitle.size() / 3 + 1;
}

unsigned int MainScenes::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return 2;
}

unsigned int MainScenes::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    return _px(250);
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
   // scence_view->setDrawRect(false);
}

void MainScenes::AutoViewChangeFrameEnd()
{
    //scence_view->setDrawRect(true);
}

void MainScenes::AutoViewMoveToPosition(CCPoint& p)
{
}