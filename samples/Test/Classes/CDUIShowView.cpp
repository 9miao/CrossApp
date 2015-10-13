//
//  CDUIShowView.cpp
//  Test
//
//  Created by renhongguang on 15/4/7.
//
//

#include "CDUIShowView.h"
#include "support/device/CADevice.h"
#include "CrossAppExt.h"

USING_NS_CC;
USING_NS_CC_EXT;

std::vector<std::string> sectionTitle;

ETableViewCell::ETableViewCell()
{
    this->setAllowsSelected(false);
}

ETableViewCell::~ETableViewCell()
{
    
}

ETableViewCell* ETableViewCell::create(const std::string& identifier, const DRect& _rect)
{
    ETableViewCell* tableViewCell = new ETableViewCell();
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
        tableViewCell->setFrame(_rect);
        tableViewCell->autorelease();
        return tableViewCell;
    }
    CC_SAFE_DELETE(tableViewCell);
    return NULL;
}

void ETableViewCell::highlightedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_clear));
}

void ETableViewCell::selectedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_clear));
}

void ETableViewCell::initWithCell()
{
    DSize _size = this->getFrame().size;
    CALabel* test = CALabel::createWithCenter(DRect(_size.width/2,
                                                        _size.height/2,
                                                        _size.width,
                                                        _size.height));
	test->setColor(ccc4(34, 151, 254, 255));
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(28));
    test->setTag(100);
    this->addSubview(test);
}
//-------------------------------------------------------------------------
//ETableView
ETableView::ETableView(){
    
}

ETableView::~ETableView(){
}

bool ETableView::init(const CrossApp::DRect &rect)
{
    size = rect.size;
    p_TableView = CATableView::createWithFrame(DRect(0, 0, size.width, size.height));
    p_TableView->setAllowsSelection(true);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setTableViewDataSource(this);
    p_TableView->setShowsScrollIndicators(false);
    p_TableView->setSeparatorViewHeight(0);
    p_TableView->setScrollEnabled(false);
    p_TableView->setSeparatorColor(CAColor_clear);
    this->addSubview(p_TableView);
    return true;
}

ETableView* ETableView::createWithFrame(const CrossApp::DRect &rect)
{
    ETableView *pRet = new ETableView();
    if (pRet && pRet->init(rect))
    {
        pRet->setFrame(rect);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

ETableView* ETableView::createWithCenter(const CrossApp::DRect &rect)
{
    ETableView *pRet = new ETableView();
    if (pRet && pRet->init(rect))
    {
        pRet->setCenter(rect);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void ETableView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    m_pETableViewDelegate->etableViewDidSelectRowAtIndexPath(table, section, row);
}

void ETableView::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* ETableView::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    ETableViewCell* cell = dynamic_cast<ETableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = ETableViewCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
        cell->initWithCell();
    }
    char order[20] = "";
    sprintf(order, "%s", sectionTitle.at(row).c_str());
    CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
    cellText->setText(order);
    
    return cell;
    
}

unsigned int ETableView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return sectionTitle.size();
}

unsigned int ETableView::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int ETableView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(size.height/sectionTitle.size());
}

unsigned int ETableView::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return _px(0);
}

unsigned int ETableView::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 0;
}
//-------------------------------------------------------------------------
//CDListView
CDListView::CDListView()
{
    tempList.clear();
    tempList.push_back("1");
    tempList.push_back("2");
    tempList.push_back("3");
    tempList.push_back("4");
    tempList.push_back("5");
    tempList.push_back("6");
    tempList.push_back("7");
    tempList.push_back("8");
    tempList.push_back("9");
    tempList.push_back("10");
    tempList.push_back("11");
    tempList.push_back("12");
    tempList.push_back("13");
    tempList.push_back("14");
    tempList.push_back("15");
    tempList.push_back("16");
    tempList.push_back("17");
    tempList.push_back("18");
    tempList.push_back("19");
    tempList.push_back("20");
}

CDListView::~CDListView()
{
    
}

bool CDListView::init(const CrossApp::DRect &rect)
{
    DSize size = rect.size;
    CAListView *listView = CAListView::createWithCenter(DRect(size.width/2,size.height/2,size.width,100));
    listView->setListViewDelegate(this);
    listView->setListViewDataSource(this);
    listView->setAllowsSelection(true);
    listView->setAllowsMultipleSelection(false);
    listView->setListViewOrientation(CAListViewOrientationHorizontal);
    //listView->setShowsHorizontalScrollIndicator(false);
    listView->setShowsScrollIndicators(false);

    listView->setSeparatorColor(CAColor_clear);
    this->addSubview(listView);
    return true;
}

CDListView* CDListView::createWithFrame(const CrossApp::DRect &rect)
{
    CDListView *pRet = new CDListView();
    if (pRet && pRet->init(rect))
    {
        pRet->setFrame(rect);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CDListView* CDListView::createWithCenter(const CrossApp::DRect &rect)
{
    CDListView *pRet = new CDListView();
    if (pRet && pRet->init(rect))
    {
        pRet->setCenter(rect);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void CDListView::listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index)
{
    
}

void CDListView::listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index)
{
    
}

unsigned int CDListView::numberOfIndex(CAListView *listView)
{
    return tempList.size();
}

unsigned int CDListView::listViewHeightForIndex(CAListView *listView, unsigned int index)
{
    return _px(150);
}

CAListViewCell* CDListView::listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index)
{
    DSize _size = cellSize;
    CAListViewCell* cell = (CAListViewCell*)listView->dequeueReusableCellWithIdentifier("ListViewCell");
    if (!cell)
    {
        cell = CAListViewCell::create("ListViewCell");
        CALabel* test = CALabel::createWithCenter(DRect(_size.width/2,
                                                            _size.height/2,
                                                            _size.width,
                                                            _size.height));
		test->setColor(ccc4(51, 204, 255, 255));
        test->setTextAlignment(CATextAlignmentCenter);
        test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        test->setFontSize(_px(28));
        test->setTag(100);
        cell->addSubview(test);
    }
    char temptext[10];
    sprintf(temptext, "cell-%d",index);
    CALabel* test = (CALabel*)cell->getSubviewByTag(100);
    test->setText(temptext);
    
    return cell;
}
//-------------------------------------------------------------------------

CDUIShowView::CDUIShowView()
:pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

CDUIShowView::~CDUIShowView()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void CDUIShowView::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    sectionTitle.clear();
    sectionTitle.push_back("A");
    sectionTitle.push_back("B");
    sectionTitle.push_back("C");
    sectionTitle.push_back("D");
    sectionTitle.push_back("E");
    sectionTitle.push_back("F");
    sectionTitle.push_back("G");
    sectionTitle.push_back("H");
    sectionTitle.push_back("I");
    sectionTitle.push_back("J");
    sectionTitle.push_back("K");
    sectionTitle.push_back("L");
    sectionTitle.push_back("M");
    sectionTitle.push_back("N");
    sectionTitle.push_back("O");
    sectionTitle.push_back("P");
    sectionTitle.push_back("Q");
    sectionTitle.push_back("R");
    sectionTitle.push_back("S");
    sectionTitle.push_back("T");
    sectionTitle.push_back("U");
    sectionTitle.push_back("V");
    sectionTitle.push_back("W");
    sectionTitle.push_back("X");
    sectionTitle.push_back("Y");
    sectionTitle.push_back("Z");
    
    showIndex = 0;
    dle_ren_index = 0;
    this->getView()->setColor(CAColor_gray);
    jsonTest();
}

void CDUIShowView::viewDidUnload()
{
    
}

