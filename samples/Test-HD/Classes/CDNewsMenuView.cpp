//
//  CDNewsMenuView.cpp
//  Test-HD
//
//  Created by renhongguang on 15/8/7.
//
//

#include "CDNewsMenuView.h"
#include "CDNewsViewController.h"
#include "CDNewsAboutController.h"

int newsPageID = 0;

CDNewsMenuTableCell::CDNewsMenuTableCell(int row)
{
    imageid = row;
}

CDNewsMenuTableCell::~CDNewsMenuTableCell()
{
    
}

CDNewsMenuTableCell* CDNewsMenuTableCell::create(const std::string& identifier, const CADipRect& _rect,int row)
{
    CDNewsMenuTableCell* tableViewCell = new CDNewsMenuTableCell(row);
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
        tableViewCell->setFrame(_rect);
        tableViewCell->autorelease();
        return tableViewCell;
    }
    CC_SAFE_DELETE(tableViewCell);
    return NULL;
}

void CDNewsMenuTableCell::setImageID(int _id)
{
    imageid = _id;
}

void CDNewsMenuTableCell::highlightedTableViewCell()
{
    char tempstr[50];
    sprintf(tempstr, "image/new_btn_down_%d.png",imageid);
    CAImageView* im = CAImageView::createWithFrame(CADipRect(0,0,233,116));
    im->setImage(CAImage::create(tempstr));
    this->setBackgroundView(im);
}

void CDNewsMenuTableCell::selectedTableViewCell()
{
    char tempstr[50];
    sprintf(tempstr, "image/new_btn_down_%d.png",imageid);
    CAImageView* im = CAImageView::createWithFrame(CADipRect(0,0,233,116));
    im->setImage(CAImage::create(tempstr));
    this->setBackgroundView(im);
}

void CDNewsMenuTableCell::normalTableViewCell()
{
    char tempstr[50];
    sprintf(tempstr, "image/new_btn_normal_%d.png",imageid);
    CCLog("imageid==%d",imageid);
    CAImageView* im = CAImageView::createWithFrame(CADipRect(0,0,233,116));
    im->setImage(CAImage::create(tempstr));
    this->setBackgroundView(im);
}

CDNewsMenuView::CDNewsMenuView()
{
    
}

CDNewsMenuView::~CDNewsMenuView()
{
    
}

void CDNewsMenuView::viewDidLoad()
{
    this->getView()->removeAllSubviews();
    this->getView()->setColor(ccc4(234,234,234,255));
    size = this->getView()->getBounds().size;
    
    CAImageView* icon = CAImageView::createWithCenter(CADipRect(size.width/2,200,278,88));
    icon->setImage(CAImage::create("image/news_icon.png"));
    this->getView()->addSubview(icon);
    
    CAButton* btn_back = CAButton::create(CAButtonTypeSquareRect);
    btn_back->setCenter(CADipRect(150, size.height-200, 40, 40));
    btn_back->setTag(0);
    btn_back->setTitleColorForState(CAControlStateNormal,CAColor_white);
    btn_back->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/news_close.png")));
    btn_back->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/news_close.png")));
    btn_back->addTarget(this, CAControl_selector(CDNewsMenuView::btn_callBack), CAControlEventTouchDown);
    this->getView()->addSubview(btn_back);
    
    CAButton* btn_set= CAButton::create(CAButtonTypeSquareRect);
    btn_set->setCenter(CADipRect(300, size.height-200, 40, 40));
    btn_set->setTag(1);
    btn_set->setTitleColorForState(CAControlStateNormal,CAColor_white);
    btn_set->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/news_set.png")));
    btn_set->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/news_set.png")));
    btn_set->addTarget(this, CAControl_selector(CDNewsMenuView::btn_callBack), CAControlEventTouchDown);
    this->getView()->addSubview(btn_set);
    
    tableView = CATableView::createWithFrame(CADipRect(75, 300, size.width-150, size.height*0.6));
    tableView->setAllowsSelection(true);
    tableView->setTableViewDelegate(this);
    tableView->setTableViewDataSource(this);
    tableView->setBackGroundColor(CAColor_clear);
    tableView->setSeparatorColor(ccc4(166, 166, 166,100));
    tableView->setShowsScrollIndicators(false);
    tableView->setSeparatorViewHeight(2);
    this->getView()->addSubview(tableView);
    tableView->reloadData();
    if (newsPageID>=0) {
        tableView->setSelectRowAtIndexPath(0, newsPageID);
    }
}

