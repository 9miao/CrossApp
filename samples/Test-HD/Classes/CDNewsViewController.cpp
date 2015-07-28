//
//  CDNewsViewController.cpp
//  Test
//
//  Created by renhongguang on 15/4/16.
//
//

#include "CDNewsViewController.h"
#include "CDWebViewController.h"

extern int page_index;
float temp_time = 0;
CDNewsTableCell::CDNewsTableCell()
:theTitle(NULL),
theDesc(NULL),
theImage(NULL)
{
    this->setAllowsSelected(false);
}

CDNewsTableCell::~CDNewsTableCell()
{
    
}

CDNewsTableCell* CDNewsTableCell::create(const std::string& identifier, const CADipRect& _rect)
{
    CDNewsTableCell* tableViewCell = new CDNewsTableCell();
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
        tableViewCell->setFrame(_rect);
        tableViewCell->autorelease();
        return tableViewCell;
    }
    CC_SAFE_DELETE(tableViewCell);
    return NULL;
}

void CDNewsTableCell::highlightedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void CDNewsTableCell::selectedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void CDNewsTableCell::initWithCell()
{
    CADipSize _size = this->getFrame().size;
    theTitle = CALabel::createWithFrame(CADipRect(240, 20, _size.width/2+50, 40));
    theTitle->setColor(CAColor_black);
    theTitle->setTextAlignment(CATextAlignmentLeft);
    theTitle->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    theTitle->setFontSize(_px(32));
    theTitle->setTag(100);
    this->getContentView()->addSubview(theTitle);
    
    theDesc = CALabel::createWithFrame(CADipRect(240, 65, _size.width/2, 40));
    theDesc->setColor(CAColor_black);
    theDesc->setTextAlignment(CATextAlignmentLeft);
    theDesc->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    theDesc->setFontSize(_px(24));
    theDesc->setTag(102);
    theDesc->setColor(CAColor_gray);
    theDesc->setLineSpacing(10);
    this->getContentView()->addSubview(theDesc);
    
    theImage = CommonUrlImageView::createWithCenter(CADipRect(120, _size.height/2, 200, _size.height-40));
    theImage->setTag(101);
    theImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    theImage->setImage(CAImage::create("image/HelloWorld.png"));
    this->getContentView()->addSubview(theImage);
}

void CDNewsTableCell::setModel(const newsMsg &cellmodel)
{
    theTitle->setText(cellmodel.m_title);
    theDesc->setText(cellmodel.m_desc);
    theImage->setImage(CAImage::create("image/HelloWorld.png"));
    theImage->setUrl(cellmodel.m_imageUrl[0]);
}

CDNewsViewController::CDNewsViewController(int index)
:p_PageView(NULL)
,p_TableView(NULL)
,p_pLoading(NULL)
,p_section(1)
{
    urlID = index;
    m_msg.clear();
}

CDNewsViewController::~CDNewsViewController(){
    
}

void CDNewsViewController::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    if (m_msg.empty())
    {
        std::map<std::string,
        std::string> key_value;
        char temurl[200];
        sprintf(temurl, "http://123.183.220.246:8090/getdemocon/?num=1&tag=%s",menuTag[urlID]);
        CommonHttpManager::getInstance()->send_get(temurl, key_value, this,
                                                   CommonHttpJson_selector(CDNewsViewController::onRequestFinished));
        
        
        p_pLoading = CAActivityIndicatorView::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,50,50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(CDNewsViewController::initNewsTableView));
    }
    else
    {
        this->initNewsTableView();
    }
}

void CDNewsViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView= NULL;
    }

    p_alertView = CAView::createWithFrame(this->getView()->getBounds());
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(CADipRect(0,0,winSize.width,winSize.height));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("image/HelloWorld.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setCenter(CADipRect(winSize.width/2, winSize.height/2, winSize.width, winSize.height));
    btn5->setTitleColorForState(CAControlStateNormal,CAColor_white);
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(CDNewsViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(CADipRect(winSize.width/2,
                                                        winSize.height-100,
                                                        winSize.width,
                                                        40));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    test->setFontSize(_px(24));
    test->setText("网络不给力，请点击屏幕重新加载～");
    p_alertView->addSubview(test);
    
}

void CDNewsViewController::buttonCallBack(CAControl* btn,CCPoint point)
{
    this->getView()->removeSubview(p_alertView);
    p_alertView = NULL;
    std::map<std::string,
    std::string> key_value;
    char temurl[200];
    sprintf(temurl, "http://123.183.220.246:8090/getdemocon/?num=1&tag=%s",menuTag[urlID]);
    CommonHttpManager::getInstance()->send_get(temurl, key_value, this,
                                               CommonHttpJson_selector(CDNewsViewController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(CADipRect(winSize.width/2,winSize.height/2,50,50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(CDNewsViewController::initNewsTableView));
    }
}

void CDNewsViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["msg"];
        const CSJson::Value& value1 = json["headmsg"];
        int length = value.size();
        int length1 = value1.size();
        m_msg.clear();
        m_page.clear();
        for (int index = 0; index < length; index++)
        {
            newsMsg temp_msg;
            temp_msg.m_title = value[index]["title"].asString();
            temp_msg.m_desc = value[index]["desc"].asString();
            temp_msg.m_url = value[index]["url"].asString();
            for(int i=0 ;i<value[index]["pic"].size();i++){
                temp_msg.m_imageUrl.push_back(value[index]["pic"][i].asString());
            }
            m_msg.push_back(temp_msg);
        }
        
        for (int i=0; i<length1; i++) {
            newsPage temp_page;
            temp_page.m_title = value1[i]["title"].asString();
            temp_page.m_pic = value1[i]["pic"].asString();
            temp_page.m_url = value1[i]["url"].asString();
            m_page.push_back(temp_page);
        }

    }
    
    CATabBarController* tabBarController = this->getTabBarController();
    
    for (int i=0; i<5; i++)
    {
        CDNewsViewController* catalog = dynamic_cast<CDNewsViewController*>(tabBarController->getViewControllerAtIndex(i));
        if (catalog && catalog->p_pLoading)
        {
            catalog->p_pLoading->stopAnimating();
        }
    }
    
    if (p_TableView)
    {
        p_TableView->reloadData();
    }
}

void CDNewsViewController::onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["msg"];
        int length = value.size();
        for (int index = 0; index < length; index++)
        {
            newsMsg temp_msg;
            temp_msg.m_title = value[index]["title"].asString();
            temp_msg.m_desc = value[index]["desc"].asString();
            temp_msg.m_url = value[index]["url"].asString();
            for(int i=0 ;i<value[index]["pic"].size();i++){
                temp_msg.m_imageUrl.push_back(value[index]["pic"][i].asString());
            }
            m_msg.push_back(temp_msg);
        }

    }
    
    do
    {
        CC_BREAK_IF(p_pLoading == NULL);
        if (p_pLoading->isAnimating())
        {
            p_pLoading->stopAnimating();
        }
        else
        {
            p_TableView->reloadData();
        }
    }
    while (0);
}

void CDNewsViewController::initNewsTableView()
{
    if (m_page.empty())
    {
        showAlert();
        return;
    }
    if (p_TableView!=NULL)
    {
        this->getView()->removeSubview(p_TableView);
        p_TableView = NULL;
    }
    
    p_TableView= CATableView::createWithFrame(CADipRect(0, 0, winSize.width, winSize.height));
    p_TableView->setTableViewDataSource(this);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setScrollViewDelegate(this);
    p_TableView->setAllowsSelection(true);
    this->getView()->addSubview(p_TableView);
    CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    refreshDiscount->setLabelColor(CAColor_black);
    CAPullToRefreshView *refreshDiscount1 = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    refreshDiscount1->setLabelColor(CAColor_black);
    p_TableView->setFooterRefreshView(refreshDiscount);
    p_TableView->setHeaderRefreshView(refreshDiscount1);
    
    initNewsPageView();
}

void CDNewsViewController::initNewsPageView()
{
    //初始化pageView
    CAView* tempview = CAView::create();
    
    p_PageView = CAPageView::createWithFrame(CADipRect(0,0,winSize.width,winSize.width/2), CAPageViewDirectionHorizontal);
    p_PageView->setPageViewDelegate(this);
    p_PageView->setTouchEnabled(true);
    CAVector<CAView* > viewList;
    CommonUrlImageView* temImage0 = CommonUrlImageView::createWithFrame(CADipRect(winSize.width/2,100,winSize.width,winSize.width/2));
    temImage0->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    temImage0->setImage(CAImage::create("image/HelloWorld.png"));
    temImage0->setUrl(m_page[m_page.size()-1].m_pic);
    
    viewList.pushBack(temImage0);
    for (int i=0; i<m_page.size(); i++) {
        //初始化viewList
        CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(CADipRect(winSize.width/2,100,winSize.width,winSize.width/2));
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        temImage->setImage(CAImage::create("image/HelloWorld.png"));
        temImage->setUrl(m_page[i].m_pic);
        viewList.pushBack(temImage);
    }
    CommonUrlImageView* temImage1 = CommonUrlImageView::createWithFrame(CADipRect(winSize.width/2,100,winSize.width,winSize.width/2));
    temImage1->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    temImage1->setImage(CAImage::create("image/HelloWorld.png"));
    temImage1->setUrl(m_page[0].m_pic);
    viewList.pushBack(temImage1);
    
    p_PageView->setViews(viewList);
    tempview->addSubview(p_PageView);
    p_PageView->setCurrPage(1, false);
    pageControl = CAPageControl::createWithCenter(CADipRect(winSize.width-80, winSize.width/2-25, 100, 50));
    pageControl->setNumberOfPages((int)m_page.size());
    pageControl->setPageIndicatorImage(CAImage::create("image/pagecontrol_selected.png"));
    pageControl->setCurrIndicatorImage(CAImage::create("image/pagecontrol_bg.png"));
    pageControl->setPageIndicatorTintColor(CAColor_gray);
    //pageControl->setCurrentPageIndicatorTintColor(CAColor_clear);
    pageControl->addTarget(this, CAControl_selector(CDNewsViewController::pageControlCallBack));
    tempview->addSubview(pageControl);
    
    CAView* bg = CAView::createWithColor(ccc4(0, 0, 0, 128));
    bg->setFrame(CADipRect(0,winSize.width/2-50,winSize.width,50));
    tempview->addSubview(bg);
    
    if (m_page.size()>0) {
        pageViewTitle = CALabel::createWithFrame(CADipRect(10, winSize.width/2-40, winSize.width-50, 50));
        pageViewTitle->setText(m_page[0].m_title);
        pageViewTitle->setColor(CAColor_white);
        pageViewTitle->setFontSize(_px(28));
        tempview->addSubview(pageViewTitle);
    }
    
    p_TableView->setTableHeaderView(tempview);
    p_TableView->setTableHeaderHeight(_px(winSize.width/2));
}

