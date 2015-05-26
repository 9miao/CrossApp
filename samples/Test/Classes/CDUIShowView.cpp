//
//  CDUIShowView.cpp
//  Test
//
//  Created by renhongguang on 15/4/7.
//
//

#include "CDUIShowView.h"
std::vector<std::string> sectionTitle;

ETableViewCell::ETableViewCell()
{
    this->setAllowsSelected(false);
}

ETableViewCell::~ETableViewCell()
{
    
}

ETableViewCell* ETableViewCell::create(const std::string& identifier, const CADipRect& _rect)
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
    CADipSize _size = this->getFrame().size;
    CALabel* test = CALabel::createWithCenter(CADipRect(_size.width/2,
                                                        _size.height/2,
                                                        _size.width,
                                                        _size.height));
    test->setColor(ccc4(34,151,254,255));
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

bool ETableView::init(const CrossApp::CCRect &rect)
{
    size = rect.size;
    p_TableView = CATableView::createWithFrame(CADipRect(0, 0, size.width, size.height));
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

ETableView* ETableView::createWithFrame(const CrossApp::CCRect &rect)
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

ETableView* ETableView::createWithCenter(const CrossApp::CCRect &rect)
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

CATableViewCell* ETableView::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CADipSize _size = cellSize;
    ETableViewCell* cell = dynamic_cast<ETableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = ETableViewCell::create("CrossApp", CADipRect(0, 0, _size.width, _size.height));
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
}

CDListView::~CDListView()
{
    
}

