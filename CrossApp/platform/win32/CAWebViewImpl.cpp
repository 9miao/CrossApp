
#include "CAWebViewImpl.h"
#include "view/CAWebView.h"
#include "basics/CAApplication.h"


NS_CC_BEGIN


CAWebViewImpl::CAWebViewImpl(CAWebView *webView) : _webView(webView) 
{
	_bkHelpView = CAView::createWithColor(CAColor4B(127, 127, 127, 127));
	_bkHelpView->retain();
}

CAWebViewImpl::~CAWebViewImpl() 
{
	_bkHelpView->release();
}

void CAWebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL) 
{
}

void CAWebViewImpl::loadURL(const std::string &url) 
{
}

void CAWebViewImpl::loadFile(const std::string &fileName) 
{
}

void CAWebViewImpl::stopLoading() 
{
}

void CAWebViewImpl::reload() 
{
}

bool CAWebViewImpl::canGoBack() 
{
	return false;
}

bool CAWebViewImpl::canGoForward() 
{
	return false;
}

void CAWebViewImpl::goBack() 
{
}

void CAWebViewImpl::goForward() 
{
}

void CAWebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme) 
{
}

std::string CAWebViewImpl::evaluateJS(const std::string &js) 
{
	return "";
}

void CAWebViewImpl::setScalesPageToFit(const bool scalesPageToFit) 
{
}

bool CAWebViewImpl::shouldStartLoading(const int viewTag, const std::string &url) 
{
	return false;
}

void CAWebViewImpl::didFinishLoading(const int viewTag, const std::string &url)
{
}

void CAWebViewImpl::didFailLoading(const int viewTag, const std::string &url)
{
}

void CAWebViewImpl::onJsCallback(const int viewTag, const std::string &message)
{
}

void CAWebViewImpl::update(float dt)
{
    _bkHelpView->setFrame(_webView->getBounds());
    CC_RETURN_IF(_bkHelpView->isRunning());
    _webView->addSubview(_bkHelpView);
}

void CAWebViewImpl::setVisible(bool visible) 
{
	_bkHelpView->setVisible(visible);
}

void CAWebViewImpl::getWebViewImage(const std::function<void(CAImage*)>& callback)
{
	return NULL;
}

NS_CC_END