void CDNewsViewController::viewDidUnload()
{
    
}

void CDNewsViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    CDWebViewController* _webController = new CDWebViewController();
    _webController->init();
    _webController->setTitle(" ");
    
    _webController->autorelease();
    //RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(_webController, true);
    _webController->initWebView(m_msg[row].m_url);
}

void CDNewsViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDNewsViewController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CADipSize _size = cellSize;
    CDNewsTableCell* cell = dynamic_cast<CDNewsTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CDNewsTableCell::create("CrossApp", CADipRect(0, 0, _size.width, _size.height));
        cell->initWithCell();
    }
    cell->setModel(m_msg[row]);

    return cell;
    
}

void CDNewsViewController::tableViewWillDisplayCellAtIndex(CATableView* table, CATableViewCell* cell, unsigned int section, unsigned int row)
{
    if (cell != NULL)
    {
        temp_time+=0.02f;
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(temp_time);
        CAViewAnimation::setAnimationDidStopSelector(this,CAViewAnimation0_selector(CDNewsViewController::tempCallBack));
        CAViewAnimation::commitAnimations();
        
        cell->getContentView()->setScale(0.8f);
        cell->getContentView()->setRotationY(-180);
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        CAViewAnimation::setAnimationDelay(temp_time);
        cell->getContentView()->setScale(1.0f);
        cell->getContentView()->setRotationY(0);
        //执行动画
        CAViewAnimation::commitAnimations();
        
    }
}

void CDNewsViewController::tempCallBack()
{
    temp_time-=0.02f;
}

unsigned int CDNewsViewController::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int CDNewsViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return m_msg.size();
}

unsigned int CDNewsViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(170);
}

void CDNewsViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    std::map<std::string,
    std::string> key_value;
    char temurl[200];
    sprintf(temurl, "http://123.183.220.246:8090/getdemocon/?num=1&tag=%s",menuTag[urlID]);
    CommonHttpManager::getInstance()->send_get(temurl, key_value, this,
                                               CommonHttpJson_selector(CDNewsViewController::onRequestFinished));
    CATabBarItem* item = this->getTabBarItem();
    CCLog("BadgeValue====%s",item->getBadgeValue().c_str());
    if (!item->getBadgeValue().empty()) {
        item->setBadgeValue("");
        this->setTabBarItem(item);
    }
}

void CDNewsViewController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    std::map<std::string,
    std::string> key_value;
    char temurl[200];
    p_section++;
    sprintf(temurl, "http://123.183.220.246:8090/getdemocon/?num=%d&tag=%s",p_section,menuTag[urlID]);
    CommonHttpManager::getInstance()->send_get(temurl, key_value, this,
                                               CommonHttpJson_selector(CDNewsViewController::onRefreshRequestFinished));
}

void CDNewsViewController::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const CCPoint& point)
{
    CDWebViewController* _webController = new CDWebViewController();
    _webController->init();
    _webController->setTitle(" ");
    
    _webController->autorelease();
    //RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(_webController, true);
    _webController->initWebView(m_page[index-1].m_url);
}

void CDNewsViewController::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void CDNewsViewController::pageViewDidEndTurning(CAPageView* pageView)
{
    if (pageView->getCurrPage()==0) {
        pageView->setCurrPage(m_page.size(), false);
    }else if(pageView->getCurrPage()==m_page.size()+1){
        pageView->setCurrPage(1, false);
    }
    pageControl->setCurrentPage(pageView->getCurrPage()-1);
    pageControl->updateCurrentPageDisplay();
    if (m_page.size()>0) {
        pageViewTitle->setText(m_page[pageView->getCurrPage()-1].m_title);
    }
}

void CDNewsViewController::pageControlCallBack(CrossApp::CAControl *btn, CrossApp::CCPoint point){
    CAPageControl* button = (CAPageControl*)btn;
    p_PageView->setCurrPage(button->getCurrentPage()+1, true);
    if (m_page.size()>0) {
        pageViewTitle->setText(m_page[button->getCurrentPage()].m_title);
    }
}