bool CDListView::init(const CrossApp::CCRect &rect)
{
    CADipSize size = rect.size;
    CAListView *listView = CAListView::createWithCenter(CADipRect(size.width/2,size.height/2,size.width,50));
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

CDListView* CDListView::createWithFrame(const CrossApp::CCRect &rect)
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

CDListView* CDListView::createWithCenter(const CrossApp::CCRect &rect)
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

CAListViewCell* CDListView::listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index)
{
    CADipSize _size = cellSize;
    CAListViewCell* cell = (CAListViewCell*)listView->dequeueReusableCellWithIdentifier("ListViewCell");
    if (!cell)
    {
        cell = CAListViewCell::create("ListViewCell");
        CALabel* test = CALabel::createWithCenter(CADipRect(_size.width/2,
                                                            _size.height/2,
                                                            _size.width,
                                                            _size.height));
        test->setColor(ccc4(51,204,255,255));
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
    this->getView()->setColor(CAColor_gray);
}

void CDUIShowView::viewDidUnload()
{
    
}

void CDUIShowView::initUIWithIndex(int _index)
{
    UIINDEX = _index;
    switch (_index) {
        case 0:
            showAlertView();
            break;
        case 1:
            showButton();
            break;
        case 2:
            showImage();
            break;
        case 3:
            showScale9Image();
            break;
        case 4:
            showIndicator();
            break;
        case 5:
            showLabel();
            break;
        case 6:
            showProgress();
            break;
        case 7:
            showSlider();
            break;
        case 8:
            showSwitch();
            break;
        case 9:
            showTextField();
            break;
        case 10:
            showTextView();
            break;
        case 11:
            showSegmentedControl();
            break;
        case 12:
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
            //httpTest();
            showWebView();
            break;
        case 17:
            showCollectionView();
            break;
        case 18:
            showScrollView();
            break;
        default:
            break;
    }
    initButtonControl();
}

void CDUIShowView::initButtonControl()
{
    CAButton* btn_l = CAButton::create(CAButtonTypeSquareRect);
    btn_l->setTag(1);
    btn_l->setCenter(CADipRect(winSize.width/2-80, winSize.height-50, 64, 64));
    btn_l->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_l->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_l->addTarget(this, CAControl_selector(CDUIShowView::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_l,10);
    
    CAButton* btn_r = CAButton::create(CAButtonTypeSquareRect);
    btn_r->setTag(2);
    btn_r->setCenter(CADipRect(winSize.width/2+80, winSize.height-50, 64, 64));
    btn_r->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_r->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_r->addTarget(this, CAControl_selector(CDUIShowView::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_r,10);
}

void CDUIShowView::buttonControlCallBack(CrossApp::CAControl *btn, CrossApp::CCPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    CCLog("btn_tag===%d",button->getTag());
    int temIndex = button->getTag();
    if (temIndex==1) {
        if (showIndex>0) {
            showIndex--;
        }else if(showIndex==0){
            showIndex=showNum-1;
        }
    }else if(temIndex==2){
        if (showIndex<showNum) {
            showIndex++;
            if (showIndex==showNum) {
                showIndex=0;
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
        case 6:
            showProgressByIndex();
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
        case 12:
            showTabBarByIndex();
            break;
        default:
            break;
    }
    
}

void CDUIShowView::showAlertView()
{
    CAButton* btn = CAButton::create(CAButtonTypeRoundedRect);
    btn->setCenter(CADipRect(winSize.width/2, winSize.height/2, 100, 50));
    btn->setTitleForState(CAControlStateAll, "Click");
    btn->addTarget(this, CAControl_selector(CDUIShowView::alertButtonCallBack), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn);
    showNum = 3;
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

void CDUIShowView::alertButtonCallBack(CAControl* btn,CCPoint point)
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
    CAButton* btn1 = CAButton::create(CAButtonTypeCustom);
    btn1->setTag(100);
    btn1->setCenter(CADipRect(winSize.width/2, winSize.height/2, 100, 50));

    btn1->setTitleForState(CAControlStateNormal, "Noborder");
    btn1->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    btn1->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn1);
    showNum = 5;
}

void CDUIShowView::showButtonByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("Button Noborder"));
        CAButton* btn1 = CAButton::create(CAButtonTypeCustom);
        btn1->setTag(100);
        btn1->setCenter(CADipRect(winSize.width/2, winSize.height/2, 100, 50));
        
        btn1->setTitleForState(CAControlStateNormal, "Noborder");
        btn1->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
        btn1->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchUpInSide);
        this->getView()->addSubview(btn1);
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("Button SquareRect"));
        CAButton* btn2 = CAButton::create(CAButtonTypeSquareRect);
        btn2->setTag(100);
        btn2->setCenter(CADipRect(winSize.width/2, winSize.height/2, 200, 50));
        btn2->setTitleForState(CAControlStateAll,"SquareRect");
        btn2->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchUpInSide);
        this->getView()->addSubview(btn2);
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("Button RoundedRect"));
        CAButton* btn3 = CAButton::create(CAButtonTypeRoundedRect);
        btn3->setTag(100);
        btn3->setCenter(CADipRect(winSize.width/2, winSize.height/2, 200, 50));
        btn3->setTitleForState(CAControlStateAll, "RoundedRect");
        btn3->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchUpInSide);
        this->getView()->addSubview(btn3);
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("Button Custom"));
        CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
        btn5->setTag(100);
        btn5->setCenter(CADipRect(winSize.width/2, winSize.height/2, 200, 50));
        //btn5->setTitleForState(CAControlStateAll, "Button");
        btn5->setTitleColorForState(CAControlStateNormal,CAColor_white);
        btn5->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/button_normal.png")));
        btn5->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/button_down.png")));
        btn5->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchDown);
        this->getView()->addSubview(btn5);
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("Button AllowsSelected"));
        CAButton* btn4 = CAButton::create(CAButtonTypeSquareRect);
        btn4->setTag(100);
        btn4->setAllowsSelected(true);
        btn4->setCenter(CADipRect(winSize.width/2, winSize.height/2, 200, 50));
        btn4->setTitleForState(CAControlStateNormal, "Normal");
        btn4->setTitleColorForState(CAControlStateNormal, CAColor_white);
        btn4->setTitleForState(CAControlStateSelected, "Selected");
        btn4->setTitleForState(CAControlStateHighlighted, "Highlighted");
        btn4->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png")));
        btn4->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_highlighted.png")));
        btn4->setBackGroundViewForState(CAControlStateSelected, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png")));
        btn4->addTarget(this, CAControl_selector(CDUIShowView::buttonCallBack), CAControlEventTouchDown);
        this->getView()->addSubview(btn4);
    }
}

void CDUIShowView::buttonCallBack(CAControl* btn,CCPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    CCLog("btn_tag===%d",button->getTag());
}

void CDUIShowView::showImage()
{
    CAImageView* image = CAImageView::createWithImage(CAImage::create("HelloWorld.png"));
    image->setFrame(this->getView()->getBounds());
    image->setTag(100);
    this->getView()->addSubview(image);
    showNum = 7;
}

