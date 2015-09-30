//
//  CDWebViewController.h
//  Test
//
//  Created by renhongguang on 15/4/27.
//
//

#ifndef __Test__CDWebViewController__
#define __Test__CDWebViewController__

#include "CrossApp.h"

class CDWebViewController : public CAViewController
{
public:
    CDWebViewController();
    
    virtual ~CDWebViewController();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void initWebView(string url);
    //CREATE_FUNC(CDWebViewController);
public:
    CCSize winSize;
    CAWebView* p_webView;
    std::string m_url;
};

#endif /* defined(__Test__CDWebViewController__) */
