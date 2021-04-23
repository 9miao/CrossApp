//
//  CAWebView.h
//  CrossApp
//
//  Created by Zhujian on 14-12-1.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CA_UI_WEBVIEW_H
#define __CA_UI_WEBVIEW_H

#include "platform/CCPlatformConfig.h"
#include "platform/CAFileUtils.h"
#include "CAView.h"
#include "CAImageView.h"
#include "CAActivityIndicatorView.h"


NS_CC_BEGIN

class CAWebView;
class CAWebViewImpl;


class CC_DLL CAWebView : public CAView
{
public:
    // event listeners.
    
    CC_LISTENING_FUNCTION(bool(const std::string&), StartLoading);
    
    CC_LISTENING_FUNCTION(void(const std::string&), FinishLoading);
    
    CC_LISTENING_FUNCTION(void(const std::string&), FailLoading);
    
    CC_LISTENING_FUNCTION(void(const std::string&), JSCallback);

public:
    
    CAWebView();
    
    virtual ~CAWebView();
    
    virtual bool init();
    
	static CAWebView *createWithFrame(const DRect& rect);

	static CAWebView *createWithCenter(const DRect& rect);

    static CAWebView* createWithLayout(const DLayout& layout);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    /**
    * Set javascript interface scheme.
    * @see #onJsCallback
    */
    void setJavascriptInterfaceScheme(const std::string &scheme);

    /**
    * Sets the main page content and base URL.
    * @param string The content for the main page.
    * @param baseURL The base URL for the content.
    */
    void loadHTMLString(const std::string &string, const std::string &baseURL);

    /**
    * Loads the given URL.
    * @param url content URL
    */
    void loadURL(const std::string &url);

    /**
    * Loads the given fileName.
    * @param fileName content fileName
    */
    void loadFile(const std::string &fileName);

    /**
    * Stops the current load.
    */
    void stopLoading();

    /**
    * Reloads the current URL.
    */
    void reload();

    /**
    * Gets whether this WebView has a back history item.
    * @return web view has a back history item.
    */
    bool canGoBack();

    /**
    * Gets whether this WebView has a forward history item.
    * @return web view has a forward history item.
    */
    bool canGoForward();

    /**
    * Goes back in the history.
    */
    void goBack();

    /**
    * Goes forward in the history.
    */
    void goForward();

    /**
    * evaluates JavaScript in the context of the currently displayed page
    */
    std::string evaluateJS(const std::string &js);

	std::string getHTMLSource();

    /**
    * Set WebView should support zooming. The default value is false.
    */
    void setScalesPageToFit(const bool scalesPageToFit);

	void hideNativeWebAndShowImage();

	void showNativeWeb();
    
    virtual void update(float dt);
    
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);

	virtual void setVisible(bool visible);
    
	void setActivityView(CAActivityIndicatorView* loadingView);
	void showLoadingActivity(bool show);
    
private:
    
    CAWebViewImpl *_impl;
    
    CAImageView* m_pImageView;
    
    CAActivityIndicatorView* m_pLoadingView;
    
    DPoint m_obLastPoint;
    
    DSize m_obLastContentSize;
    
	friend class CAWebViewImpl;
    
};


NS_CC_END


#endif //__CA_UI_WEBVIEW_H
