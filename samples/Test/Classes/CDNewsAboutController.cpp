//
//  CDNewsAboutController.cpp
//  Test
//
//  Created by renhongguang on 15/4/17.
//
//

#include "CDNewsAboutController.h"

CDNewsAboutTableCell::CDNewsAboutTableCell()
{
    this->setAllowsSelected(false);
}

CDNewsAboutTableCell::~CDNewsAboutTableCell()
{
    
}

CDNewsAboutTableCell* CDNewsAboutTableCell::create(const std::string& identifier, const CADipRect& _rect)
{
    CDNewsAboutTableCell* tableViewCell = new CDNewsAboutTableCell();
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
        tableViewCell->setFrame(_rect);
        tableViewCell->autorelease();
        return tableViewCell;
    }
    CC_SAFE_DELETE(tableViewCell);
    return NULL;
}

void CDNewsAboutTableCell::highlightedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void CDNewsAboutTableCell::selectedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void CDNewsAboutTableCell::initWithCell()
{
    CADipSize m_size = this->getFrame().size;
    
    CALabel* cellText = CALabel::createWithCenter(CADipRect(m_size.width/2+40, m_size.height/2, m_size.width, 40));
    cellText->setTag(100);
    cellText->setFontSize(_px(30));
    cellText->setTextAlignment(CATextAlignmentLeft);
    cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->addSubview(cellText);
    
    CAScale9ImageView* iv = CAScale9ImageView::createWithCenter(CADipRect(m_size.width-50,m_size.height/2,50,50));
    iv->setImage(CAImage::create("source_material/cell_btn_right.png"));
    iv->setTag(101);
    this->addSubview(iv);
    
    CASwitch* sw = CASwitch::createWithCenter(CADipRect(m_size.width-90, m_size.height/2, 100, 50));
    sw->setTag(103);
    sw->setVisible(false);
    this->addSubview(sw);
}

CDNewsAboutController::CDNewsAboutController(){
    
}

CDNewsAboutController::~CDNewsAboutController()
{
    
}

void CDNewsAboutController::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    
    p_TableView= CATableView::createWithFrame(CADipRect(0, -150, winSize.width, winSize.height+150));
    p_TableView->setTableViewDataSource(this);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setAllowsSelection(true);
    p_TableView->setAllowsMultipleSelection(false);
    p_TableView->setShowsScrollIndicators(false);
    p_TableView->setAlwaysTopSectionHeader(false);
    p_TableView->setBackGroundColor(CAColor_white);
    this->getView()->addSubview(p_TableView);
    p_TableView->setTableHeaderHeight(_px(602));
    CAView* view = CAView::createWithColor(CAColor_clear);
    view->setFrame(CADipRect(0,0,winSize.width,602));
    
    CAImageView* head_bg = CAImageView::createWithFrame(CADipRect(0,0,winSize.width,602));
    head_bg->setImage(CAImage::create("image/about_head_bg.png"));
    view->addSubview(head_bg);
    
    CAImageView* head = CAImageView::createWithCenter(CADipRect(winSize.width/2,320,96,96));
    head->setImage(CAImage::create("image/avatar_bg_70.png"));
    view->addSubview(head);
    
    CAButton* btn1 = CAButton::create(CAButtonTypeSquareRect);
    btn1->setCenter(CADipRect(winSize.width/6, 600-(winSize.width/3)/9*5/2, winSize.width/3-1, (winSize.width/3)/9*5));
    btn1->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn1_up.png")));
    btn1->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn1_down.png")));
    view->addSubview(btn1);
    
    CAButton* btn2 = CAButton::create(CAButtonTypeSquareRect);
    btn2->setCenter(CADipRect(winSize.width/2, 600-(winSize.width/3)/9*5/2, winSize.width/3-1, (winSize.width/3)/9*5));
    btn2->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn2_up.png")));
    btn2->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn2_down.png")));
    view->addSubview(btn2);
    
    CAButton* btn3 = CAButton::create(CAButtonTypeSquareRect);
    btn3->setTag(100);
    btn3->setCenter(CADipRect(winSize.width/6*5, 600-(winSize.width/3)/9*5/2, winSize.width/3-1, (winSize.width/3)/9*5));
    btn3->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn3_up.png")));
    btn3->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn3_down.png")));
    view->addSubview(btn3);
    
    p_TableView->setTableHeaderView(view);
}

void CDNewsAboutController::viewDidUnload()
{
    
}

CAView* CDNewsAboutController::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(ccc4(240,240,240,255));
    
    return view;
}

CAView* CDNewsAboutController::tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor_gray);
    
    return view;
}

void CDNewsAboutController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void CDNewsAboutController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDNewsAboutController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CADipSize _size = cellSize;
    CDNewsAboutTableCell* cell = dynamic_cast<CDNewsAboutTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CDNewsAboutTableCell::create("CrossApp", CADipRect(0, 0, _size.width, _size.height));
        cell->initWithCell();
    }
    
    CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
    CASwitch* cellBtn = (CASwitch*)cell->getSubviewByTag(103);
    cellBtn->setVisible(false);
    switch (section) {
        case 0:
			cellText->setText(UTF8EX(aboutMenuTag[row]));
            break;
        case 1:
			cellText->setText(UTF8EX(aboutMenuTag[row + 2]));
            if (row==2||row==3||row==4) {
                cellBtn->setVisible(true);
            };
            break;
        case 2:
			cellText->setText(UTF8EX(aboutMenuTag[row + 8]));
            break;
    }
    return cell;
    
}

unsigned int CDNewsAboutController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    int cellnum = 0;
    if (section==0) {
        cellnum = 2;
    }else if (section==1){
        cellnum = 6;
    }
    else if (section==2){
        cellnum = 4;
    }
    return cellnum;
}

unsigned int CDNewsAboutController::numberOfSections(CATableView *table)
{
    return 3;
}
unsigned int CDNewsAboutController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(100);
}

unsigned int CDNewsAboutController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    int h = 40;
    if (section==0) {
        h=30;
    }
    return _px(h);
}

unsigned int CDNewsAboutController::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 1;
}