void CDNewsMenuView::viewDidUnload()
{
    
}

void CDNewsMenuView::btn_callBack(CrossApp::CAControl *_btn, CrossApp::CCPoint point)
{
    CAButton* btn = (CAButton*)_btn;
    int tag = btn->getTag();
    if(tag==0){
        CAApplication::getApplication()->setStatusBarStyle(CAStatusBarStyleLightContent);
        RootWindow::getInstance()->getSplitNavigationController()->popToRootViewControllerAnimated(true);
    }else if(tag==1)
    {
        CDNewsAboutController* news_controller1 = new CDNewsAboutController();
        news_controller1->autorelease();
        CANavigationBarItem* temp_nav = CANavigationBarItem::create(" ");
        temp_nav->setNagigationBarHidden(true);
        news_controller1->setNavigationBarItem(temp_nav);
        RootWindow::getInstance()->getSplitNavigationController()->presentModalViewController(news_controller1, false);
    }

}

void CDNewsMenuView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    //RootWindow::getInstance()->getSplitNavigationController()->setNavigationBarHidden(true, false);
    newsPageID = row;
    CDNewsMenuView* _menuview = new CDNewsMenuView();
    _menuview->init();
    _menuview->autorelease();
    
    CANavigationBarItem* temp_nav0 = CANavigationBarItem::create(" ");
    temp_nav0->setNagigationBarHidden(true);
    _menuview->setNavigationBarItem(temp_nav0);
    string temp_str;
    if (row<5) {
        temp_str = "getdemocon";
    }else{
        temp_str = "pic";
    }
    CDNewsViewController* news_controller1 = new CDNewsViewController(row,temp_str);
    news_controller1->autorelease();
    CANavigationBarItem* temp_nav = CANavigationBarItem::create(" ");
    temp_nav->setNagigationBarHidden(true);
    news_controller1->setNavigationBarItem(temp_nav);
    
    CCSize winSize = CAApplication::getApplication()->getWinSize();
    CAVector<CrossApp::CAViewController *> _vec;
    _vec.pushBack(_menuview);
    _vec.pushBack(news_controller1);
    vector<unsigned int> widths;
    widths.push_back(_px(400));
    widths.push_back(winSize.width-_px(400));
    
    CASplitViewController* _splitViewController = new CASplitViewController();
    _splitViewController->initWithController(_vec, widths);
    _splitViewController->setBackgroundView(CAImageView::createWithImage(CAImage::create("image/bg1.jpg")));
    
    CANavigationBarItem* temp_nav1 = CANavigationBarItem::create(" ");
    temp_nav1->setNagigationBarHidden(true);
    _splitViewController->setNavigationBarItem(temp_nav1);
    RootWindow::getInstance()->getSplitNavigationController()->replaceViewController(_splitViewController, false);
}

CATableViewCell* CDNewsMenuView::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CCLog("row==%d",section);
    CADipSize _size = cellSize;
    CDNewsMenuTableCell* cell = dynamic_cast<CDNewsMenuTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CDNewsMenuTableCell::create("CrossApp", CADipRect(0, 0, _size.width, _size.height),row+1);
        //cell->setImageID(row+1);
    }
    //cell->setImageID(row+1);
    return cell;
}

unsigned int CDNewsMenuView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 8;
}

unsigned int CDNewsMenuView::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int CDNewsMenuView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(116);
}