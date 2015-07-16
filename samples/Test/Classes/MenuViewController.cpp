//
//  MenuViewController.cpp
//  Test
//
//  Created by renhongguang on 15/4/3.
//
//

#include "MenuViewController.h"
#include "CDWebViewController.h"
#include "CDUIShowCollectionView.h"

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
    size = this->getView()->getBounds().size;
    
    tableView = CATableView::createWithFrame(CADipRect(0, size.height/3, size.width, size.height*0.6));
    tableView->setAllowsSelection(true);
    tableView->setTableViewDelegate(this);
    tableView->setTableViewDataSource(this);
    tableView->setBackGroundColor(CAColor_clear);
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
        _webController->initWebView("http://www.crossapp.com.cn");
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

CATableViewCell* MenuViewController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CADipSize _size = cellSize;
    CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
    if (cell == NULL)
    {
        cell = CATableViewCell::create("CrossApp");
        cell->setBackgroundView(NULL);
        CALabel* test = CALabel::createWithCenter(CADipRect(_size.width/2+30,
                                                            _size.height/2,
                                                            _size.width,
                                                            _size.height));
        test->setColor(ccc4(220, 227, 115, 255));
        test->setTextAlignment(CATextAlignmentLeft);
        test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        test->setFontSize(_px(32));
        test->setColor(CAColor_white);
        test->setTag(100);
        cell->addSubview(test);
        
        CAImageView* arrow = CAImageView::createWithCenter(CADipRect(_size.width-64,_size.height/2,64,64));
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
    return _px(100);
}