void CDUIShowView::showImageByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("ImageView"));
        CAImageView* image = CAImageView::createWithImage(CAImage::create("HelloWorld.png"));
        image->setTag(100);
        image->setFrame(this->getView()->getBounds());
        this->getView()->addSubview(image);
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageXY"));
        CAImageView* image = CAImageView::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,400,400));
        image->setTag(100);
        image->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        image->setImage(CAImage::create("HelloWorld.png"));
        this->getView()->addSubview(image);
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageCrop"));
        CAImageView* image = CAImageView::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,400,400));
        image->setTag(100);
        image->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        image->setImage(CAImage::create("HelloWorld.png"));
        this->getView()->addSubview(image);
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitImageInside"));
        CAImageView* image = CAImageView::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,400,400));
        image->setTag(100);
        image->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        image->setImage(CAImage::create("HelloWorld.png"));
        this->getView()->addSubview(image);
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitViewByHorizontal"));
        CAImageView* image = CAImageView::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,400,400));
        image->setTag(100);
        image->setImageViewScaleType(CAImageViewScaleTypeFitViewByVertical);
        image->setImage(CAImage::create("HelloWorld.png"));
        this->getView()->addSubview(image);
    }else if(showIndex==5){
        this->setNavigationBarItem(CANavigationBarItem::create("ScaleTypeFitViewByVertical"));
        CAImageView* image = CAImageView::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,400,400));
        image->setTag(100);
        image->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
        image->setImage(CAImage::create("HelloWorld.png"));
        this->getView()->addSubview(image);
    }
    else if(showIndex==6){
        this->setNavigationBarItem(CANavigationBarItem::create("Frame animation"));
        CAVector<CAImage* > img;
        for (int i=0; i<8; i++) {
            char temp[50];
            sprintf(temp, "animation/npc_382-%d.png",i+1);
            CAImage* im = CAImage::create(temp);
            img.pushBack(im);
        }
        CAImageView* image1 = CAImageView::createWithImage(CAImage::create("animation/npc_382-1.png"));
        image1->setTag(100);
        image1->setCenter(CADipRect(winSize.width/2,winSize.height/2,40,88));
        image1->setAnimationImages(img);
        image1->setAnimationDuration(0.2);
        this->getView()->addSubview(image1);
        image1->startAnimating();
    }
}

void CDUIShowView::showScale9Image()
{
    CAScale9ImageView* s9image = CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_normal.png"));
    s9image->setCenter(CADipRect(winSize.width/2,winSize.height/2,200,100));
    this->getView()->addSubview(s9image);
}

void CDUIShowView::showIndicator()
{
    CAActivityIndicatorView* idc = CAActivityIndicatorView::createWithCenter(CADipRect(winSize.width/2, winSize.height/2,
                                                                     100, 100));
    idc->setStyle(CAActivityIndicatorViewStyleWhiteLarge);
    idc->setTag(100);
    //idc->setTargetOnCancel(this, callfunc_selector(CDUIShowView::cancelCallback));
    this->getView()->addSubview(idc);
    idc->startAnimating();
    showNum = 5;
}

void CDUIShowView::showIndicatorByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView"));
        CAActivityIndicatorView* idc = CAActivityIndicatorView::createWithCenter(CADipRect(winSize.width/2, winSize.height/2,
                                                                                           100, 100));
        idc->setStyle(CAActivityIndicatorViewStyleWhiteLarge);
        idc->setTag(100);
        //idc->setTargetOnCancel(this, callfunc_selector(CDUIShowView::cancelCallback));
        this->getView()->addSubview(idc);
        idc->startAnimating();
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView1"));
        CAActivityIndicatorView* idc = CAActivityIndicatorView::createWithCenter(CADipRect(winSize.width/2, winSize.height/2,
                                                                                           100, 100));
        idc->setStyle(CAActivityIndicatorViewStyleWhite);
        idc->setTag(100);
        //idc->setTargetOnCancel(this, callfunc_selector(CDUIShowView::cancelCallback));
        this->getView()->addSubview(idc);
        idc->startAnimating();
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView2"));
        CAActivityIndicatorView* idc = CAActivityIndicatorView::createWithCenter(CADipRect(winSize.width/2, winSize.height/2,
                                                                                           100, 100));
        idc->setStyle(CAActivityIndicatorViewStyleGray);
        idc->setTag(100);
        //idc->setTargetOnCancel(this, callfunc_selector(CDUIShowView::cancelCallback));
        this->getView()->addSubview(idc);
        idc->startAnimating();
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView2"));
        CAActivityIndicatorView* idc = CAActivityIndicatorView::createWithCenter(CADipRect(winSize.width/2, winSize.height/2,
                                                                                           100, 100));
        idc->setStyle(CAActivityIndicatorViewStyleImage);
        idc->setTag(100);
        idc->setActivityIndicatorView(CAImageView::createWithImage(CAImage::create("image/indicator1.png")));
        //idc->setTargetOnCancel(this, callfunc_selector(CDUIShowView::cancelCallback));
        this->getView()->addSubview(idc);
        idc->startAnimating();
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("IndicatorView2"));
        CAActivityIndicatorView* idc = CAActivityIndicatorView::createWithCenter(CADipRect(winSize.width/2, winSize.height/2,
                                                                                           100, 100));
        idc->setStyle(CAActivityIndicatorViewStyleImage);
        idc->setTag(100);
        idc->setActivityIndicatorView(CAImageView::createWithImage(CAImage::create("image/indicator2.png")));
        //idc->setTargetOnCancel(this, callfunc_selector(CDUIShowView::cancelCallback));
        this->getView()->addSubview(idc);
        idc->startAnimating();
    }
}

