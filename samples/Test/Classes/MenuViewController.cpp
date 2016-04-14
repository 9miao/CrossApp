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
    CANotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MenuViewController::changeStatusBarOrientation), CAApplicationDidChangeStatusBarOrientationNotification, NULL);
}

MenuViewController::~MenuViewController()
{
    CANotificationCenter::sharedNotificationCenter()->removeObserver(this, CAApplicationDidChangeStatusBarOrientationNotification);
}

void MenuViewController::viewDidLoad()
{
    this->getView()->setColor(CAColor_clear);
    
    DLayout tableViewLayout;
    DLayout logoLayout;
    const CAInterfaceOrientation& orientation = CAApplication::getApplication()->getStatusBarOrientation();
    if (orientation == CAInterfaceOrientationPortrait || orientation == CAInterfaceOrientationPortraitUpsideDown)
    {
        tableViewLayout = DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(450, 0));
        logoLayout = DLayout(DHorizontalLayout_W_C(261, 0.5), DVerticalLayout_T_H(120, 258));
    }
    else if (orientation == CAInterfaceOrientationLandscapeLeft || CAInterfaceOrientationLandscapeRight)
    {
        tableViewLayout = DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(0, 400));
        logoLayout = DLayout(DHorizontalLayout_W_C(261, 0.5), DVerticalLayout_T_H(120, 258));
    }
    
    tableView = CATableView::createWithLayout(DLayoutFill);
    tableView->setLayout(tableViewLayout);
    tableView->setAllowsSelection(true);
    tableView->setTableViewDelegate(this);
    tableView->setTableViewDataSource(this);
    tableView->setBackgroundColor(CAColor_clear);
    tableView->setSeparatorColor(ccc4(166, 166, 166,100));
    tableView->setShowsScrollIndicators(false);
    tableView->setScrollEnabled(false);
    this->getView()->addSubview(tableView);
    
    m_pLogo = CAImageView::createWithImage(CAImage::create("image/logo.png"));
    m_pLogo->setLayout(logoLayout);
    this->getView()->addSubview(m_pLogo);
    
}

void MenuViewController::viewDidUnload()
{

}

void MenuViewController::changeStatusBarOrientation(CAObject* obj)
{
    const CAInterfaceOrientation& orientation = CAApplication::getApplication()->getStatusBarOrientation();
    
    DLayout tableViewLayout;
    DLayout logoLayout;
    if (orientation == CAInterfaceOrientationPortrait || orientation == CAInterfaceOrientationPortraitUpsideDown)
    {
        tableViewLayout = DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(450, 0));
        logoLayout = DLayout(DHorizontalLayout_W_C(261, 0.5), DVerticalLayout_T_H(120, 258));
    }
    else if (orientation == CAInterfaceOrientationLandscapeLeft || CAInterfaceOrientationLandscapeRight)
    {
        tableViewLayout = DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(0, 400));
        logoLayout = DLayout(DHorizontalLayout_W_C(261, 0.5), DVerticalLayout_T_H(120, 258));
    }
    tableView->setLayout(tableViewLayout);
    m_pLogo->setLayout(logoLayout);
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
    CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
    if (cell == NULL)
    {
        cell = CATableViewCell::create("CrossApp");
        cell->setBackgroundView(NULL);
        CALabel* test = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 0), DVerticalLayoutFill));
        test->setTextAlignment(CATextAlignmentLeft);
        test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        test->setFontSize(32);
		test->setColor(CAColor_white);
        test->setTag(100);
        cell->addSubview(test);
        
        CAImageView* arrow = CAImageView::createWithLayout(DLayout(DHorizontalLayout_R_W(0, 64), DVerticalLayout_T_H(20, 64)));
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
