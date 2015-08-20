//
//  CDNewsMenuView.cpp
//  Test-HD
//
//  Created by renhongguang on 15/8/7.
//
//

#include "CDNewsMenuView.h"

CDNewsMenuTableCell::CDNewsMenuTableCell()
{
    
}

CDNewsMenuTableCell::~CDNewsMenuTableCell()
{
    
}

CDNewsMenuTableCell* CDNewsMenuTableCell::create(const std::string& identifier, const CADipRect& _rect)
{
    CDNewsMenuTableCell* tableViewCell = new CDNewsMenuTableCell();
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
        tableViewCell->setFrame(_rect);
        tableViewCell->autorelease();
        return tableViewCell;
    }
    CC_SAFE_DELETE(tableViewCell);
    return NULL;
}

void CDNewsMenuTableCell::highlightedTableViewCell()
{
    this->getBackgroundView()->setColor(ccc4(0, 0, 0, 64));
}

void CDNewsMenuTableCell::selectedTableViewCell()
{
    this->getBackgroundView()->setColor(ccc4(0, 0, 0, 64));
}

void CDNewsMenuTableCell::normalTableViewCell()
{
    this->getBackgroundView()->setColor(CAColor_clear);
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
    this->getView()->setColor(ccc4(230,230,230,255));
    size = this->getView()->getBounds().size;
    
    tableView = CATableView::createWithFrame(CADipRect(75, 300, size.width-150, size.height*0.6));
    tableView->setAllowsSelection(true);
    tableView->setTableViewDelegate(this);
    tableView->setTableViewDataSource(this);
    tableView->setBackGroundColor(CAColor_clear);
    tableView->setSeparatorColor(ccc4(166, 166, 166,100));
    tableView->setShowsScrollIndicators(false);
    this->getView()->addSubview(tableView);
    
}

void CDNewsMenuView::viewDidUnload()
{
    
}

void CDNewsMenuView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDNewsMenuView::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CADipSize _size = cellSize;
    CDNewsMenuTableCell* cell = dynamic_cast<CDNewsMenuTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CDNewsMenuTableCell::create("CrossApp", CADipRect(0, 0, _size.width, _size.height));
        //cell->setBackgroundView(NULL);
        CALabel* test = CALabel::createWithCenter(CADipRect(_size.width/2+100,
                                                            _size.height/2,
                                                            _size.width,
                                                            _size.height));
        test->setColor(ccc4(220, 227, 115, 255));
        test->setTextAlignment(CATextAlignmentLeft);
        test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        test->setFontSize(_px(32));
        test->setColor(CAColor_black);
        test->setTag(100);
        cell->addSubview(test);
        
//        CAImageView* arrow = CAImageView::createWithCenter(CADipRect(_size.width-64,_size.height/2,64,64));
//        arrow->setTag(101);
//        cell->addSubview(arrow);
    }
    CALabel* test = (CALabel*)cell->getSubviewByTag(100);
    test->setText(unicode_to_utf8(newsTitle[row]));// menuList[row]);
//    CAImageView* arrow = (CAImageView*)cell->getSubviewByTag(101);
//    arrow->setImage(CAImage::create("source_material/cell_btn_right.png"));
    
    return cell;
}

unsigned int CDNewsMenuView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 9;
}

unsigned int CDNewsMenuView::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int CDNewsMenuView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(100);
}