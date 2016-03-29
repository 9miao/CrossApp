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

CDNewsTableCell* CDNewsTableCell::create(const std::string& identifier)
{
    CCLog("CDNewsTableCell");
    CDNewsTableCell* tableViewCell = new CDNewsTableCell();
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
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

bool CDNewsTableCell::initWithReuseIdentifier(const std::string& reuseIdentifier)
{
    if (!CATableViewCell::initWithReuseIdentifier(reuseIdentifier))
    {
        return false;
    }
    
    theImage = CommonUrlImageView::createWithLayout(DRectLayout(20, 200, 20, 20, DRectLayout::L_W_T_B));
    theImage->setTag(101);
    theImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    theImage->setImage(CAImage::create("image/HelloWorld.png"));
    this->getContentView()->addSubview(theImage);
    
    theTitle = CALabel::createWithLayout(DRectLayout(240, 150, 20, 40, DRectLayout::L_R_T_H));
	theTitle->setColor(CAColor_black);
    theTitle->setTextAlignment(CATextAlignmentLeft);
    theTitle->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    theTitle->setFontSize(32);
    theTitle->setTag(100);
    this->getContentView()->addSubview(theTitle);
    
    theDesc = CALabel::createWithLayout(DRectLayout(240, 150, 65, 40, DRectLayout::L_R_T_H));
	theDesc->setColor(CAColor_black);
    theDesc->setTextAlignment(CATextAlignmentLeft);
    theDesc->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    theDesc->setFontSize(24);
    theDesc->setTag(102);
	theDesc->setColor(CAColor_gray);
    theDesc->setLineSpacing(10);
    this->getContentView()->addSubview(theDesc);
    
    return true;
}

void CDNewsTableCell::setModel(const newsMsg &cellmodel)
{
    theImage->setImage(CAImage::create("image/HelloWorld.png"));
    theImage->setUrl(cellmodel.m_imageUrl);
    theTitle->setText(cellmodel.m_title);
    theDesc->setText(cellmodel.m_desc);
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

CDNewsViewController::~CDNewsViewController()
{
    
}

string CDNewsViewController::getSign(std::map<std::string,std::string> key_value)
{
    string appsecret = "c174cb1fda3491285be953998bb867a0";
    string tempStr = "";
    std::map<std::string,std::string>::iterator itr;
    for (itr=key_value.begin(); itr!=key_value.end(); itr++) {
        tempStr = tempStr+itr->first+itr->second;
    }
    tempStr = appsecret+tempStr+appsecret;
    CCLog("tempStr===%s",tempStr.c_str());
    string sign = MD5(tempStr).md5();
    for(int i=0;i<sign.length();i++)
    {
        if(sign[i]>='a'&&sign[i]<='z')
            sign[i]-=32;
        else if
            (sign[i]>='A'&&sign[i]<='Z')sign[i]+=32;
    }
    return sign;
}

void CDNewsViewController::viewDidLoad()
{
    if (m_msg.empty())
    {
        std::map<std::string,
        std::string> key_value;
        key_value["tag"] = menuTag[urlID];
        key_value["page"]= "1";
        key_value["limit"]= "20";
        key_value["appid"]="10000";
        key_value["sign_method"]="md5";
        string tempSign = getSign(key_value);
        CCLog("sign===%s",tempSign.c_str());
        key_value["sign"] = tempSign;
        string tempUrl = "http://api.9miao.com/news/";
        
        CommonHttpManager::getInstance()->send_post(tempUrl, key_value, this,
                                                   CommonHttpJson_selector(CDNewsViewController::onRequestFinished));
        
        
        p_pLoading = CAActivityIndicatorView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
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
    
    CAImageView* bg = CAImageView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_W_T_B));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("image/HelloWorld.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_W_T_B));
    btn5->setTitleColorForState(CAControlStateNormal,CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(CDNewsViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithLayout(DRectLayout(0, 0, 100, 40, DRectLayout::L_W_B_H));
	test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    test->setFontSize(24);
    test->setText("网络不给力，请点击屏幕重新加载～");
    p_alertView->addSubview(test);
    
}

void CDNewsViewController::buttonCallBack(CAControl* btn,DPoint point)
{
    this->getView()->removeSubview(p_alertView);
    p_alertView = NULL;
    std::map<std::string,
    std::string> key_value;
    key_value["tag"] = menuTag[urlID];
    key_value["page"]= "1";
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str());
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/news/";
    CommonHttpManager::getInstance()->send_post(tempUrl, key_value, this,
                                               CommonHttpJson_selector(CDNewsViewController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(CDNewsViewController::initNewsTableView));
    }
}

void CDNewsViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg.clear();
        m_page.clear();
        for (int i=0; i<3; i++) {
            newsPage temp_page;
            temp_page.m_title   = value[i]["title"].asString();
            temp_page.m_pic     = value[i]["image"].asString();
            temp_page.m_url     = value[i]["url"].asString();
            m_page.push_back(temp_page);
        }
        for (int index = 3; index < length; index++)
        {
            newsMsg temp_msg;
            temp_msg.m_title = value[index]["title"].asString();
            temp_msg.m_desc = value[index]["desc"].asString();
            temp_msg.m_url = value[index]["url"].asString();
            temp_msg.m_imageUrl = value[index]["image"].asString();
            m_msg.push_back(temp_msg);
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
        const CSJson::Value& value = json["result"];
        int length = value.size();
        for (int index = 0; index < length; index++)
        {
            newsMsg temp_msg;
            temp_msg.m_title = value[index]["title"].asString();
            temp_msg.m_desc = value[index]["desc"].asString();
            temp_msg.m_url = value[index]["url"].asString();
            temp_msg.m_imageUrl = value[index]["image"].asString();
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
    
    p_TableView= CATableView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
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
    p_TableView->setTableHeaderView(tempview);
    p_TableView->setTableHeaderHeight(this->getView()->getBounds().size.width/2);
    
    
    CAVector<CAView* > viewList;
    CommonUrlImageView* temImage0 = CommonUrlImageView::createWithImage(CAImage::create("image/HelloWorld.png"));
    temImage0->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    temImage0->setUrl(m_page[m_page.size()-1].m_pic);
    viewList.pushBack(temImage0);
    
    for (int i=0; i<m_page.size(); i++)
    {
        //初始化viewList
        CommonUrlImageView* temImage = CommonUrlImageView::createWithImage(CAImage::create("image/HelloWorld.png"));
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        temImage->setUrl(m_page[i].m_pic);
        viewList.pushBack(temImage);
    }
    
    CommonUrlImageView* temImage1 = CommonUrlImageView::createWithImage(CAImage::create("image/HelloWorld.png"));
    temImage1->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    temImage1->setUrl(m_page[0].m_pic);
    viewList.pushBack(temImage1);
    
    p_PageView = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageView->setViews(viewList);
    p_PageView->setPageViewDelegate(this);
    p_PageView->setTouchEnabled(true);
    tempview->addSubview(p_PageView);
    p_PageView->setCurrPage(1, false);
    
    CAView* bg = CAView::createWithColor(ccc4(0, 0, 0, 128));
    bg->setLayout(DRectLayout(0, 0, 0, 50, DRectLayout::L_R_B_H));
    tempview->addSubview(bg);
    
    pageControl = CAPageControl::createWithLayout(DRectLayout(40, 100, 0, 0, DRectLayout::R_W_T_B));
    pageControl->setNumberOfPages((int)m_page.size());
    pageControl->setPageIndicatorImage(CAImage::create("image/pagecontrol_selected.png"));
    pageControl->setCurrIndicatorImage(CAImage::create("image/pagecontrol_bg.png"));
    pageControl->setPageIndicatorTintColor(CAColor_gray);
    //pageControl->setCurrentPageIndicatorTintColor(CAColor_clear);
    pageControl->addTarget(this, CAControl_selector(CDNewsViewController::pageControlCallBack));
    bg->addSubview(pageControl);
    
    
    if (m_page.size()>0)
    {
        pageViewTitle = CALabel::createWithLayout(DRectLayout(10, 140, 0, 0, DRectLayout::L_R_T_B));
        pageViewTitle->setText(m_page[0].m_title);
		pageViewTitle->setColor(CAColor_white);
        pageViewTitle->setFontSize(28);
        tempview->addSubview(pageViewTitle);
    }
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
    RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(_webController, true);
    _webController->initWebView(m_msg[row].m_url);
}

void CDNewsViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDNewsViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{

    CDNewsTableCell* cell = dynamic_cast<CDNewsTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CDNewsTableCell::create("CrossApp");
    }
    cell->setModel(m_msg[row]);

    return cell;
    
}

void CDNewsViewController::tableViewWillDisplayCellAtIndex(CATableView* table, CATableViewCell* cell, unsigned int section, unsigned int row)
{
    /*
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
  */
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
    return (unsigned int)m_msg.size();
}

unsigned int CDNewsViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return 170;
}

void CDNewsViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    std::map<std::string,
    std::string> key_value;
    key_value["tag"] = menuTag[urlID];
    key_value["page"]= "1";
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str());
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/news/";
    CommonHttpManager::getInstance()->send_post(tempUrl, key_value, this,
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
    p_section++;
    std::map<std::string,
    std::string> key_value;
    key_value["tag"] = menuTag[urlID];
    key_value["page"]= crossapp_format_string("%d",p_section);
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str());
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/news/";
    CommonHttpManager::getInstance()->send_post(tempUrl, key_value, this,
                                               CommonHttpJson_selector(CDNewsViewController::onRefreshRequestFinished));
}

void CDNewsViewController::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    CDWebViewController* _webController = new CDWebViewController();
    _webController->init();
    _webController->setTitle(" ");
    
    _webController->autorelease();
    RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(_webController, true);
    _webController->initWebView(m_page[index-1].m_url);
}

void CDNewsViewController::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void CDNewsViewController::pageViewDidEndTurning(CAPageView* pageView)
{
    if (pageView->getCurrPage()==0)
    {
        pageView->setCurrPage((int)m_page.size(), false);
    }
    else if(pageView->getCurrPage()==m_page.size()+1)
    {
        pageView->setCurrPage(1, false);
    }
    pageControl->setCurrentPage(pageView->getCurrPage()-1);
    pageControl->updateCurrentPageDisplay();
    if (m_page.size()>0) {
        pageViewTitle->setText(m_page[pageView->getCurrPage()-1].m_title);
    }
}

void CDNewsViewController::pageControlCallBack(CrossApp::CAControl *btn, CrossApp::DPoint point){
    CAPageControl* button = (CAPageControl*)btn;
    p_PageView->setCurrPage(button->getCurrentPage()+1, true);
    if (m_page.size()>0) {
        pageViewTitle->setText(m_page[button->getCurrentPage()].m_title);
    }
}