void CDUIShowView::showUiWithIndex(int _index)
{
    UIINDEX = _index;
    switch (_index) {
        case 0:
            initButtonControl();
            showAlertView();
            break;
        case 1:
            initButtonControl();
            showButton();
            break;
        case 2:
            initButtonControl();
            showImage();
            break;
        case 3:
            showScale9Image();
            break;
        case 4:
            initButtonControl();
            showIndicator();
            break;
        case 5:
            initButtonControl();
            showLabel();
            break;
        case 6:
            showProgress();
            break;
        case 7:
            initButtonControl();
            showSlider();
            break;
        case 8:
            initButtonControl();
            showSwitch();
            break;
        case 9:
            showTextField();
            break;
        case 10:
            showTextView();
            break;
        case 11:
            initButtonControl();
            showSegmentedControl();
            break;
        case 12:
            initButtonControl();
            showTabBar();
            break;
        case 13:
            showPageView();
            break;
        case 14:
            showTableView();
            break;
        case 15:
            showListView();
            break;
        case 16:
            showCollectionView();
            break;
        case 17:
            showScrollView();
            break;
        case 18:
            showWebView();
            break;
        case 19:
            showFlashView();
            break;
        case 20:
            showGifView();
            break;
        case 21:
            initButtonControl();
            showDatePickerView();
            break;
        case 22:
            showStepper();
            break;
        case 23:
            showRenderImage();
            break;
        case 24:
            initButtonControl();
            showAnimation();
            break;
        default:
            break;
    }
}

void CDUIShowView::initButtonControl()
{
    CAButton* btn_l = CAButton::create(CAButtonTypeSquareRect);
    btn_l->setTag(1);
    btn_l->setCenter(DRect(winSize.width/2-80, winSize.height-50, 64, 64));
    btn_l->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_l->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_l->addTarget(this, CAControl_selector(CDUIShowView::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_l,10);
    
    CAButton* btn_r = CAButton::create(CAButtonTypeSquareRect);
    btn_r->setTag(2);
    btn_r->setCenter(DRect(winSize.width/2+80, winSize.height-50, 64, 64));
    btn_r->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_r->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_r->addTarget(this, CAControl_selector(CDUIShowView::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_r,10);
    
    p_PageViewVec = CAPageView::createWithFrame(DRect(0, 0, winSize.width, winSize.height-100), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackGroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    this->getView()->addSubview(p_PageViewVec);
}

void CDUIShowView::buttonControlCallBack(CrossApp::CAControl *btn, CrossApp::DPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    CCLog("btn_tag===%d",button->getTag());
    int temIndex = button->getTag();
    if (temIndex==1) {
        if (showIndex>0) {
            showIndex--;
            p_PageViewVec->setCurrPage(showIndex, true);
        }else if(showIndex==0){
            showIndex=showNum-1;
            p_PageViewVec->setCurrPage(showIndex, false);
        }
    }else if(temIndex==2){
        if (showIndex<showNum) {
            showIndex++;
            if (showIndex==showNum) {
                showIndex=0;
                p_PageViewVec->setCurrPage(showIndex, false);
            }else{
                p_PageViewVec->setCurrPage(showIndex, true);
            }
        }
    }
    switch (UIINDEX) {
        case 0:
            setAlertViewNavigationBarTitle();
            break;
        case 1:
            showButtonByIndex();
            break;
        case 2:
            showImageByIndex();
            break;
        case 4:
            showIndicatorByIndex();
            break;
        case 5:
            showLabelByIndex();
            break;
        case 7:
            showSliderByindex();
            break;
        case 8:
            showSwitchByIndex();
            break;
        case 9:
            showTextFieldByIndex();
            break;
        case 11:
            showSegmentedControlByIndex();
            break;
        default:
            break;
    }
    
}

void CDUIShowView::showAlertView()
{
    showNum = 3;
    pageViewIndex = 1;
    VIEWLIST.clear();
    for (int i=0; i<showNum; i++) {
        //初始化viewList
        char tem[10];
        sprintf(tem, "Click-%d",i+1);
        CAButton* btn = CAButton::create(CAButtonTypeRoundedRect);
        btn->setCenter(DRect(winSize.width/2, winSize.height/2, 100, 50));
        btn->setTitleForState(CAControlStateAll, tem);
        btn->addTarget(this, CAControl_selector(CDUIShowView::alertButtonCallBack), CAControlEventTouchUpInSide);
        
        CAView* view = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
        view->addSubview(btn);
        view->setColor(CAColor_gray);
        VIEWLIST.pushBack(view);
    }
    p_PageViewVec->setViews(VIEWLIST);
}

void CDUIShowView::setAlertViewNavigationBarTitle()
{
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("AlertView"));
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("AlertView1"));
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("AlertView2"));
    }
}

void CDUIShowView::alertButtonCallBack(CAControl* btn,DPoint point)
{
    if (showIndex==0) {
        CAAlertView* alertView = CAAlertView::createWithText("Alert", "this is a alert!", "close",NULL);
        alertView->show();
        alertView->setTarget(this, CAAlertView_selector(CDUIShowView::alertCallBack));
    }else if(showIndex==1){
        CAAlertView* alertView = CAAlertView::createWithText("Alert", "this is a alert!", "ok","close",NULL);
        alertView->show();
        alertView->setTarget(this, CAAlertView_selector(CDUIShowView::alertCallBack));
    }else if(showIndex==2){
        CAAlertView* alertView = CAAlertView::createWithText("Alert", "message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,!", "ok","close","button3","button4","button5","button6","button7",NULL);
        alertView->show();
        alertView->setTarget(this, CAAlertView_selector(CDUIShowView::alertCallBack));
    }
}

void CDUIShowView::alertCallBack(int btnIndex)
{
    CCLog("btnIndex==%d",btnIndex);
}

void CDUIShowView::showButton()
{
    showNum = 5;
    
    pageViewIndex = 1;
    VIEWLIST.clear();
    
    CAButton* btn1 = CAButton::create(CAButtonTypeCustom);
    btn1->setCenter(DRect(winSize.width/2, winSize.height/2, 100, 50));
    btn1->setTitleForState(CAControlStateNormal, "Noborder");
    btn1->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    btn1->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchUpInSide);
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(btn1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CAButton* btn2 = CAButton::create(CAButtonTypeSquareRect);
    btn2->setCenter(DRect(winSize.width/2, winSize.height/2, 200, 50));
    btn2->setTitleForState(CAControlStateAll,"SquareRect");
    btn2->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchUpInSide);
    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view2->addSubview(btn2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    CAButton* btn3 = CAButton::create(CAButtonTypeRoundedRect);
    btn3->setCenter(DRect(winSize.width/2, winSize.height/2, 200, 50));
    btn3->setTitleForState(CAControlStateAll, "RoundedRect");
    btn3->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchUpInSide);
    CAView* view3 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view3->addSubview(btn3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
    CAButton* btn4 = CAButton::create(CAButtonTypeSquareRect);
    btn4->setCenter(DRect(winSize.width/2, winSize.height/2, 200, 50));
    btn4->setTitleColorForState(CAControlStateNormal,CAColor_white);
    btn4->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/button_normal.png")));
    btn4->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/button_down.png")));
    btn4->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchDown);
    CAView* view4 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view4->addSubview(btn4);
    view4->setColor(CAColor_gray);
    VIEWLIST.pushBack(view4);
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setAllowsSelected(true);
    btn5->setCenter(DRect(winSize.width/2, winSize.height/2, 200, 50));
    btn5->setTitleForState(CAControlStateNormal, "Normal");
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setTitleForState(CAControlStateSelected, "Selected");
    btn5->setTitleForState(CAControlStateHighlighted, "Highlighted");
    btn5->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png")));
    btn5->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_highlighted.png")));
    btn5->setBackGroundViewForState(CAControlStateSelected, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png")));
    btn5->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchDown);
    CAView* view5 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view5->addSubview(btn5);
    view5->setColor(CAColor_gray);
    VIEWLIST.pushBack(view5);
    
    p_PageViewVec->setViews(VIEWLIST);
}

void CDUIShowView::showButtonByIndex()
{
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("Button Noborder"));
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("Button SquareRect"));
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("Button RoundedRect"));
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("Button Custom"));
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("Button AllowsSelected"));
    }
}

void CDUIShowView::buttonCallBack(CAControl* btn,DPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
}

