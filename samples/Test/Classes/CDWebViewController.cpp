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

}

CDWebViewController::~CDWebViewController()
{
    this->getView()->removeSubview(p_webView);
    p_webView = NULL;
}

void CDWebViewController::viewDidLoad()
{
    this->getNavigationBarItem()->setShowGoBackButton(true);
}

void CDWebViewController::viewDidUnload()
{
    
}

void CDWebViewController::initWebView(string url)
{
    p_webView = CAWebView::createWithLayout(DLayout(0,0,0,0,DLayout::L_R_T_B));
    p_webView->loadURL(url);
    this->getView()->addSubview(p_webView);
}