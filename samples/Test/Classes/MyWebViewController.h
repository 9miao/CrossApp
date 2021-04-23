

#ifndef __Test__MyWebViewController__
#define __Test__MyWebViewController__

#include "CrossApp.h"

class MyWebViewController : public CAViewController
{
public:
    MyWebViewController(const std::string& url);
    
    virtual ~MyWebViewController();
    
    void viewDidLoad();
    
    void viewDidUnload();
    

public:

    std::string m_sUrl;
    
    CAWebView* m_pWebView;
};

#endif /* defined(__Test__MyWebViewController__) */