void CDUIShowView::showImage()
{
    showNum = 7;
    pageViewIndex = 1;
    VIEWLIST.clear();
    
    CAImageView* image1 = CAImageView::createWithImage(CAImage::create("image/h1.png"));
    image1->setFrame(this->getView()->getBounds());
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(image1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CAImageView* image2 = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,400,400));
    image2->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    image2->setImage(CAImage::create("image/h1.png"));
    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view2->addSubview(image2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    CAImageView* image3 = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,400,400));
    image3->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    image3->setImage(CAImage::create("image/h1.png"));
    CAView* view3 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view3->addSubview(image3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
    CAImageView* image4 = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,400,400));
    image4->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    image4->setImage(CAImage::create("image/h1.png"));
    CAView* view4 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view4->addSubview(image4);
    view4->setColor(CAColor_gray);
    VIEWLIST.pushBack(view4);
    
    CAImageView* image5 = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,400,400));
    image5->setImageViewScaleType(CAImageViewScaleTypeFitViewByVertical);
    image5->setImage(CAImage::create("image/h1.png"));
    CAView* view5 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view5->addSubview(image5);
    view5->setColor(CAColor_gray);
    VIEWLIST.pushBack(view5);
    
    CAImageView* image6 = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,400,400));
    image6->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    image6->setImage(CAImage::create("image/h1.png"));
    CAView* view6 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view6->addSubview(image6);
    view6->setColor(CAColor_gray);
    VIEWLIST.pushBack(view6);
    
    CAVector<CAImage* > img;
    for (int i=0; i<8; i++) {
        char temp[50];
        sprintf(temp, "animation/npc_382-%d.png",i+1);
        CAImage* im = CAImage::create(temp);
        img.pushBack(im);
    }
    CAImageView* image7 = CAImageView::createWithImage(CAImage::create("animation/npc_382-1.png"));
    image7->setCenter(DRect(winSize.width/2,winSize.height/2,40,88));
    image7->setAnimationImages(img);
    image7->setAnimationDuration(0.2);
    image7->startAnimating();
    CAView* view7 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view7->addSubview(image7);
    view7->setColor(CAColor_gray);
    VIEWLIST.pushBack(view7);
    
    p_PageViewVec->setViews(VIEWLIST);
}

void CDUIShowView::showImageByIndex()
{
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("ImageView"));
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageXY"));
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageCrop"));
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageInside"));
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitViewByHorizontal"));
    }else if(showIndex==5){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitViewByVertical"));
    }else if(showIndex==6){
        this->setNavigationBarItem(CANavigationBarItem::create("Frame animation"));
    }
}

void CDUIShowView::showScale9Image()
{
    CAScale9ImageView* s9image = CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_normal.png"));
    s9image->setCenter(DRect(winSize.width/2,winSize.height/2,200,100));
    this->getView()->addSubview(s9image);
}

void CDUIShowView::showIndicator()
{
    showNum = 5;
    pageViewIndex = 1;
    VIEWLIST.clear();
    
    CAActivityIndicatorView* idc1 = CAActivityIndicatorView::createWithFrame(DRect(0, 0, winSize.width, winSize.height));
    idc1->setStyle(CAActivityIndicatorViewStyleWhiteLarge);
    idc1->startAnimating();
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(idc1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CAActivityIndicatorView* idc2 = CAActivityIndicatorView::createWithFrame(DRect(0, 0, winSize.width, winSize.height));
    idc2->setStyle(CAActivityIndicatorViewStyleGrayLarge);
    idc2->startAnimating();
    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view2->addSubview(idc2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    CAActivityIndicatorView* idc3 = CAActivityIndicatorView::createWithFrame(DRect(0, 0, winSize.width, winSize.height));
    idc3->setStyle(CAActivityIndicatorViewStyleWhite);
    idc2->setCycleTime(1.0f);
    idc2->setTimesOneCycle(12);
    idc2->startAnimating();
    CAView* view3 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view3->addSubview(idc3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
    CAActivityIndicatorView* idc4 = CAActivityIndicatorView::createWithFrame(DRect(0, 0,
                                                                                       winSize.width, winSize.height));
    idc4->setStyle(CAActivityIndicatorViewStyleGray);
    idc4->setCycleTime(1.0f);
    idc4->setTimesOneCycle(12);
    idc4->startAnimating();
    CAView* view4 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view4->addSubview(idc4);
    view4->setColor(CAColor_gray);
    VIEWLIST.pushBack(view4);
    
    
    CAImage* image = CAImage::create("image/indicator2.png");
    CAImageView* imageView = CAImageView::createWithCenter(DRect(0, 0, 100, 100));
    imageView->setImage(image);
    CAActivityIndicatorView* idc5 = CAActivityIndicatorView::createWithCenter(DRect(winSize.width/2, winSize.height/2,
                                                                                       winSize.width, winSize.height));
    idc5->setStyle(CAActivityIndicatorViewStyleImage);
    idc5->setActivityIndicatorView(imageView);
    idc5->startAnimating();
    CAView* view5 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view5->addSubview(idc5);
    view5->setColor(CAColor_gray);
    VIEWLIST.pushBack(view5);
    
    p_PageViewVec->setViews(VIEWLIST);
    
}

void CDUIShowView::showIndicatorByIndex()
{
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView"));
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView1"));
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView2"));
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView3"));
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView4"));
    }
}

void CDUIShowView::showLabel()
{
    showNum = 6;
    pageViewIndex = 1;
    VIEWLIST.clear();
    for (int i=0; i<showNum; i++) {
        CALabel* label = CALabel::createWithCenter(DRect(winSize.width/2, winSize.height/2, 300, 40));

		std::string s = "Hello World";

		std::u32string c;
		StringUtils::UTF8ToUTF32(s, c);
		c += 0x1F604;

		std::string cc;
		StringUtils::UTF32ToUTF8(c, cc);

        label->setText(cc);
        label->setColor(CAColor_red);
        label->setFontName("c:/x.ttf");
        label->setFontSize((_px(36)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        if (i==1) {
            label->setUnderLine(true);
        }else if(i==2){
            label->setBold(true);
        }else if(i==3){
            label->setItalics(true);
        }else if(i==4){
            label->setText("Enable Copy");
            label->setEnableCopy(true);
        }else if(i==5){
            label->setText("Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World");
            label->setUnderLine(true);
            label->setBold(true);
            label->setItalics(true);
            label->setLineSpacing(20);
            label->setCenter(DRect(winSize.width/2, winSize.height/2, winSize.width, 200));
        }
        
        CAView* view = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
        view->addSubview(label);
        view->setColor(CAColor_gray);
        VIEWLIST.pushBack(view);
    }
    p_PageViewVec->setViews(VIEWLIST);
}

void CDUIShowView::showLabelByIndex()
{

}

void CDUIShowView::showProgress()
{
    progress_value = CALabel::createWithCenter(DRect(winSize.width/2, winSize.height/4, 200, 50));
    progress_value->setTag(100);
    progress_value->setText("");
    progress_value->setFontSize(_px(30));
	progress_value->setColor(ccc4(51, 204, 255, 255));
    progress_value->setTextAlignment(CATextAlignmentCenter);
    progress_value->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->getView()->addSubview(progress_value);
    
    progress = CAProgress::create();
    progress->setCenter(DRect(winSize.width/2, winSize.height/2, winSize.width/2, 0));
    CAScheduler::schedule(schedule_selector(CDUIShowView::progressValueChange), this, 0.01, false);
    
    this->getView()->addSubview(progress);
    showNum = 1;
}

void CDUIShowView::progressValueChange(float _t)
{
    char valueText[20] = "";
    float currentValue = progress->getProgress() + 0.001;
    progress->setProgress(currentValue);
    sprintf(valueText,"%.02f%%",currentValue*100-0.1);
    progress_value->setText(valueText);

}

void CDUIShowView::showSlider()
{
    showNum = 2;
    pageViewIndex = 1;
    VIEWLIST.clear();
    
    sliderValue1 = CALabel::createWithCenter(DRect(winSize.width/2, winSize.height/4, 200, 50));
    sliderValue1->setColor(ccc4(51,204,255,255));
    sliderValue1->setText("0");
    sliderValue1->setFontSize(_px(30));
    sliderValue1->setTextAlignment(CATextAlignmentCenter);
    sliderValue1->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    
    CASlider* slider1 = CASlider::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width/2,56));
    slider1->addTarget(this, CAControl_selector(CDUIShowView::sliderValueChange));
    slider1->setTag(100);
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(slider1);
    view1->addSubview(sliderValue1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    sliderValue2 = CALabel::createWithCenter(DRect(winSize.width/2, winSize.height/4, 200, 50));
	sliderValue2->setColor(ccc4(51, 204, 255, 255));
    sliderValue2->setText("0");
    sliderValue2->setFontSize(_px(30));
    sliderValue2->setTextAlignment(CATextAlignmentCenter);
    sliderValue2->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    
    CASlider* slider2 = CASlider::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width/2,56));
    slider2->setTag(101);
    slider2->setMaxTrackTintImage(CAImage::create("source_material/ex1.png"));
    slider2->setMinTrackTintImage(CAImage::create("source_material/ex3.png"));
    slider2->setThumbTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
    slider2->addTarget(this, CAControl_selector(CDUIShowView::sliderValueChange));
    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    //view2->addSubview(slider2);
    view2->addSubview(sliderValue2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    p_PageViewVec->setViews(VIEWLIST);
}

void CDUIShowView::showSliderByindex()
{
    this->getView()->removeSubview(slider);
    if (showIndex==0)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Slider"));
    }
    else if(showIndex==1)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Slider Custem"));
    }
}