void CDUIShowView::showLabel()
{
    CALabel* label = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 200, 50));
    label->setText("Hello World");
    label->setColor(CAColor_black);
    label->setFontName("fonts/m.ttf");
    label->setFontSize((_px(36)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setTag(100);
    this->getView()->addSubview(label);
    showNum = 6;
}

void CDUIShowView::showLabelByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex==0) {
        CALabel* label = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 200, 50));
        label->setText("Hello World");
        label->setColor(CAColor_black);
        label->setFontName("fonts/m.ttf");
        label->setFontSize((_px(36)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setTag(100);
        this->getView()->addSubview(label);
    }else if(showIndex==1){
        CALabel* label = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 200, 50));
        label->setText("Hello World");
        label->setColor(CAColor_black);
        label->setFontSize((_px(36)));
        label->setUnderLine(true);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setTag(100);
        this->getView()->addSubview(label);
    }
    else if(showIndex==2){
        CALabel* label = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 200, 50));
        label->setText("Hello World");
        label->setColor(CAColor_black);
        label->setFontSize((_px(36)));
        label->setBold(true);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setTag(100);
        this->getView()->addSubview(label);
    }
    else if(showIndex==3){
        CALabel* label = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 300, 50));
        label->setText("Hello World");
        label->setColor(CAColor_black);
        label->setFontSize((_px(36)));
        label->setItalics(true);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setTag(100);
        this->getView()->addSubview(label);
    }
    else if(showIndex==4){
        CALabel* label = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 400, 50));
        label->setText("Hello World Can Copy");
        label->setColor(CAColor_black);
        label->setFontSize((_px(36)));
        label->setEnableCopy(true);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setTag(100);
        this->getView()->addSubview(label);
    }
    else if(showIndex==5){
        CALabel* label = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width, 200));
        label->setText("Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World");
        label->setColor(CAColor_black);
        label->setFontSize((_px(36)));
        label->setUnderLine(true);
        label->setBold(true);
        label->setItalics(true);
        label->setLineSpacing(20);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setTag(100);
        this->getView()->addSubview(label);
    }
}

void CDUIShowView::showProgress()
{
    progress_value = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/4, 200, 50));
    progress_value->setText("");
    progress_value->setFontSize(_px(30));
    progress_value->setColor(ccc4(51,204,255,255));
    progress_value->setTextAlignment(CATextAlignmentCenter);
    progress_value->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->getView()->addSubview(progress_value);
    
    progress = CAProgress::create();
    progress->setCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width/2, 0));

    this->getView()->addSubview(progress);
    CAScheduler::schedule(schedule_selector(CDUIShowView::progressValueChange), this, 0.01, false);
    showNum = 2;
}

void CDUIShowView::showProgressByIndex()
{
    this->getView()->removeSubview(progress);
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("Progress"));
        progress_value->setText("");
        progress = CAProgress::create();
        progress->setCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width/2, 0));
        this->getView()->addSubview(progress);
        CAScheduler::schedule(schedule_selector(CDUIShowView::progressValueChange), this, 0.01, false);
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("Progress Custem"));
        progress_value->setText("");
        progress = CAProgress::create();
        progress->setProgressTintColor(CAColor_white);
        progress->setProgressTrackColor(CAColor_white);
        progress->setCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width/2, 50));
        progress->setProgressTrackImage(CAImage::create("source_material/btn_rounded_normal.png"));
        progress->setProgressTintImage(CAImage::create("source_material/btn_rounded_highlighted.png"));
        
        this->getView()->addSubview(progress);
        CAScheduler::schedule(schedule_selector(CDUIShowView::progressValueChange), this, 0.01, false);
    }
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
    sliderValue = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/4, 200, 50));
    sliderValue->setColor(ccc4(51,204,255,255));
    sliderValue->setText("");
    sliderValue->setFontSize(_px(30));
    sliderValue->setTextAlignment(CATextAlignmentCenter);
    sliderValue->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->getView()->addSubview(sliderValue);
    
    slider = CASlider::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,winSize.width/2,20));
    slider->addTarget(this, CAControl_selector(CDUIShowView::sliderValueChange));
    this->getView()->addSubview(slider);
    showNum = 2;
}

