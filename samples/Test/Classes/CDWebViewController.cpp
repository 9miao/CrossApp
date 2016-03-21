//
//  CDWebViewController.cpp
//  Test
//
//  Created by renhongguang on 15/4/27.
//
//

#include "CDWebViewController.h"

CDWebViewController::CDWebViewController()
:m_url("")
{
//    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
//    drawer->setTouchMoved(false);
}

CDWebViewController::~CDWebViewController()
{
//    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
//    drawer->setTouchMoved(true);
    this->getView()->removeSubview(p_webView);
    p_webView = NULL;
}

void CDWebViewController::viewDidLoad()
{
//    winSize = this->getView()->getBounds().size;
    this->getNavigationBarItem()->setShowGoBackButton(true);
}

void CDWebViewController::viewDidUnload()
{
    
}

void CDWebViewController::initWebView(string url)
{
//    p_webView = CAWebView::createWithFrame(DRect(0,0,winSize.width,winSize.height));
    p_webView = CAWebView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    p_webView->loadURL(url);
    p_webView->loadURL("wwww.baidu.com");
    this->getView()->addSubview(p_webView);
}