void CDUIShowView::sliderValueChange(CAControl* btn, DPoint point)
{
    char value[20] = "";
    CASlider* p_Slider = (CASlider*)btn;
    sprintf(value, "%.02f%%", p_Slider->getValue() * 100);
    if (p_Slider->getTag()==100) {
        sliderValue1->setText(value);
    }else if(p_Slider->getTag()==101)
    {
        sliderValue2->setText(value);
    }
    
}

void CDUIShowView::showSwitch()
{
    showNum = 2;
    pageViewIndex = 1;
    VIEWLIST.clear();
    
    CASwitch* customSwitch1 = CASwitch::createWithCenter(DRect(winSize.width/2, winSize.height/2, 100, 20));
    customSwitch1->setTag(100);
    customSwitch1->setIsOn(true, false);
    customSwitch1->addTarget(this, CAControl_selector(CDUIShowView::switchStateChange));
    
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(customSwitch1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CASwitch* customSwitch2 = CASwitch::createWithCenter(DRect(winSize.width/2, winSize.height/2, 100, 20));
    customSwitch2->setTag(101);
    customSwitch2->setIsOn(true, false);
    customSwitch2->setOnImage(CAImage::create("image/Switch_on.png"));
    customSwitch2->setOffImage(CAImage::create("image/Switch_off.png"));
    customSwitch2->addTarget(this, CAControl_selector(CDUIShowView::switchStateChange));
    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view2->addSubview(customSwitch2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    p_PageViewVec->setViews(VIEWLIST);
}

void CDUIShowView::showSwitchByIndex()
{
    if (showIndex==0)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Switch"));
    }
    else if(showIndex==1)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Switch Custem"));
    }
}

void CDUIShowView::switchStateChange(CAControl* btn, DPoint point)
{
    CASwitch* state = (CASwitch*)btn;
    if (!state->isOn())
    {
        CCLog("switchStateChange false");
    }
    else
    {
        CCLog("switchStateChange true");
    }
}

void CDUIShowView::showTextField()
{
    CATextField* textField = CATextField::createWithFrame(DRect(100, 300, winSize.width-200, 80));
    textField->setFontSize(_px(40));
    textField->setTag(100);
    textField->setPlaceHolder("Input");
    textField->setKeyboardType(KEY_BOARD_TYPE_NORMAL);
    this->getView()->addSubview(textField);
    showNum = 1;
}

void CDUIShowView::showTextFieldByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex == 0)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("TextField"));
        CATextField* textField = CATextField::createWithFrame(DRect(100, 150, winSize.width-200, 80));
        textField->setFontSize(_px(40));
        textField->setTag(100);
        textField->setPlaceHolder("Input");
        this->getView()->addSubview(textField);
    }
    else if(showIndex == 1)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("TextField Custem"));
        CATextField* textField = CATextField::createWithFrame(DRect(100, 150, winSize.width-200, 80));
        textField->setFontSize(_px(40));
        textField->setTag(100);
        textField->setPlaceHolder("Input");
        textField->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_highlighted.png")));
        this->getView()->addSubview(textField);
    }
}

void CDUIShowView::showTextView()
{
    CATextView* textView = CATextView::createWithFrame(DRect(100, 150, winSize.width-200, 300));
    textView->setPlaceHolder("TextView");
    //textView->setSpaceHolderColor(CAColor_blueStyle);
    //textView->setTextColor(CAColor_green);
    //textView->setCursorColor(CAColor_red);
    textView->setFontSize(_px(40));
    textView->setSpaceHolderColor(CAColor_blue);
    //textView->setWordWrap(false);
    //textView->setLineSpacing(50);
    //textView->setFontColor(CAColor_gray);
    //textView->setBackGroundColor(CAColor_blueStyle);
	//textView->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/textField_bg.png")));
    //textView->setBackGroundImage(CAImage::create("source_material/btn_square_selected.png"));
    this->getView()->addSubview(textView);
    
    CATextField* textField = CATextField::createWithFrame(DRect(100, 450, winSize.width-200, 80));
    textField->setFontSize(_px(40));
    textField->setTag(100);
    textField->setPlaceHolder("Input");
    textField->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_highlighted.png")));
    this->getView()->addSubview(textField);
}

void CDUIShowView::showSegmentedControl()
{
    showNum = 2;
    pageViewIndex = 1;
    VIEWLIST.clear();
    
    CASegmentedControl* segment1 = CASegmentedControl::createWithCenter(DRect(winSize.width/2,
                                                             winSize.height/2,
                                                             winSize.width/2,
                                                             50), 3);

    char temstr[20];
    for (int i = 0; i < 3; i++)
    {
        sprintf(temstr, "Page%d", i + 1);
        segment1->setTitleForSegmentAtIndex(temstr, i);
    }
    segment1->setTitleColor(CAColor_black);
    segment1->setTitleSelectedColor(CAColor_white);
    segment1->addTarget(this, CASegmentedControl_selector(CDUIShowView::segmentCallback));
    segment1->setSelectedAtIndex(0);
    
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(segment1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CASegmentedControl* segment2 = CASegmentedControl::createWithCenter(DRect(winSize.width/2,
                                                                                 winSize.height/2,
                                                                                 winSize.width/2,
                                                                                 50), 3);
    char temstr2[20];
    for (int i = 0; i < 3; i++)
    {
        sprintf(temstr2, "Page%d", i + 1);
        segment2->setTitleForSegmentAtIndex(temstr2, i);
    }
    segment2->setTitleColor(CAColor_black);
    segment2->setTitleSelectedColor(CAColor_white);
    
    segment2->addTarget(this, CASegmentedControl_selector(CDUIShowView::segmentCallback));
    segment2->setBackgroundImage(CAImage::create("image/seg1.png"));
    segment2->setSegmentItemBackgroundImage(CAImage::create("image/seg2.png"));
    segment2->setTintColor(ccc4(44, 178, 93, 255));
    segment2->setSelectedAtIndex(0);
    
    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view2->addSubview(segment2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    p_PageViewVec->setViews(VIEWLIST);

}

void CDUIShowView::showSegmentedControlByIndex()
{
    if (showIndex==0)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Segment"));
    }
    else if(showIndex==1)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Segment Custem"));
    }
}

void CDUIShowView::segmentCallback(CASegmentedControl* btn, int index)
{
    CCLog("segmentCallback");
}

