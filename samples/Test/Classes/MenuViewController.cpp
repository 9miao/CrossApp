//
//  MenuViewController.cpp
//  Test
//
//  Created by renhongguang on 15/4/3.
//
//

#include "MenuViewController.h"
#include "CDWebViewController.h"
#include "CDUIShowAutoCollectionView.h"

MenuViewController::MenuViewController()
{

}

MenuViewController::~MenuViewController()
{
    
}

void MenuViewController::viewDidLoad()
{
    this->getView()->removeAllSubviews();
    this->getView()->setColor(CAColor_clear);
    
    tableView = CATableView::createWithLayout(DLayout(0,0,450,0,DLayout::L_R_T_B));
    tableView->setAllowsSelection(true);
    tableView->setTableViewDelegate(this);
    tableView->setTableViewDataSource(this);
    tableView->setBackgroundColor(CAColor_clear);
    tableView->setSeparatorColor(ccc4(166, 166, 166,100));
    tableView->setShowsScrollIndicators(false);
    this->getView()->addSubview(tableView);
    
}

void MenuViewController::viewDidUnload()
{

}

void MenuViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    RootWindow::getInstance()->dismissModalViewController(true);
    if (row==0)
    {
        RootWindow::getInstance()->initUIView();
    }
    else if(row==1)
    {
        RootWindow::getInstance()->intNewsView();
    }
    else if(row==2)
    {
        CDWebViewController* _webController = new CDWebViewController();
        _webController->init();
        _webController->setTitle(" ");
        
        _webController->autorelease();
        RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(_webController, true);
        _webController->initWebView("http://crossapp.9miao.com");
    }
    else if(row==3)
    {
        CDWebViewController* _webController = new CDWebViewController();
        _webController->init();
        _webController->setTitle(" ");
        
        _webController->autorelease();
        RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(_webController, true);
        _webController->initWebView("http://www.9miao.com/");
    }
    
}

CATableViewCell* MenuViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
//    DSize _size = cellSize;
    CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
    if (cell == NULL)
    {
        cell = CATableViewCell::create("CrossApp");
        cell->setBackgroundView(NULL);
//        CALabel* test = CALabel::createWithCenter(DRect(_size.width/2+30,_size.height/2,_size.width,_size.height));
        CALabel* test = CALabel::createWithLayout(DLayout(50,0,0,0,DLayout::L_R_T_B));
        test->setTextAlignment(CATextAlignmentLeft);
        test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        test->setFontSize(32);
		test->setColor(CAColor_white);
        test->setTag(100);
        cell->addSubview(test);
        
//        CAImageView* arrow = CAImageView::createWithCenter(DRect(_size.width-64,_size.height/2,64,64));
        CAImageView* arrow = CAImageView::createWithLayout(DLayout(0,64,20,64,DLayout::R_W_T_H));
        arrow->setTag(101);
        cell->addSubview(arrow);
    }
	CALabel* test = (CALabel*)cell->getSubviewByTag(100);
	test->setText(unicode_to_utf8(menuList[row]));// menuList[row]);
    CAImageView* arrow = (CAImageView*)cell->getSubviewByTag(101);
    arrow->setImage(CAImage::create("source_material/cell_btn_right.png"));

    return cell;
}

unsigned int MenuViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 4;
}

unsigned int MenuViewController::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int MenuViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return 100;
}
