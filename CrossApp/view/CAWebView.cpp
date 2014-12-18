
#include "CAWebView.h"
#include "basics/CAScheduler.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/CAWebViewImpl.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#include "platform/win32/CAWebViewImpl.h"
#endif

NS_CC_BEGIN

CAWebView::CAWebView()
: _impl(new CAWebViewImpl(this))
, m_bHideNativeWeCmd(false)
, m_pWebViewDelegate(NULL)
{
}

CAWebView::~CAWebView()
{
	CC_SAFE_DELETE(_impl);
}


CAWebView *CAWebView::createWithFrame(const CCRect& rect)
{
	CAWebView* webView = new CAWebView();
	if (webView && webView->initWithFrame(rect))
	{
		webView->autorelease();
		return webView;
	}
	CC_SAFE_DELETE(webView);
	return NULL;
}

CAWebView *CAWebView::createWithCenter(const CCRect& rect)
{
	CAWebView* webView = new CAWebView();
	if (webView && webView->initWithCenter(rect))
	{
		webView->autorelease();
		return webView;
	}
	CC_SAFE_DELETE(webView);
	return NULL;
}

void CAWebView::setJavascriptInterfaceScheme(const std::string &scheme)
{
	_impl->setJavascriptInterfaceScheme(scheme);
}

void CAWebView::loadHTMLString(const std::string &string, const std::string &baseURL)
{
	_impl->loadHTMLString(string, baseURL);
}

void CAWebView::loadURL(const std::string &url)
{
	_impl->loadURL(url);
}

void CAWebView::loadFile(const std::string &fileName)
{
	_impl->loadFile(fileName);
}

void CAWebView::stopLoading()
{
	_impl->stopLoading();
}

void CAWebView::reload()
{
	_impl->reload();
}

bool CAWebView::canGoBack()
{
	return _impl->canGoBack();
}

bool CAWebView::canGoForward()
{
	return _impl->canGoForward();
}

void CAWebView::goBack()
{
	_impl->goBack();
}

void CAWebView::goForward()
{
	_impl->goForward();
}

void CAWebView::evaluateJS(const std::string &js)
{
	_impl->evaluateJS(js);
}

void CAWebView::setScalesPageToFit(bool const scalesPageToFit)
{
	_impl->setScalesPageToFit(scalesPageToFit);
}

void CAWebView::hideNativeWebAndShowImage()
{
	removeSubviewByTag((int)_impl);
	CAImageView* pImageView = _impl->getWebViewImage();
	if (pImageView)
	{
		pImageView->setTag((int)_impl);
		insertSubview(pImageView, 0x00FFFFFF);
	}
	m_bHideNativeWeCmd = true;
}

void CAWebView::showNativeWeb()
{
	_impl->setVisible(true);
}

void CAWebView::visit()
{
	CAView::visit();
	_impl->visit();
}

void CAWebView::draw()
{
	CAView::draw();

	if (m_bHideNativeWeCmd)
	{
		_impl->setVisible(false);
		m_bHideNativeWeCmd = false;
	}
}


void CAWebView::setVisible(bool visible)
{
	CAView::setVisible(visible);
	_impl->setVisible(visible);
}

NS_CC_END