void CDUIShowView::showTabBar()
{
    showNum = 2;
    pageViewIndex = 1;
    VIEWLIST.clear();
    
    CATabBarItem* item1 = CATabBarItem::create("item1", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item2 = CATabBarItem::create("item2", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item3 = CATabBarItem::create("item3", CAImage::create(""), CAImage::create(""));
    
    std::vector<CATabBarItem*> item;
    item.push_back(item1);
    item.push_back(item2);
    item.push_back(item3);
    
    CATabBar* tabBar = CATabBar::create(item,DSize(winSize.width, 100));
    tabBar->setFrameOrigin(DPoint(0, winSize.height-200));
    tabBar->showSelectedIndicator();
//    tabBar->setTitleColorForNormal(CAColor_yellow);
//    tabBar->setTitleColorForSelected(CAColor_orange);
    tabBar->setTag(100);
    tabBar->setSelectedAtIndex(0);
    
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(tabBar);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CATabBarItem* item4 = CATabBarItem::create("", CAImage::create("image/tab_news_btn_up.png"), CAImage::create("image/tab_news_btn_down.png"));
    CATabBarItem* item5 = CATabBarItem::create("", CAImage::create("image/tab_image_btn_up.png"), CAImage::create("image/tab_image_btn_down.png"));
    CATabBarItem* item6 = CATabBarItem::create("", CAImage::create("image/tab_mine_btn_up.png"), CAImage::create("image/tab_mine_btn_down.png"));
    
    std::vector<CATabBarItem*> it1;
    it1.push_back(item4);
    it1.push_back(item5);
    it1.push_back(item6);
    
    CATabBar* tabBar1 = CATabBar::create(it1,DSize(winSize.width, 100));
    tabBar1->setFrameOrigin(DPoint(0, winSize.height-200));
    tabBar1->setTitleColorForNormal(CAColor_yellow);
    tabBar1->setTitleColorForSelected(CAColor_orange);
    tabBar1->setBackGroundImage(CAImage::create("image/tab_news_bg.png"));
    tabBar1->setSelectedBackGroundColor(CAColor_clear);
    tabBar1->setTag(100);
    tabBar1->setSelectedAtIndex(0);
    
    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view2->addSubview(tabBar1);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    p_PageViewVec->setViews(VIEWLIST);

}

void CDUIShowView::showPageView()
{
    CAImageView* view1 = CAImageView::createWithImage(CAImage::create("image/1.jpg"));
    CAImageView* view2 = CAImageView::createWithImage(CAImage::create("image/2.jpg"));
    CAImageView* view3 = CAImageView::createWithImage(CAImage::create("image/3.jpg"));
    CAVector<CAView*> _view;
    _view.pushBack(view1);
    _view.pushBack(view2);
    _view.pushBack(view3);
    
    p_pageView = CAPageView::createWithCenter(DRect(winSize.width/2, winSize.height/2, winSize.width, winSize.height), CAPageViewDirectionHorizontal);
    p_pageView->setViews(_view);
    p_pageView->setPageViewDelegate(this);
    this->getView()->addSubview(p_pageView);
    
    pageControl = CAPageControl::createWithCenter(DRect(winSize.width/2, 50, 100, 50));
    pageControl->setNumberOfPages(_view.size());
    pageControl->setPageIndicatorTintColor(CAColor_gray);
    pageControl->setCurrentPageIndicatorTintColor(CAColor_green);
    pageControl->addTarget(this, CAControl_selector(CDUIShowView::pageControlCallBack));
    this->getView()->insertSubview(pageControl,20);
}
void CDUIShowView::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void CDUIShowView::pageViewDidEndTurning(CAPageView* pageView)
{
    if (pageViewIndex==0) {
        pageControl->setCurrentPage(pageView->getCurrPage());
        pageControl->updateCurrentPageDisplay();
    }else{
        
    }

}

void CDUIShowView::pageControlCallBack(CrossApp::CAControl *btn, CrossApp::DPoint point){
    CAPageControl* button = (CAPageControl*)btn;
    CCLog("btn_tag===%d",button->getCurrentPage());
    p_pageView->setCurrPage(button->getCurrentPage(), true);
}
void CDUIShowView::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{

}

void CDUIShowView::reshapeViewRectDidFinish()
{
    
}
//TableView
void CDUIShowView::showTableView()
{
    p_TableView = CATableView::createWithFrame(DRect(0, 0, winSize.width-50, winSize.height));
    p_TableView->setTableViewDataSource(this);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setAllowsSelection(true);
    p_TableView->setAllowsMultipleSelection(false);
    p_TableView->setShowsScrollIndicators(false);
    p_TableView->setSelectRowAtIndexPath(2, 1);
    this->getView()->addSubview(p_TableView);
    
    
    ETableView* p_TableView1 = ETableView::createWithFrame(DRect(winSize.width-50, 0, 50, winSize.height));
    p_TableView1->setETableViewDelegate(this);
    this->getView()->addSubview(p_TableView1);
}

void CDUIShowView::etableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    //CCLog("ssss====%d",row);
    p_TableView->setContentOffset(DPoint(0,313*row), false);
}

void CDUIShowView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

void CDUIShowView::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDUIShowView::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    CATableViewCell* cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CATableViewCell::create("CrossApp");
        CAImageView* image = CAImageView::createWithCenter(DRect(50, _size.height/2, 96, 96));
        image->setImage(CAImage::create("source_material/second_2.png"));
        cell->addSubview(image);
        
        CALabel* cellText = CALabel::createWithCenter(DRect(200, _size.height/2, _size.width/2, _size.height));
        cellText->setTag(100);
        cellText->setFontSize(_px(30));
        cellText->setTextAlignment(CATextAlignmentCenter);
        cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        cell->addSubview(cellText);
    }
    char order[20] = "";
    sprintf(order, "%s-%d", sectionTitle.at(section).c_str(),row);
    CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
    cellText->setText(order);
    return cell;
    
}

CAView* CDUIShowView::tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    std::string head = sectionTitle.at(section);
    CAView* view = CAView::createWithColor(CAColor_gray);
    
    DSize _size = viewSize;
    CALabel* header = CALabel::createWithCenter(DRect(50, _size.height/2, _size.width/2, _size.height));
    header->setText(head);
    header->setFontSize(_px(30));
	header->setColor(CAColor_white);
    header->setTextAlignment(CATextAlignmentCenter);
    header->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    view->addSubview(header);
    
    return view;
}

CAView* CDUIShowView::tableViewSectionViewForFooterInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor_gray);
    return view;
}

unsigned int CDUIShowView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 2;
}

unsigned int CDUIShowView::numberOfSections(CATableView *table)
{
    return sectionTitle.size();
}

unsigned int CDUIShowView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(130);
}

unsigned int CDUIShowView::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return _px(50);
}

unsigned int CDUIShowView::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 1;
}

void CDUIShowView::showListView()
{
    CDListView* listview1 = CDListView::createWithFrame(DRect(0,0,winSize.width,100));
    this->getView()->addSubview(listview1);
    
    p_ListView = CAListView::createWithFrame(DRect(0,100,winSize.width,winSize.height-100));
    p_ListView->setListViewDelegate(this);
    p_ListView->setListViewDataSource(this);
    p_ListView->setAllowsSelection(true);
    p_ListView->setAllowsMultipleSelection(false);
    p_ListView->setListViewOrientation(CAListViewOrientationVertical);
    p_ListView->setShowsScrollIndicators(true);
    
    p_ListView->setSeparatorColor(CAColor_gray);
    this->getView()->addSubview(p_ListView);
}

void CDUIShowView::listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index)
{
    
}

void CDUIShowView::listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index)
{
    
}

unsigned int CDUIShowView::numberOfIndex(CAListView *listView)
{
    return 30;
}

unsigned int CDUIShowView::listViewHeightForIndex(CAListView *listView, unsigned int index)
{
    return _px(100);
}

CAListViewCell* CDUIShowView::listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index)
{
    DSize _size = cellSize;
    CAListViewCell* cell = (CAListViewCell*)listView->dequeueReusableCellWithIdentifier("ListViewCell");
    if (cell==NULL)
    {
        cell = CAListViewCell::create("ListViewCell");
        
        CALabel* test = CALabel::createWithCenter(DRect(100,
                                                            _size.height/2,
                                                            _size.width,
                                                            _size.height));
		test->setColor(ccc4(51, 204, 255, 255));
        test->setTextAlignment(CATextAlignmentCenter);
        test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        test->setFontSize(_px(28));
        test->setTag(100);
        cell->addSubview(test);
        
        CAButton* btn = CAButton::createWithCenter(DRect(_size.width-100, _size.height/2, 100, 50), CAButtonTypeSquareRect);
        btn->setTitleForState(CAControlStateNormal, "btn");
        btn->setTag(200);
        cell->addSubview(btn);
        btn->setTouchEventScrollHandOverToSuperview(false);
    }
    char temptext[10];
    sprintf(temptext, "cell-%d",index);
    CALabel* test = (CALabel*)cell->getSubviewByTag(100);
    test->setText(temptext);

    return cell;
}

void CDUIShowView::showWebView()
{
    CAWebView* web = CAWebView::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width,winSize.height/2));
    web->loadURL("http://crossapp.9miao.com");
    this->getView()->addSubview(web);
}


