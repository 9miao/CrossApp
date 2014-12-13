
#ifndef __PLATFORM_WIN32__WEBVIEWIMPL_H_
#define __PLATFORM_WIN32__WEBVIEWIMPL_H_

#include "platform/CCPlatformConfig.h"
#include "view/CAWebView.h"
#include <string>


NS_CC_BEGIN


class CC_DLL CAWebViewImpl
{
public:
	CAWebViewImpl(CAWebView *webView);

	virtual ~CAWebViewImpl();

    void setJavascriptInterfaceScheme(const std::string &scheme);

     void loadHTMLString(const std::string &string, const std::string &baseURL);

    void loadURL(const std::string &url);

    void loadFile(const std::string &fileName);

    void stopLoading();

    void reload();

    bool canGoBack();

    bool canGoForward();

    void goBack();

    void goForward();

    void evaluateJS(const std::string &js);

    void setScalesPageToFit(const bool scalesPageToFit);

	virtual void visit();

    virtual void setVisible(bool visible);

	CAImageView* getWebViewImage();

    static bool shouldStartLoading(const int viewTag, const std::string &url);
    static void didFinishLoading(const int viewTag, const std::string &url);
    static void didFailLoading(const int viewTag, const std::string &url);
    static void onJsCallback(const int viewTag, const std::string &message);

private:
    CAWebView *_webView;
	CAView* _bkHelpView;
};


NS_CC_END

#endif /* __PLATFORM_WIN32__WEBVIEWIMPL_H_ */
