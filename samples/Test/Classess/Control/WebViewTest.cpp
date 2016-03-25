
#include "WebViewTest.h"

WebViewTest::WebViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

WebViewTest::~WebViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void WebViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    CAWebView* web = CAWebView::createWithLayout(DRectLayout(0, 0, 200, 240, DRectLayout::L_R_T_B));
    web->loadURL("http://crossapp.9miao.com");
    this->getView()->addSubview(web);
}

void WebViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

