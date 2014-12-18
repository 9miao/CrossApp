
#include "CAWebViewImpl.h"
#include "view/CAWebView.h"
#include "basics/CAApplication.h"


NS_CC_BEGIN


CAWebViewImpl::CAWebViewImpl(CAWebView *webView) : _webView(webView) 
{
	_bkHelpView = CAView::createWithColor(CAColor_red);
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

void CAWebViewImpl::evaluateJS(const std::string &js) 
{
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

void CAWebViewImpl::visit()
{
	_bkHelpView->setFrame(_webView->convertRectToWorldSpace(_webView->getBounds()));

	if (_bkHelpView->m_pSuperview == NULL && _webView->m_pSuperview!=NULL)
	{
		_webView->m_pSuperview->addSubview(_bkHelpView);
	}
}

void CAWebViewImpl::setVisible(bool visible) 
{
	_bkHelpView->setVisible(visible);
}

CAImageView* CAWebViewImpl::getWebViewImage()
{
	return NULL;
}

NS_CC_END
