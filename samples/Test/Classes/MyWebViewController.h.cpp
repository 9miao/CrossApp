//
//  MyWebViewController.cpp
//  Test
//
//  Created by renhongguang on 15/4/27.
//
//

#include "MyWebViewController.h"

MyWebViewController::MyWebViewController(const std::string& url)
:m_sUrl(url)
{

}

MyWebViewController::~MyWebViewController()
{

}

void MyWebViewController::viewDidLoad()
{
    m_pWebView = CAWebView::createWithLayout(DLayoutFill);
    m_pWebView->loadURL(m_sUrl);
    this->getView()->addSubview(m_pWebView);
}

void MyWebViewController::viewDidUnload()
{
    
}
