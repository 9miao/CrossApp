//
//  MenuViewController.cpp
//  Test
//
//  Created by renhongguang on 15/4/3.
//
//

#include "MenuViewController.h"
#include "MyWebViewController.h"
#include "FirstViewController.h"

#pragma execution_character_set("utf-8")

static const wchar_t* menuList[4] = { L"CrossApp", L"9秒" };

MenuViewController::MenuViewController()
{
	CANotificationCenter::getInstance()->addObserver(CALLBACK_BIND_1(MenuViewController::changeStatusBarOrientation, this), this, CAApplicationDidChangeStatusBarOrientationNotification);
}

MenuViewController::~MenuViewController()
{
    CANotificationCenter::getInstance()->removeObserver(this, CAApplicationDidChangeStatusBarOrientationNotification);
}

void MenuViewController::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::CLEAR);
    
    DLayout tableViewLayout;
    DLayout logoLayout;
    const CAInterfaceOrientation& orientation = CAApplication::getApplication()->getStatusBarOrientation();
    if (orientation == CAInterfaceOrientation::Landscape)
    {
        tableViewLayout = DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(0, 400));
        logoLayout = DLayout(DHorizontalLayout_W_C(261, 0.5), DVerticalLayout_T_H(120, 258));
    }
    else
    {
        tableViewLayout = DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(450, 0));
        logoLayout = DLayout(DHorizontalLayout_W_C(261, 0.5), DVerticalLayout_T_H(120, 258));
    }
    
    m_pTableView = CATableView::createWithLayout(tableViewLayout);
    m_pTableView->setAllowsSelection(true);
    m_pTableView->setBackgroundImage(nullptr);
    m_pTableView->setSeparatorColor(ccc4(166, 166, 166,100));
    m_pTableView->setShowsScrollIndicators(false);
    m_pTableView->setScrollEnabled(false);
    this->getView()->addSubview(m_pTableView);
    
    m_pTableView->onCellAtIndexPath(CALLBACK_BIND_3(MenuViewController::tableCellAtIndex, this));
    m_pTableView->onCellHeightAtIndexPath(CALLBACK_BIND_2(MenuViewController::tableViewHeightForRowAtIndexPath, this));
    m_pTableView->onNumberOfRowsAtIndexPath(CALLBACK_BIND_1(MenuViewController::numberOfRowsInSection, this));
    m_pTableView->onDidSelectCellAtIndexPath(CALLBACK_BIND_2(MenuViewController::tableViewDidSelectRowAtIndexPath, this));
    
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
    if (orientation == CAInterfaceOrientation::Landscape)
    {
        tableViewLayout = DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(0, 400));
        logoLayout = DLayout(DHorizontalLayout_W_C(261, 0.5), DVerticalLayout_T_H(120, 258));
    }
    else
    {
        tableViewLayout = DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(450, 0));
        logoLayout = DLayout(DHorizontalLayout_W_C(261, 0.5), DVerticalLayout_T_H(120, 258));
    }
    
    m_pTableView->setLayout(tableViewLayout);
    m_pLogo->setLayout(logoLayout);
}

void MenuViewController::tableViewDidSelectRowAtIndexPath(unsigned int section, unsigned int row)
{
    if(row == 0)
    {
        MyWebViewController* webViewController = new MyWebViewController("http://crossapp.9miao.com");
        webViewController->setTitle("CrossApp");
        webViewController->init();
        webViewController->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(webViewController, true);
    }
    else if(row == 1)
    {
        MyWebViewController* webViewController = new MyWebViewController("http://www.9miao.com/");
        webViewController->setTitle("9秒");
        webViewController->init();
        webViewController->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(webViewController, true);
    }
    RootWindow::getInstance()->getDrawerController()->hideLeftViewController(false);
}

CATableViewCell* MenuViewController::tableCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int row)
{
    CATableViewCell* cell = m_pTableView->dequeueReusableCellWithIdentifier("CrossApp");
    if (cell == NULL)
    {
        cell = CATableViewCell::create("CrossApp");
        cell->setBackgroundView(NULL);
        CALabel* test = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 0), DVerticalLayoutFill));
        test->setTextAlignment(CATextAlignment::Left);
        test->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        test->setFontSize(32);
		test->setColor(CAColor4B::WHITE);
        test->setTag(100);
        cell->addSubview(test);
        
        CAImageView* arrow = CAImageView::createWithLayout(DLayout(DHorizontalLayout_R_W(0, 64), DVerticalLayout_T_H(20, 64)));
        arrow->setTag(101);
        cell->addSubview(arrow);
    }
	CALabel* test = (CALabel*)cell->getSubviewByTag(100);
	test->setText(unicode_to_utf8(menuList[row]));
    CAImageView* arrow = (CAImageView*)cell->getSubviewByTag(101);
    arrow->setImage(CAImage::create("source_material/cell_btn_right.png"));

    return cell;
}

unsigned int MenuViewController::numberOfRowsInSection(unsigned int section)
{
    return 2;
}

unsigned int MenuViewController::tableViewHeightForRowAtIndexPath(unsigned int section, unsigned int row)
{
    return 100;
}
