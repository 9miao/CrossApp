//
//  CDNewsImageController.cpp
//  Test
//
//  Created by renhongguang on 15/4/17.
//
//

#include "CDNewsImageController.h"
#include "CDShowNewsImage.h"

CDNewsImageTableCell::CDNewsImageTableCell()
{
    cell_tag = 0;
    this->setAllowsSelected(false);
}

CDNewsImageTableCell::~CDNewsImageTableCell()
{
    
}

CDNewsImageTableCell* CDNewsImageTableCell::create(const std::string& identifier, const DRect& _rect)
{
    CDNewsImageTableCell* tableViewCell = new CDNewsImageTableCell();
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
        tableViewCell->setFrame(_rect);
        tableViewCell->autorelease();
        return tableViewCell;
    }
    CC_SAFE_DELETE(tableViewCell);
    return NULL;
}

void CDNewsImageTableCell::highlightedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 128)));
}

void CDNewsImageTableCell::selectedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 128)));
}
void CDNewsImageTableCell::initWithCell(int num)
{
    cell_tag = CCRANDOM_0_1()*4+1;
    DSize _size = this->getFrame().size;
    CALabel* test = CALabel::createWithFrame(DRect(40,_size.height-150,_size.width-90,40));
	test->setColor(CAColor_black);
    test->setTextAlignment(CATextAlignmentLeft);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    test->setFontSize(32);
    test->setBold(true);
    test->setTag(100);
    this->addSubview(test);
    
    CALabel* dsc = CALabel::createWithFrame(DRect(40,_size.height-90,_size.width-90,70));
	dsc->setColor(CAColor_black);
    dsc->setTextAlignment(CATextAlignmentLeft);
    dsc->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    dsc->setFontSize(26);

    dsc->setTag(101);
	dsc->setColor(ccc4(200, 200, 200, 255));
    this->addSubview(dsc);
    
    CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(DRect(0,5,_size.width,_size.height-180));
    temImage->setTag(200);
    temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    
    CommonUrlImageView* temImage1 = CommonUrlImageView::createWithFrame(DRect(0,5,_size.width,_size.height-180));
    temImage1->setTag(201);
    temImage1->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    
    CommonUrlImageView* temImage2 = CommonUrlImageView::createWithFrame(DRect(0,5,_size.width,_size.height-180));
    temImage2->setTag(202);
    temImage2->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    
    CommonUrlImageView* temImage3 = CommonUrlImageView::createWithFrame(DRect(0,5,_size.width,_size.height-180));
    temImage3->setTag(203);
    temImage3->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    
    this->addSubview(temImage);
    this->addSubview(temImage1);
    this->addSubview(temImage2);
    this->addSubview(temImage3);
}

CDNewsImageController::CDNewsImageController(int index)
:p_TableView(NULL)
,p_pLoading(NULL)
,p_section(1)
{
    urlID = index;
}

CDNewsImageController::~CDNewsImageController()
{

}

string CDNewsImageController::getSign(std::map<std::string,std::string> key_value)
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

void CDNewsImageController::viewDidLoad()
{
    CCLog("NewsImageController");
    winSize = this->getView()->getBounds().size;
    m_ImageMsg.clear();
    m_ImageNum.clear();

    if (m_ImageMsg.empty()) {
//        std::map<std::string,
//        std::string> key_value;
//        char temurl[200];
//        sprintf(temurl, "http://h5.9miao.com/pic?num=1&tag=%s",imageTag[urlID]);
        
        std::map<std::string,std::string> key_value;
        key_value["tag"] = imageTag[urlID];
        key_value["page"]= "1";
        key_value["limit"]= "20";
        key_value["appid"]="10000";
        key_value["sign_method"]="md5";
        string tempSign = getSign(key_value);
        CCLog("sign===%s",tempSign.c_str());
        key_value["sign"] = tempSign;
        string tempUrl = "http://api.9miao.com/newsgirlpic/";
        CommonHttpManager::getInstance()->send_get(tempUrl, key_value, this,
                                                   CommonHttpJson_selector(CDNewsImageController::onRequestFinished));
        {
            p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(winSize.width/2,winSize.height/2,50,50));
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
            p_pLoading->setLoadingMinTime(0.5f);
            p_pLoading->setTargetOnCancel(this, callfunc_selector(CDNewsImageController::initImageTableView));
        }
    }else{
        this->initImageTableView();
    }

}

void CDNewsImageController::initImageTableView()
{
    if (m_ImageMsg.empty()) {
        showAlert();
        return;
    }

    if (p_TableView!=NULL)
    {
        this->getView()->removeSubview(p_TableView);
        p_TableView = NULL;
    }
//    p_TableView= CATableView::createWithFrame(DRect(0, 0, winSize.width, winSize.height));
    p_TableView= CATableView::createWithLayout(DLayout(0,0,0,0,DLayout::L_R_T_B));
    p_TableView->setTableViewDataSource(this);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setAllowsSelection(true);
    p_TableView->setScrollViewDelegate(this);
    p_TableView->setAllowsMultipleSelection(false);
    p_TableView->setSeparatorViewHeight(40);
    p_TableView->setSeparatorColor(ccc4(240,240,240,255));
    this->getView()->addSubview(p_TableView);
    CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    refreshDiscount->setLabelColor(CAColor_black);
    CAPullToRefreshView *refreshDiscount1 = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    refreshDiscount1->setLabelColor(CAColor_black);
    p_TableView->setFooterRefreshView(refreshDiscount);
    p_TableView->setHeaderRefreshView(refreshDiscount1);
}