void CDUIShowView::showCollectionView()
{
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    
    headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    
    p_Conllection = CACollectionView::createWithFrame(DRect(0, 0, winSize.width, winSize.height));
    p_Conllection->setAllowsSelection(true);
    //p_Conllection->setAllowsMultipleSelection(true);
    p_Conllection->setCollectionViewDelegate(this);
    p_Conllection->setCollectionViewDataSource(this);
    p_Conllection->setScrollViewDelegate(this);
    p_Conllection->setHeaderRefreshView(headerRefreshView);
    p_Conllection->setFooterRefreshView(footerRefreshView);
    p_Conllection->setHoriInterval(_px(40));
    p_Conllection->setVertInterval(_px(40));
    this->getView()->addSubview(p_Conllection);
}

void CDUIShowView::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    
}

void CDUIShowView::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    
}

CACollectionViewCell* CDUIShowView::collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    if (row * 3 + item >= colorArr.size())
    {
        return NULL;
    }
    
    DSize _size = cellSize;
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithFrame(DRect(0, 0, _size.width, _size.height));
        itemImage->setTag(99);
        p_Cell->addSubview(itemImage);
        
        DSize itemSize = itemImage->getBounds().size;
        CALabel* itemText = CALabel::createWithCenter(DRect(itemSize.width/2, itemSize.height/2, 150, 40));
        itemText->setTag(100);
        itemText->setFontSize(_px(29));
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        itemImage->addSubview(itemText);
    }
    CAView* itemImageView = p_Cell->getSubviewByTag(99);
    itemImageView->setColor(colorArr.at(row * 3 + item));
    CCLog("%d", row * 3 + item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d,%d)", section, row, item);
    CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(99)->getSubviewByTag(100);
    itemText->setText(pos);
    
    return p_Cell;
}

unsigned int CDUIShowView::numberOfSections(CACollectionView *collectionView)
{
    return 1;
}

unsigned int CDUIShowView::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    return colorArr.size() % 3 == 0 ? colorArr.size() / 3 : colorArr.size() / 3 + 1;
}

unsigned int CDUIShowView::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return 3;
}

unsigned int CDUIShowView::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    return (this->getView()->getBounds().size.width - _px(40) * 4) / 3;
}

void CDUIShowView::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    colorArr.clear();
    CAScheduler::schedule(schedule_selector(CDUIShowView::refreshData), this, 0.1, 0, CCRANDOM_0_1() * 2, false);
}

void CDUIShowView::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(CDUIShowView::refreshData), this, 0.1, 0, CCRANDOM_0_1() * 2, false);
}

void CDUIShowView::refreshData(float interval)
{
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    p_Conllection->reloadData();
}

void CDUIShowView::showScrollView()
{
    p_ScrollView = CAScrollView::createWithFrame(DRect(0,0,winSize.width,winSize.height));
    p_ScrollView->setMinimumZoomScale(0.2f);
    p_ScrollView->setMaximumZoomScale(5.0f);
    this->getView()->addSubview(p_ScrollView);
    
    DSize _size = p_ScrollView->getBounds().size;
    p_imageView = CAImageView::createWithImage(CAImage::create("image/h1.png"));
    p_imageView->setCenter(DRect(_size.width/2, _size.height/2,800,1200));
    p_ScrollView->addSubview(p_imageView);

}

void CDUIShowView::showFlashView()
{
	//http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4 
	//mnt/sdcard/video.mp4
	std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("image/video.mp4");// image / 11.avi");// video.mp4");
	//CAVideoPlayerController* pv = CAVideoPlayerController::createWithPath(path.c_str(), "asdas");
	//CAVideoPlayerController* pv = CAVideoPlayerController::createWithUrl("http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4", "asdas");

	DRect frame = getView()->getFrame();

	//frame.origin.x = frame.size.width / 2;
	//frame.origin.y = frame.size.height / 2;

//	CAView* pView = CAView::createWithFrame(frame);
//	pView->setColor(ccc4(255, 0, 255, 255));
	CAVideoPlayerControlView* pView = CAVideoPlayerControlView::createWithFrame(frame);
//	pView->setColor(ccc4(255, 0, 255, 255));
//    pView->initWithUrl("http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4");
	pView->initWithPath(path);
//	pView->initWithPath("D:\\XiGua Yingshi\\x.rmvb", false);
	this->getView()->addSubview(pView);
//	pView->play();

	return;

    CAFlashView* swfBg = CAFlashView::createWithFlash(CrossApp::extension::CAFlash::createWithFilePath("image/swfWK.swf"));
    swfBg->setFrame(DRect(100, 100, winSize.width/2, winSize.height/2));
    swfBg->runAnimation();
    swfBg->setRepeatForever(true);
    this->getView()->addSubview(swfBg);
}

void CDUIShowView::showDatePickerView()
{
    
    pageViewIndex = 1;
    VIEWLIST.clear();
    
    p_pickerView = CAPickerView::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width,280));
    p_pickerView->setPickerViewDelegate(this);
    p_pickerView->setPickerViewDataSource(this);
    p_pickerView->setFontSizeNormal(_px(40));
    p_pickerView->setFontSizeSelected(_px(40));
    p_pickerView->reloadAllComponents();
    
    city_value = CALabel::createWithFrame(DRect(0, 100, winSize.width, 40));
    city_value->setText(UTF8("天津市"));
	city_value->setColor(CAColor_black);
    city_value->setFontSize((_px(28)));
    city_value->setTextAlignment(CATextAlignmentCenter);
    city_value->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(p_pickerView);
    view1->addSubview(city_value);
    view1->setColor(CAColor_white);
    VIEWLIST.pushBack(view1);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    
    CADatePickerView* dpv = CADatePickerView::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width,280),CADatePickerModeDate);
    //dpv->setDate(2004, 2, 16, false);
    dpv->setDelegate(this);

    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view2->addSubview(dpv);
    view2->setColor(CAColor_white);
    VIEWLIST.pushBack(view2);
    
    CADatePickerView* dpv3 = CADatePickerView::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width,280),CADatePickerModeDateAndTime);
    dpv3->setDelegate(this);
    
    CAView* view3 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view3->addSubview(dpv3);
    view3->setColor(CAColor_white);
    VIEWLIST.pushBack(view3);
    
    CADatePickerView* dpv4 = CADatePickerView::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width,280),CADatePickerModeTime);
    dpv4->setDelegate(this);
    
    CAView* view4 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view4->addSubview(dpv4);
    view4->setColor(CAColor_white);
    VIEWLIST.pushBack(view4);
    
    CADatePickerView* dpv5 = CADatePickerView::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width,280),CADatePickerModeCountDownTimer);
    dpv5->setDelegate(this);
    
    CAView* view5 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view5->addSubview(dpv5);
    view5->setColor(CAColor_white);
    VIEWLIST.pushBack(view5);
#endif
	p_PageViewVec->setViews(VIEWLIST);
	showNum = VIEWLIST.size();
}

void CDUIShowView::didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    char tem[100];
	sprintf(tem, "%s", unicode_to_utf8(adressTag[row]).c_str());
    city_value->setText(tem);
}

void CDUIShowView::didSelectRow(const struct tm& tm)
{
    char temp_time[20];
    strftime(temp_time, 100, "%F", &tm);
    CCLog("didSelectRow----tm======%d-%d-%d",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday);
    CCLog("didSelectRow----tm======%s",temp_time);
}

unsigned int CDUIShowView::numberOfComponentsInPickerView(CAPickerView* pickerView)
{
    return 1;
}

unsigned int CDUIShowView::numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component)
{
    return 34;
}

float CDUIShowView::widthForComponent(CAPickerView* pickerView, unsigned int component)
{
    return p_pickerView->getFrame().size.width/numberOfComponentsInPickerView(pickerView);
}

float CDUIShowView::rowHeightForComponent(CAPickerView* pickerView, unsigned int component)
{
    static float rowHeight = 80;
    return rowHeight;
}

const char* CDUIShowView::titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    return unicode_to_utf8(adressTag[row]).c_str();
}

void CDUIShowView::zoomViewBySliderValue(CrossApp::CAControl *btn, CrossApp::DPoint point)
{

}

