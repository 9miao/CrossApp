
#include "CAWebView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "platform/ios/CAWebViewImpl.h"
#include "platform/CCPlatformConfig.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/CAWebViewImpl.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "platform/win32/CAWebViewImpl.h"
#endif


NS_CC_BEGIN

CAWebView::CAWebView()
: _impl(new CAWebViewImpl(this))
, m_bHideNativeWeCmd(false)
, m_pWebViewDelegate(NULL)
, m_pImageView(NULL)
, m_pLoadingView(NULL)
, m_obLastPoint(CCPointZero)
, m_obLastContentSize(CCSizeZero)
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

bool CAWebView::init()
{
    CAScheduler::schedule(schedule_selector(CAWebView::update), this, 1/60.0f);
    
    CCSize size = this->getBounds().size;
    m_pLoadingView = CAActivityIndicatorView::create();
    m_pLoadingView->setStyle(CAActivityIndicatorViewStyleGrayLarge);
	m_pLoadingView->setVisible(false);
	this->addSubview(m_pLoadingView);
    
    return true;
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

std::string CAWebView::evaluateJS(const std::string &js)
{
	return _impl->evaluateJS(js);
}

std::string CAWebView::getHTMLSource()
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	return evaluateJS(std::string("window.local_obj.showSource('<head>'+") + "document.getElementsByTagName('html')[0].innerHTML+'</head>');");
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	return evaluateJS("document.documentElement.innerHTML");
#endif

	return "";
}

void CAWebView::setScalesPageToFit(bool const scalesPageToFit)
{
	_impl->setScalesPageToFit(scalesPageToFit);
}

void CAWebView::hideNativeWebAndShowImage()
{
    this->removeSubview(m_pImageView);
	m_pImageView = _impl->getWebViewImage();
	if (m_pImageView)
	{
		this->insertSubview(m_pImageView, 1);
	}
	m_bHideNativeWeCmd = true;
}

void CAWebView::showNativeWeb()
{
	_impl->setVisible(true);
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

void CAWebView::setActivityView(CAActivityIndicatorView* loadingView)
{
	if (m_pLoadingView)
	{
		m_pLoadingView->removeFromSuperview();
	}
	m_pLoadingView = loadingView;

	if (m_pLoadingView)
	{
		this->addSubview(m_pLoadingView);
	}
}

void CAWebView::showLoadingActivity(bool show)
{
	if (show)
	{
		_impl->setVisible(false);
		m_pLoadingView->startAnimating();
		m_pLoadingView->setVisible(true);
	}
	else
	{
		m_pLoadingView->stopAnimating();
		m_pLoadingView->setVisible(false);
		_impl->setVisible(true);
	}
}

void CAWebView::update(float dt)
{
    do
    {
        CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        CCPoint point = this->convertToWorldSpace(m_obPoint);
        CCSize contentSize = CCSizeApplyAffineTransform(m_obContentSize, worldToNodeTransform());
        //CC_BREAK_IF(m_obLastPoint.equals(point) && m_obLastContentSize.equals(contentSize));
        m_obLastPoint = point;
        m_obLastContentSize = contentSize;

        _impl->update(dt);
    }
    while (0);
}

void CAWebView::setContentSize(const CCSize &contentSize)
{
    CAView::setContentSize(contentSize);
    m_pLoadingView->setFrame(this->getBounds());
}

NS_CC_END