void CDNewsImageController::showAlert()
{
    p_alertView = CAView::createWithFrame(this->getView()->getBounds());
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect(0,0,winSize.width,winSize.height));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("image/HelloWorld.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setCenter(DRect(winSize.width/2, winSize.height/2, winSize.width, winSize.height));
    btn5->setTitleColorForState(CAControlStateNormal,CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(CDNewsImageController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(winSize.width/2,winSize.height-100,winSize.width,40));
	test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    test->setFontSize(24);
    test->setText("网络不给力，请点击屏幕重新加载～");
    p_alertView->addSubview(test);
    
}

void CDNewsImageController::buttonCallBack(CAControl* btn,DPoint point)
{
    this->getView()->removeSubview(p_alertView);
    p_alertView = NULL;
//    std::map<std::string,
//    std::string> key_value;
//    char temurl[200];
//    sprintf(temurl, "http://h5.9miao.com/pic?num=1&tag=%s",imageTag[urlID]);
    std::map<std::string,std::string> key_value;
    key_value["tag"] = imageTag[urlID];
    key_value["page"]= "1";
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str());
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/newsgirlpic/";
    CommonHttpManager::getInstance()->send_get(tempUrl, key_value, this,
                                               CommonHttpJson_selector(CDNewsImageController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(winSize.width/2,winSize.height/2,50,50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(CDNewsImageController::initImageTableView));
    }
}

void CDNewsImageController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    std::map<std::string,std::string> key_value;
    key_value["tag"] = imageTag[urlID];
    key_value["page"]= "1";
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str());
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/newsgirlpic/";
    CommonHttpManager::getInstance()->send_get(tempUrl, key_value, this,
                                               CommonHttpJson_selector(CDNewsImageController::onRequestFinished));
}

void CDNewsImageController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    p_section++;
    std::map<std::string,std::string> key_value;
    key_value["tag"] = imageTag[urlID];
    key_value["page"]= crossapp_format_string("%d",p_section);
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str());
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/newsgirlpic/";
    CommonHttpManager::getInstance()->send_get(tempUrl, key_value, this,
                                               CommonHttpJson_selector(CDNewsImageController::onRefreshRequestFinished));
}

void CDNewsImageController::scrollViewStopMoved(CrossApp::CAScrollView *view)
{
    CAVector<CATableViewCell*> temVec =  p_TableView->displayingTableCell();
    for(int i=0;i<temVec.size();i++)
    {
        CDNewsImageTableCell* cell = (CDNewsImageTableCell*)temVec.at(i);
        CCLog("cell-index===%d",cell->getRow());
        int row = cell->getRow();
        int img_num = m_ImageNum[row];
        
        for (int i=0; i<img_num; i++)
        {
            CommonUrlImageView* temImage = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(200 + i));
            if (temImage)
            {
                temImage->setUrl(m_ImageMsg[row].m_imageUrl[1 + i]);
            }
        }
    }
}

void CDNewsImageController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        
        m_ImageMsg.clear();
        for (int index = 0; index < length; index++)
        {
            newsImage temp_msg;
            temp_msg.m_title = value[index]["title"].asString();
            for (int i=0; i<value[index]["picon"].size(); i++) {
                string temp_pic = value[index]["picon"][i]["pic"].asString();
                string temp_dsc = value[index]["picon"][i]["desc"].asString();
                temp_msg.m_imageUrl.push_back(temp_pic);
                temp_msg.m_imageDesc.push_back(temp_dsc);
            }

            m_ImageMsg.push_back(temp_msg);
            m_ImageNum.push_back((int)getRandNum());
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

void CDNewsImageController::onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["msg"];
        int length = value.size();
        for (int index = 0; index < length; index++)
        {
            newsImage temp_msg;
            temp_msg.m_title = value[index]["title"].asString();
            for (int i=0; i<value[index]["picon"].size(); i++) {
                string temp_pic = value[index]["picon"][i]["pic"].asString();
                string temp_dsc = value[index]["picon"][i]["desc"].asString();
                temp_msg.m_imageUrl.push_back(temp_pic);
                temp_msg.m_imageDesc.push_back(temp_dsc);
            }
            
            m_ImageMsg.push_back(temp_msg);
            m_ImageNum.push_back((int)getRandNum());
        }
        
    }else{
        p_section--;
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

void CDNewsImageController::viewDidUnload()
{
    
}

void CDNewsImageController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    //CCLog("title====%d",(int)getRandNum());
    CDShowNewsImage* _controller = new CDShowNewsImage();
    _controller->init();
    _controller->setTitle(" ");
    
    _controller->autorelease();
    RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(_controller, true);
    _controller->initNewsImageView(m_ImageMsg[row]);
}