void CDUIShowView::showAnimation()
{
    showNum = 3;
    pageViewIndex = 1;
    VIEWLIST.clear();
    //Animation 1
    heart_index = 0;
    CAButton* btn1 = CAButton::create(CAButtonTypeRoundedRect);
    btn1->setCenter(DRect(winSize.width/2, winSize.height/4, 200, 50));
    btn1->setTitleForState(CAControlStateNormal, "Play Animation");
    btn1->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    btn1->setTag(100);
    btn1->addTarget(this, CAControl_selector(CDUIShowView::doAction), CAControlEventTouchUpInSide);
    animation_1_view = CAImageView::createWithCenter(DRect(winSize.width/2, winSize.height/2, 28, 24));
    animation_1_view->setImage(CAImage::create("image/heart1.png"));
    
    CAView* view1 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view1->addSubview(btn1);
    view1->addSubview(animation_1_view);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);

    //Animation 2
    CAImageView* bg = CAImageView::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width,80));
    bg->setImage(CAImage::create("image/navbg.jpg"));
    
    animation_2_textfield = CATextField::createWithFrame(DRect(90,winSize.height/2-25,100,50));
    animation_2_textfield->setFontSize(_px(40));
    animation_2_textfield->setKeyboardType(KEY_BOARD_TYPE_NORMAL);
    animation_2_textfield->setVisible(false);
    
    animation_2_btn_search = CAButton::createWithCenter(DRect(70,winSize.height/2,56,48), CAButtonTypeCustom);
    animation_2_btn_search->setImageForState(CAControlStateNormal, CAImage::create("image/search_btn.png"));
    animation_2_btn_search->addTarget(this, CAControl_selector(CDUIShowView::doAction), CAControlEventTouchUpInSide);
    animation_2_btn_search->setTag(201);
    
    animation_2_btn_cancel = CAButton::createWithCenter(DRect(winSize.width-60,winSize.height/2,100,50), CAButtonTypeCustom);
    animation_2_btn_cancel->setTitleForState(CAControlStateNormal, "Cancel");
    animation_2_btn_cancel->setTag(202);
    animation_2_btn_cancel->setTitleColorForState(CAControlStateNormal, CAColor_white);
    animation_2_btn_cancel->addTarget(this, CAControl_selector(CDUIShowView::doAction), CAControlEventTouchUpInSide);
    animation_2_btn_cancel->setVisible(false);
    
    CAView* view2 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view2->addSubview(bg);
    view2->addSubview(animation_2_textfield);
    view2->addSubview(animation_2_btn_search);
    view2->addSubview(animation_2_btn_cancel);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    //Animation 3
    animation_3_imageview = CAImageView::createWithFrame(DRect(0,0,winSize.width,winSize.height));
    animation_3_imageview->setImage(CAImage::create("image/2.jpg"));
    
    CAButton* btn3 = CAButton::create(CAButtonTypeRoundedRect);
    btn3->setCenter(DRect(winSize.width/2, winSize.height/4, 200, 50));
    btn3->setTitleForState(CAControlStateNormal, "Play Animation");
    btn3->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    btn3->setTag(300);
    btn3->addTarget(this, CAControl_selector(CDUIShowView::doAction), CAControlEventTouchUpInSide);
    CAView* view3 = CAView::createWithFrame(DRect(0,0,winSize.width,winSize.height-100));
    view3->addSubview(animation_3_imageview);
    view3->addSubview(btn3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
    p_PageViewVec->setViews(VIEWLIST);
}

void CDUIShowView::doAction(CAControl* btn,DPoint point)
{
    CAButton* button = (CAButton*)btn;
    int tag = button->getTag();
    
    if (tag==100) {
        animation_1_view->setScale(1);
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        CAViewAnimation::setAnimationRepeatAutoreverses(true);
        if (heart_index==0) {
            animation_1_view->setImage(CAImage::create("image/heart2.png"));
            heart_index = 1;
        }else{
            animation_1_view->setImage(CAImage::create("image/heart1.png"));
            heart_index = 0;
        }
        animation_1_view->setScale(2);
        CAViewAnimation::commitAnimations();
    }else if (tag==201) {
        animation_2_textfield->setVisible(true);
        animation_2_btn_cancel->setVisible(false);
        animation_2_textfield->setAlpha(0);
        animation_2_textfield->setFrame(DRect(90,winSize.height/2-25,100,50));
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        animation_2_textfield->setFrame(DRect(90,winSize.height/2-25,winSize.width-200,50));
        animation_2_textfield->setAlpha(1);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CDUIShowView::endAction));
        CAViewAnimation::commitAnimations();
    }else if(tag==202){
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        animation_2_textfield->setFrame(DRect(90,winSize.height/2-25,0,50));
        animation_2_textfield->setAlpha(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CDUIShowView::endAction));
        CAViewAnimation::commitAnimations();
    }else if(tag==300){
        animation_3_imageview->setImageRect(DRect(0,0,100,winSize.height));
        animation_3_imageview->setFrame(DRect(0,0,100,winSize.height));
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.8f);
        CAViewAnimation::setAnimationRepeatAutoreverses(true);
        animation_3_imageview->setImageRect(DRect(winSize.width-100,0,100,winSize.height));
        animation_3_imageview->setFrame(DRect(winSize.width-100,0,100,winSize.height));
        CAViewAnimation::commitAnimations();
    }
}

void CDUIShowView::endAction()
{
    if (animation_2_btn_cancel->isVisible()) {
        animation_2_btn_cancel->setVisible(false);
    }else{
        animation_2_btn_cancel->setVisible(true);
    }
    
}

void CDUIShowView::showStepper()
{
    step_value = CALabel::createWithCenter(DRect(winSize.width/2, winSize.height/2, winSize.width, 40));
    step_value->setText("step_value:0");
	step_value->setColor(CAColor_black);
    step_value->setFontSize((_px(28)));
    step_value->setTextAlignment(CATextAlignmentCenter);
    step_value->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->getView()->addSubview(step_value);
    
    step = CAStepper::createWithCenter(DRect(winSize.width/2,winSize.height/4*3,160, 56));
    //step->setWraps(true);//是否循环,默认循环
    step->setMinValue(0);
    step->setMaxValue(50);
    step->setStepValue(1);
    //step->setAutoRepeat(false);
    this->getView()->addSubview(step);
//    step->setDividerImage(CAImage::create("image/stepper_divider1.png"), CAControlStateNormal);
//    step->setDecrementImage(CAImage::create("image/stepper_dec_n1.png"), CAControlStateNormal);
//    step->setIncrementImage(CAImage::create("image/stepper_inc_n1.png"), CAControlStateNormal);
//    step->setDecrementImage(CAImage::create("image/stepper_dec_h1.png"), CAControlStateHighlighted);
//    step->setIncrementImage(CAImage::create("image/stepper_inc_h1.png"), CAControlStateHighlighted);
    step->addTarget(this, CAControl_selector(CDUIShowView::stepperCallBack));
    
}

void CDUIShowView::stepperCallBack(CAControl *btn, DPoint point)
{
    char tem[30];
    sprintf(tem, "step_value:%.0f",step->getValue());
    step_value->setText(tem);
    CCLog("step-tag === %f",step->getValue());
}

void CDUIShowView::showGifView()
{
    CAGifView* swfBg = CAGifView::createWithGif(CAGif::createWithFilePath("image/gifview1.gif"));
    swfBg->setFrame(DRect(100, 100, winSize.width/2, winSize.height/2));
    swfBg->setRepeatForever(true);
    this->getView()->addSubview(swfBg);
}

void CDUIShowView::showVideo()
{
//    //http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4
//    //mnt/sdcard/video.mp4
//    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("image/video.mp4");
//    CAVideoPlayerController* pv = CAVideoPlayerController::createWithPath(path.c_str(), "asdas");
//    //CAVideoPlayerController* pv = CAVideoPlayerController::createWithUrl("http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4", "asdas");
//    this->getView()->addSubview(pv->getView());
//    pv->retain();
//    pv->play();
//    pv->setDelegate(this);
}