void CDUIShowView::showSliderByindex()
{
    this->getView()->removeSubview(slider);
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("Slider"));
        slider = CASlider::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,winSize.width/2,20));
        slider->addTarget(this, CAControl_selector(CDUIShowView::sliderValueChange));
        this->getView()->addSubview(slider);
        sliderValue->setText("");
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("Slider Custem"));
        slider = CASlider::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,winSize.width/2,20));
        slider->setTrackHeight(20);
        slider->setMaxTrackTintImage(CAImage::create("source_material/ex1.png"));
        slider->setMinTrackTintImage(CAImage::create("source_material/ex3.png"));
        slider->setThumbTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
        
        slider->addTarget(this, CAControl_selector(CDUIShowView::sliderValueChange));
        this->getView()->addSubview(slider);
        sliderValue->setText("");
    }
}

void CDUIShowView::sliderValueChange(CAControl* btn, CCPoint point)
{
    char value[20] = "";
    CASlider* p_Slider = (CASlider*)btn;
    sprintf(value, "%.02f%%", p_Slider->getValue() * 100);
    sliderValue->setText(value);
}

void CDUIShowView::showSwitch()
{
    CASwitch* customSwitch = CASwitch::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 100, 20));
    customSwitch->setTag(100);
    customSwitch->addTarget(this, CAControl_selector(CDUIShowView::switchStateChange));
    this->getView()->addSubview(customSwitch);
    
    switchState = CALabel::createWithCenter(CADipRect(winSize.width/2, winSize.height/4, 300, 50));
    switchState->setColor(ccc4(51,204,255,255));
    switchState->setText("Switch State:Off");
    switchState->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
    switchState->setTextAlignment(CATextAlignmentCenter);
    switchState->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->getView()->addSubview(switchState);
    showNum = 2;
}

void CDUIShowView::showSwitchByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("Switch"));
        CASwitch* customSwitch = CASwitch::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 100, 20));
        customSwitch->setTag(100);
        customSwitch->addTarget(this, CAControl_selector(CDUIShowView::switchStateChange));
        this->getView()->addSubview(customSwitch);
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("Switch Custem"));
        CASwitch* customSwitch = CASwitch::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 100, 20));
        customSwitch->setTag(100);
        customSwitch->setOnImage(CAImage::create("image/Switch_on.png"));
        customSwitch->setOffImage(CAImage::create("image/switch_off.png"));
        //customSwitch->setThumbTintImage(CAImage::create("image/switch_p.png"));
        
        customSwitch->addTarget(this, CAControl_selector(CDUIShowView::switchStateChange));
        this->getView()->addSubview(customSwitch);
    }
}

void CDUIShowView::switchStateChange(CAControl* btn, CCPoint point)
{
    CASwitch* state = (CASwitch*)btn;
    if (!state->isOn())
    {
        switchState->setText("Switch State:Off");
    }
    else
    {
        switchState->setText("Switch State:On");
    }
}

void CDUIShowView::showTextField()
{
    CATextField* textField = CATextField::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width/2, 50));
    textField->setTag(100);
    textField->setPlaceHolder("Input");
    textField->setKeyboardType(KEY_BOARD_TYPE_NORMAL);
    this->getView()->addSubview(textField);
    showNum = 2;
}

void CDUIShowView::showTextFieldByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("TextField"));
        CATextField* textField = CATextField::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width/2, 50));
        textField->setTag(100);
        textField->setPlaceHolder("Input");
        this->getView()->addSubview(textField);
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("TextField Custem"));
        CATextField* textField = CATextField::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width/2, 50));
        textField->setTag(100);
        textField->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_highlighted.png")));
        textField->setPlaceHolder("Input");
        this->getView()->addSubview(textField);
    }
}

void CDUIShowView::showTextView()
{
    CATextView* textView = CATextView::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, 500, 300));
    textView->setPlaceHolder("TextView");
    //textView->setSpaceHolderColor(CAColor_blueStyle);
    //textView->setTextColor(CAColor_green);
    //textView->setCursorColor(CAColor_red);
    textView->setFontSize(_px(40));
    //textView->setWordWrap(false);
    //textView->setLineSpacing(50);
    //textView->setFontColor(CAColor_gray);
    //textView->setBackGroundColor(CAColor_blueStyle);
    textView->setBackGroundImage(CAImage::create("source_material/btn_square_selected.png"));
    this->getView()->addSubview(textView);
}