void CDNewsImageController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDNewsImageController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    
    DSize _size = cellSize;
    CDNewsImageTableCell* cell = dynamic_cast<CDNewsImageTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CDNewsImageTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
        cell->initWithCell(1);
    }
    CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
    cellText->setText(m_ImageMsg[row].m_title);
    
    CALabel* cellTextdsc = (CALabel*)cell->getSubviewByTag(101);
    cellTextdsc->setText(m_ImageMsg[row].m_imageDesc[1]);
    
    int img_num = m_ImageNum[row];
    
    if (img_num==1)
    {
        CommonUrlImageView* temImage = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(200));
        if (temImage)
        {
            temImage->setVisible(true);
            temImage->setFrame(DRect(0,5,_size.width,_size.height-180));
            temImage->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage->setUrl(m_ImageMsg[row].m_imageUrl[1]);
        }
        CommonUrlImageView* temImage1 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(201));
        CommonUrlImageView* temImage2 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(202));
        CommonUrlImageView* temImage3 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(203));
        temImage1->setVisible(false);
        temImage2->setVisible(false);
        temImage3->setVisible(false);
    }
    else if(img_num==2)
    {
        CommonUrlImageView* temImage = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(200));
        if (temImage)
        {
            temImage->setVisible(true);
            temImage->setFrame(DRect(0,5,_size.width/2-1,_size.height-180));
            temImage->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage->setUrl(m_ImageMsg[row].m_imageUrl[1]);
        }
        CommonUrlImageView* temImage1 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(201));
        if (temImage1)
        {
            temImage1->setVisible(true);
            temImage1->setFrame(DRect(_size.width/2+1,5,_size.width/2,_size.height-180));
            temImage1->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage1->setUrl(m_ImageMsg[row].m_imageUrl[2]);
        }

        CommonUrlImageView* temImage2 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(202));
        CommonUrlImageView* temImage3 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(203));;
        temImage2->setVisible(false);
        temImage3->setVisible(false);
    }
    else if(img_num==3)
    {
        CommonUrlImageView* temImage = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(200));
        if (temImage)
        {
            temImage->setVisible(true);
            temImage->setFrame(DRect(0,5,_size.width/2-1,_size.height-180));
            temImage->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage->setUrl(m_ImageMsg[row].m_imageUrl[1]);
        }
        CommonUrlImageView* temImage1 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(201));
        if (temImage1)
        {
            temImage1->setVisible(true);
            temImage1->setFrame(DRect(_size.width/2+1,5,_size.width/2,(_size.height-180)/2-1));
            temImage1->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage1->setUrl(m_ImageMsg[row].m_imageUrl[2]);
        }
        
        CommonUrlImageView* temImage2 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(202));
        if (temImage2)
        {
            temImage2->setVisible(true);
            temImage2->setFrame(DRect(_size.width/2+1,5+(_size.height-180)/2+2,_size.width/2,(_size.height-180)/2));
            temImage2->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage2->setUrl(m_ImageMsg[row].m_imageUrl[3]);
        }
        CommonUrlImageView* temImage3 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(203));;
        temImage3->setVisible(false);

    }
    else if(img_num==4)
    {
        CommonUrlImageView* temImage = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(200));
        if (temImage)
        {
            temImage->setVisible(true);
            temImage->setFrame(DRect(0,5,_size.width/2-1,(_size.height-180)/2-1));
            temImage->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage->setUrl(m_ImageMsg[row].m_imageUrl[1]);
        }
        CommonUrlImageView* temImage1 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(201));
        if (temImage1)
        {
            temImage1->setVisible(true);
            temImage1->setFrame(DRect(_size.width/2+1,5,_size.width/2,(_size.height-180)/2-1));
            temImage1->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage1->setUrl(m_ImageMsg[row].m_imageUrl[2]);
        }
        
        CommonUrlImageView* temImage2 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(202));
        if (temImage2)
        {
            temImage2->setVisible(true);
            temImage2->setFrame(DRect(0,5+(_size.height-180)/2+2,_size.width/2-1,(_size.height-180)/2));
            temImage2->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage2->setUrl(m_ImageMsg[row].m_imageUrl[3]);
        }
        
        CommonUrlImageView* temImage3 = dynamic_cast<CommonUrlImageView*>(cell->getSubviewByTag(203));
        if (temImage3)
        {
            temImage3->setVisible(true);
            temImage3->setFrame(DRect(_size.width/2+1,5+(_size.height-180)/2+2,_size.width/2,(_size.height-180)/2));
            temImage3->setImage(CAImage::create("image/HelloWorld.png"));
            //temImage3->setUrl(m_ImageMsg[row].m_imageUrl[4]);
        }
    }
    return cell;
    
}

unsigned int CDNewsImageController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return m_ImageMsg.size();
}

unsigned int CDNewsImageController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return 650;
}

float CDNewsImageController::getRandNum()
{
    return CCRANDOM_0_1()*4+1;
}