void CDUIShowView::showRenderImage()
{
    CAButton* btn = CAButton::create(CAButtonTypeSquareRect);
    btn->setCenter(DRect(winSize.width/2, winSize.height/2, 100, 50));
    btn->setTitleForState(CAControlStateNormal, "Click");
    btn->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    btn->setTag(1);
    btn->addTarget(this, CAControl_selector(CDUIShowView::renderCallBack), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn);
    
}

void CDUIShowView::renderCallBack(CAControl* control, DPoint point)
{
    CAButton* button = (CAButton*)control;
    if (button->getTag()==1) {
        CADevice::openAlbum(this);
    }else if(button->getTag()==2) {

        m_clvImage->setClippingEnabled(true);
        CARenderImage* rm = CARenderImage::create(_px(winSize.width-100), _px(winSize.width-100));
        rm->printscreenWithView(m_clvImage);
        
        renderImage = CAView::createWithFrame(DRect(50,winSize.height/4,winSize.width-100,winSize.width-100));
        this->getView()->addSubview(renderImage);
        
        m_clvImage->setClippingEnabled(false);
        
        if (m_clv!=NULL)
        {
            this->getView()->removeSubview(m_clv);
            m_clv = NULL;
            this->getView()->removeSubview(m_clvImage);
            m_clvImage = NULL;
            this->getView()->removeSubview(render_btn);
            render_btn = NULL;
        }
        
        CAImageView* imageView = CAImageView::createWithFrame(DRect(0,0,winSize.width-100,winSize.width-100));
        imageView->setImage(rm->getImageView()->getImage());
        renderImage->addSubview(imageView);
        
        CAScheduler::schedule(schedule_selector(CDUIShowView::scheduleFuck), this, 3);
    }

    
}

CADrawView* getStencil(const DSize& size, int index)
{
    if (index == 0)
    {
        DPoint ver[4];
        ver[0] = DPoint(0, 0);
        ver[1] = DPoint(0, size.height);
        ver[2] = DPoint(size.width, size.height);
        ver[3] = DPoint(size.width, 0);
        CADrawView* stencil = CADrawView::create();
        stencil->drawPolygon(ver, 4, ccc4f(255, 0, 0, 0), 2, ccc4f(255, 0, 0, 0));
        stencil->setFrameOrigin(DPoint(0, size.height));
        return stencil;
    }
    else if (index == 1)
    {
        DPoint cir[720];
        for (int i=0; i<720; i++)
        {
            float x = cosf(i * M_PI/180.f) * size.width/2;
            float y = sinf(i * M_PI/180.f) * size.width/2;
            cir[i] = DPoint(x, y);
        }
        CADrawView* stencil = CADrawView::create();
        stencil->drawPolygon(cir, 720, ccc4f(1, 1, 1, 0.5), 0, ccc4f(1, 1, 1, 0));
        stencil->setCenterOrigin(DPoint(size.width/2, size.height/2));
        return stencil;
    }
    return NULL;
}

void CDUIShowView::getSelectedImage(CAImage *image)
{
    int index = 0;
    
    DRect scrollRect;
    scrollRect.origin.x = 50;
    scrollRect.origin.y = winSize.height/4;
    scrollRect.size.width = winSize.width-100;
    scrollRect.size.height = scrollRect.size.width;
    
    m_clvImage = CAClippingView::create();
    m_clvImage->setStencil(getStencil(scrollRect.size, index));
    m_clvImage->setFrame(scrollRect);
    m_clvImage->setInverted(false);
    m_clvImage->setClippingEnabled(false);
    this->getView()->addSubview(m_clvImage);
    
    float temp_mini = 0;
    if (image->getContentSize().width>image->getContentSize().height) {
        temp_mini = scrollRect.size.height/image->getContentSize().height;
    }else{
        temp_mini = scrollRect.size.width/image->getContentSize().width;
    }
    CAScrollView* scrollView = CAScrollView::createWithFrame(m_clvImage->getBounds());
    scrollView->setViewSize(DSize(image->getContentSize()));
    scrollView->setContentOffset(DPoint(0,winSize.height/4), false);
    scrollView->setMinimumZoomScale(temp_mini);
    scrollView->setMaximumZoomScale(2.5f);
    scrollView->setBackGroundColor(CAColor_clear);
    scrollView->setShowsScrollIndicators(false);
    scrollView->setBounces(false);
    scrollView->setScrollViewDelegate(this);
    scrollView->setDisplayRange(true);
    m_clvImage->addSubview(scrollView);
    
    DRect rect;
    rect.origin = DPointZero;
    rect.size = scrollView->getViewSize();
    CAImageView* imv = CAImageView::createWithFrame(rect);
    imv->setImage(image);
    imv->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    scrollView->addSubview(imv);
    
    
    
    m_clv = CAClippingView::create();
    m_clv->setStencil(getStencil(scrollRect.size, index));
    m_clv->setFrame(scrollRect);
    m_clv->setInverted(true);
    m_clv->setTouchEnabled(false);
    this->getView()->addSubview(m_clv);

    DRect ivRect;
    ivRect.size = winSize;
    ivRect.origin = ccpMult(scrollRect.origin, -1);
    CAView* iv = CAView::createWithColor(ccc4(0,0,0,128));
    iv->setFrame(ivRect);
    m_clv->addSubview(iv);
    
    render_btn = CAButton::create(CAButtonTypeSquareRect);
    render_btn->setCenter(DRect(winSize.width/2, winSize.height-100, 100, 50));
    render_btn->setTitleForState(CAControlStateNormal, "Click");
    render_btn->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    render_btn->addTarget(this, CAControl_selector(CDUIShowView::renderCallBack), CAControlEventTouchUpInSide);
    render_btn->setTag(2);
    this->getView()->addSubview(render_btn);
}

void CDUIShowView::scheduleFuck(float dt)
{
    if (dle_ren_index>=1) {
        CAScheduler::unschedule(schedule_selector(CDUIShowView::scheduleFuck), this);
        if (renderImage!=NULL)
        {
            this->getView()->removeSubview(renderImage);
            renderImage = NULL;
        }
        dle_ren_index = 0;
    }else{
        dle_ren_index++;
    }

    
}

void CDUIShowView::jsonTest()
{
    Value value1;
    value1[0]["name"] = "aaaa";
    value1[0]["id"] = "1001";
    value1[0]["gender"] = "1";
    value1[0]["occupation"] = "adfafaf";
    
    value1[1]["name"] = "bbbb";
    value1[1]["id"] = "1002";
    value1[1]["gender"] = "2";
    value1[1]["occupation"] = "fsdfsfs";
    
    FastWriter writer;
    string tempjson = writer.write(value1);
    CCLog("Write json == %s",tempjson.c_str());
    
    Reader reader;
    Value value;
    string jsonData = CCFileUtils::sharedFileUtils()->getFileString(tempjson.c_str());

    if (reader.parse(jsonData.c_str(),value))
    {
        int length = value.size();
        CCLog("length==%d",length);
        for (int index = 0; index < length; index++)
        {
            Info* personInfo = new Info();
            personInfo->autorelease();
            personInfo->name = value[index]["name"].asString();
            personInfo->uid = value[index]["id"].asString();
            personInfo->gender = value[index]["gender"].asString();
            personInfo->occupation = value[index]["occupation"].asString();
            CCLog("personInfo->name == %s",personInfo->name.c_str());
        }
    }
    
}

void CDUIShowView::httpTest()
{
    string url = "http://test.yuhaisong.com/test/index.php";
    CAHttpRequest* request = new CAHttpRequest();
    request->setTextTag("Getpage");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setUrl(url.c_str());
    request->setResponseCallback(this,httpresponse_selector(CDUIShowView::requestResult));
    CAHttpClient* httpClient = CAHttpClient::getInstance();
    httpClient->setTimeoutForConnect(30);
    httpClient->send(request);
    request->release();
}

void CDUIShowView::requestResult(CAHttpClient *client, CAHttpResponse *response)
{
    if (!response->isSucceed())
    {
        CCLog("Error reason: %s", response->getErrorBuffer());
        return;
    }

    if (response->getResponseCode() == 200)
    {
        std::string responseRes = "";
        std::vector<char> *buffer = response->getResponseData();
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            responseRes += (*buffer)[i];
        }
        CCLog("%s",responseRes.c_str());
    }
}

void CDUIShowView::DeviceTest()
{
    
}