void CDUIShowView::showSegmentedControl()
{
    CASegmentedControl* segment = CASegmentedControl::createWithCenter(CADipRect(winSize.width/2,
                                                             winSize.height/4,
                                                             winSize.width/2,
                                                             50), 3);
    char temstr[20];
    for (int i = 0; i < 3; i++)
    {
        sprintf(temstr, "Page%d", i + 1);
        segment->setTitleForSegmentAtIndex(temstr, i);
    }
    segment->setTitleColor(CAColor_black);
    segment->setTitleSelectedColor(CAColor_white);
    
    segment->addTarget(this, CASegmentedControl_selector(CDUIShowView::segmentCallback));
//    segment->setBackgroundImage(CAImage::create("image/seg1.png"));
//    segment->setSegmentItemBackgroundImage(CAImage::create("image/seg2.png"));
    segment->setSelectedAtIndex(0);
    this->getView()->addSubview(segment);
    segment->setTag(100);
    
    segmentPage = CALabel::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,200,50));
    segmentPage->setColor(CAColor_black);
    segmentPage->setTextAlignment(CATextAlignmentCenter);
    segmentPage->setText("page1");
    this->getView()->addSubview(segmentPage);
    showNum = 2;
}

void CDUIShowView::showSegmentedControlByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("Segment"));
        CASegmentedControl* segment = CASegmentedControl::createWithCenter(CADipRect(winSize.width/2,
                                                                                     winSize.height/4,
                                                                                     winSize.width/2,
                                                                                     50), 3);
        char temstr[20];
        for (int i = 0; i < 3; i++)
        {
            sprintf(temstr, "Page%d", i + 1);
            segment->setTitleForSegmentAtIndex(temstr, i);
        }
        segment->setTitleColor(CAColor_black);
        segment->setTitleSelectedColor(CAColor_white);
        
        segment->addTarget(this, CASegmentedControl_selector(CDUIShowView::segmentCallback));
        segment->setSelectedAtIndex(0);
        this->getView()->addSubview(segment);
        segment->setTag(100);
        segmentPage->setText("page1");
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("Segment Custem"));
        CASegmentedControl* segment = CASegmentedControl::createWithCenter(CADipRect(winSize.width/2,
                                                                                     winSize.height/4,
                                                                                     winSize.width/2,
                                                                                     50), 3);
        char temstr[20];
        for (int i = 0; i < 3; i++)
        {
            sprintf(temstr, "Page%d", i + 1);
            segment->setTitleForSegmentAtIndex(temstr, i);
        }
        segment->setTitleColor(CAColor_black);
        segment->setTitleSelectedColor(CAColor_white);
        
        segment->addTarget(this, CASegmentedControl_selector(CDUIShowView::segmentCallback));
        segment->setBackgroundImage(CAImage::create("image/seg1.png"));
        segment->setSegmentItemBackgroundImage(CAImage::create("image/seg2.png"));
        segment->setSelectedAtIndex(0);
        this->getView()->addSubview(segment);
        segment->setTag(100);
        segmentPage->setText("page1");
    }
}

void CDUIShowView::segmentCallback(CASegmentedControl* btn, int index)
{
    switch (index)
    {
        case 0:
        {
            segmentPage->setText("page1");
            break;
        }
        case 1:
        {
            segmentPage->setText("page2");
            break;
        }
        case 2:
        {
            segmentPage->setText("page3");
            break;
        }
        default:
            break;
    }
}

