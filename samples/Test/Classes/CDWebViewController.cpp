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
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

CDWebViewController::~CDWebViewController()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
    this->getView()->removeSubview(p_webView);
    p_webView = NULL;
}

void CDWebViewController::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    this->getNavigationBarItem()->setShowGoBackButton(true);
}

void CDWebViewController::viewDidUnload()
{
    
}

void CDWebViewController::initWebView(string url)
{
    p_webView = CAWebView::createWithFrame(CADipRect(0,0,winSize.width,winSize.height));
    p_webView->loadURL(url);
    this->getView()->addSubview(p_webView);
}