void CDUIShowView::showTabBar()
{
    CATabBarItem* item1 = CATabBarItem::create("item1", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item2 = CATabBarItem::create("item2", CAImage::create(""), CAImage::create(""));
    CATabBarItem* item3 = CATabBarItem::create("item3", CAImage::create(""), CAImage::create(""));
    
    std::vector<CATabBarItem*> item;
    item.push_back(item1);
    item.push_back(item2);
    item.push_back(item3);
    
    CATabBar* tabBar = CATabBar::create(item);
    tabBar->setFrame(CADipRect(0, winSize.height-200, winSize.width, 100));
    tabBar->showSelectedIndicator();
    tabBar->setTitleColorForNormal(CAColor_yellow);
    tabBar->setTitleColorForSelected(CAColor_orange);
    tabBar->setTag(100);
    this->getView()->addSubview(tabBar);
    tabBar->setSelectedAtIndex(0);
    showNum = 2;
}

void CDUIShowView::showTabBarByIndex()
{
    this->getView()->removeSubviewByTag(100);
    if (showIndex==0) {
        CATabBarItem* item1 = CATabBarItem::create("item1", CAImage::create(""), CAImage::create(""));
        CATabBarItem* item2 = CATabBarItem::create("item2", CAImage::create(""), CAImage::create(""));
        CATabBarItem* item3 = CATabBarItem::create("item3", CAImage::create(""), CAImage::create(""));
        
        std::vector<CATabBarItem*> item;
        item.push_back(item1);
        item.push_back(item2);
        item.push_back(item3);
        
        CATabBar* tabBar = CATabBar::create(item);
        tabBar->setFrame(CADipRect(0, winSize.height-200, winSize.width, 100));
        tabBar->showSelectedIndicator();
        tabBar->setTitleColorForNormal(CAColor_yellow);
        tabBar->setTitleColorForSelected(CAColor_orange);
        tabBar->setTag(100);
        this->getView()->addSubview(tabBar);
        tabBar->setSelectedAtIndex(0);
    }else if(showIndex==1){
        CATabBarItem* item1 = CATabBarItem::create("", CAImage::create("image/tab_news_btn_up.png"), CAImage::create("image/tab_news_btn_down.png"));
        CATabBarItem* item2 = CATabBarItem::create("", CAImage::create("image/tab_image_btn_up.png"), CAImage::create("image/tab_image_btn_down.png"));
        CATabBarItem* item3 = CATabBarItem::create("", CAImage::create("image/tab_mine_btn_up.png"), CAImage::create("image/tab_mine_btn_down.png"));
        
        std::vector<CATabBarItem*> item;
        item.push_back(item1);
        item.push_back(item2);
        item.push_back(item3);
        
        CATabBar* tabBar = CATabBar::create(item);
        tabBar->setFrame(CADipRect(0, winSize.height-200, winSize.width, 100));
        //tabBar->showSelectedIndicator();
        tabBar->setTitleColorForNormal(CAColor_yellow);
        tabBar->setTitleColorForSelected(CAColor_orange);
        tabBar->setBackGroundImage(CAImage::create("image/tab_news_bg.png"));
        tabBar->setSelectedBackGroundColor(CAColor_clear);
        tabBar->setTag(100);
        this->getView()->addSubview(tabBar);
        tabBar->setSelectedAtIndex(0);
    }
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
    
    p_pageView = CAPageView::createWithCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width, winSize.height), CAPageViewDirectionHorizontal);
    p_pageView->setViews(_view);
    p_pageView->setPageViewDelegate(this);
    this->getView()->addSubview(p_pageView);
    
    pageControl = CAPageControl::createWithCenter(CADipRect(winSize.width/2, 50, 100, 50));
    pageControl->setNumberOfPages(_view.size());
    pageControl->setPageIndicatorTintColor(CAColor_gray);
    pageControl->setCurrentPageIndicatorTintColor(CAColor_green);
    pageControl->addTarget(this, CAControl_selector(CDUIShowView::pageControlCallBack));
    this->getView()->addSubview(pageControl);
}
void CDUIShowView::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void CDUIShowView::pageViewDidEndTurning(CAPageView* pageView)
{
    pageControl->setCurrentPage(pageView->getCurrPage());
    pageControl->updateCurrentPageDisplay();
}

void CDUIShowView::pageControlCallBack(CrossApp::CAControl *btn, CrossApp::CCPoint point){
    CAPageControl* button = (CAPageControl*)btn;
    CCLog("btn_tag===%d",button->getCurrentPage());
    p_pageView->setCurrPage(button->getCurrentPage(), true);
}
void CDUIShowView::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const CCPoint& point)
{

}

void CDUIShowView::reshapeViewRectDidFinish()
{
    
}
//TableView
void CDUIShowView::showTableView()
{
    p_TableView = CATableView::createWithFrame(CADipRect(0, 0, winSize.width-50, winSize.height-100));
    p_TableView->setTableViewDataSource(this);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setAllowsSelection(true);
    p_TableView->setAllowsMultipleSelection(false);
    p_TableView->setShowsScrollIndicators(false);
    p_TableView->setSelectRowAtIndexPath(2, 1);
    this->getView()->addSubview(p_TableView);
    
    
    ETableView* p_TableView1 = ETableView::createWithFrame(CADipRect(winSize.width-50, 0, 50, winSize.height-100));
    p_TableView1->setETableViewDelegate(this);
    this->getView()->addSubview(p_TableView1);
}

void CDUIShowView::etableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    //CCLog("ssss====%d",row);
    p_TableView->setContentOffset(CADipPoint(0,313*row), false);
}

void CDUIShowView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

void CDUIShowView::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDUIShowView::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CADipSize _size = cellSize;
    CATableViewCell* cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CATableViewCell::create("CrossApp");
        CAImageView* image = CAImageView::createWithCenter(CADipRect(50, _size.height/2, 96, 96));
        image->setImage(CAImage::create("source_material/second_2.png"));
        cell->addSubview(image);
        
        CALabel* cellText = CALabel::createWithCenter(CADipRect(200, _size.height/2, _size.width/2, _size.height));
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

CAView* CDUIShowView::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    std::string head = sectionTitle.at(section);
    CAView* view = CAView::createWithColor(CAColor_gray);
    
    CADipSize _size = viewSize;
    CALabel* header = CALabel::createWithCenter(CADipRect(50, _size.height/2, _size.width/2, _size.height));
    header->setText(head);
    header->setFontSize(_px(30));
    header->setColor(CAColor_white);
    header->setTextAlignment(CATextAlignmentCenter);
    header->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    view->addSubview(header);
    
    return view;
}

CAView* CDUIShowView::tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
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
    CDListView* listview1 = CDListView::createWithFrame(CADipRect(0,0,winSize.width,50));
    this->getView()->addSubview(listview1);
    
    p_ListView = CAListView::createWithFrame(CADipRect(0,50,winSize.width,winSize.height-150));
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
    return 8;
}

unsigned int CDUIShowView::listViewHeightForIndex(CAListView *listView, unsigned int index)
{
    return _px(100);
}

CAListViewCell* CDUIShowView::listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index)
{
    CADipSize _size = cellSize;
    CAListViewCell* cell = (CAListViewCell*)listView->dequeueReusableCellWithIdentifier("ListViewCell");
    if (cell==NULL)
    {
        cell = CAListViewCell::create("ListViewCell");
        
        CALabel* test = CALabel::createWithCenter(CADipRect(100,
                                                            _size.height/2,
                                                            _size.width,
                                                            _size.height));
        test->setColor(ccc4(51,204,255,255));
        test->setTextAlignment(CATextAlignmentCenter);
        test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        test->setFontSize(_px(28));
        test->setTag(100);
        cell->addSubview(test);
        
        CAButton* btn = CAButton::createWithCenter(CADipRect(_size.width-100, _size.height/2, 100, 50), CAButtonTypeSquareRect);
        btn->setTitleForState(CAControlStateNormal, "btn");
        btn->setTag(200);
        cell->addSubview(btn);
    }
    char temptext[10];
    sprintf(temptext, "cell-%d",index);
    CALabel* test = (CALabel*)cell->getSubviewByTag(100);
    test->setText(temptext);

    return cell;
}

void CDUIShowView::showWebView()
{
    CAWebView* web = CAWebView::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,winSize.width,winSize.height/2));
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
    
    p_Conllection = CACollectionView::createWithFrame(CADipRect(0, 0, winSize.width, winSize.height-100));
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

CACollectionViewCell* CDUIShowView::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    if (row * 3 + item >= colorArr.size())
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
        CALabel* itemText = CALabel::createWithCenter(CADipRect(itemSize.width/2, itemSize.height/2, 150, 40));
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
    p_ScrollView = CAScrollView::createWithFrame(CADipRect(0,0,winSize.width,winSize.height));
    p_ScrollView->setMinimumZoomScale(0.2f);
    p_ScrollView->setMaximumZoomScale(5.0f);
    this->getView()->addSubview(p_ScrollView);
    
    CADipSize _size = p_ScrollView->getBounds().size;
    p_imageView = CAImageView::createWithImage(CAImage::create("HelloWorld.png"));
    p_imageView->setCenter(CADipRect(_size.width/2, _size.height/2,800,1200));
    p_ScrollView->addSubview(p_imageView);
}

void CDUIShowView::zoomViewBySliderValue(CrossApp::CAControl *btn, CrossApp::CCPoint point)
{
//    float multiple = zoomView->getValue()+1.0;
//    p_imageView->setScale(multiple);
}

void CDUIShowView::jsonTest()
{
    Reader reader;
    Value value;
    string jsonFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("information.json");
    CCString *jsonData = CCString::createWithContentsOfFile(jsonFile.c_str());
    if (reader.parse(jsonData->getCString(),value))
    {
        int length = value["info"].size();
        for (int index = 0; index < length; index++)
        {
            Info* personInfo = new Info();
            personInfo->autorelease();
            personInfo->name = value["info"][index]["name"].asString();
            personInfo->uid = value["info"][index]["id"].asString();
            personInfo->gender = value["gender"].asString();
            personInfo->occupation = value["occupation"].asString();
        }
    }
}

void CDUIShowView::httpTest()
{
    string url = "http://test.yuhaisong.com/test/index.php";
    CCHttpRequest* request = new CCHttpRequest();
    request->setTag("Getpage");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setUrl(url.c_str());
    request->setResponseCallback(this,httpresponse_selector(CDUIShowView::requestResult));
    CCHttpClient* httpClient = CCHttpClient::getInstance();
    httpClient->setTimeoutForConnect(30);
    httpClient->send(request);
    request->release();
}

void CDUIShowView::requestResult(CCHttpClient *client, CCHttpResponse *response)
{
    if (!response->isSucceed())
    {
        CCLog("Error reason: %s", response->getErrorBuffer());
        return;
    }
    //if (!strcmp("Getpage", response->getHttpRequest()->getTag()) && (response->getResponseCode() == 200))
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