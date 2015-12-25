
#import <UIKit/UIKit.h>
#include "CAWebViewImpl.h"
#include "EAGLView.h"
#include "basics/CAApplication.h"
#include "platform/CCFileUtils.h"
#include "platform/CADensityDpi.h"

USING_NS_CC;

#define WebViewWrapper ((UIWebViewWrapper*)m_pWebViewWrapper)

@interface UIWebViewWrapper : NSObject<UIWebViewDelegate>
@property(nonatomic, retain) UIWebView *uiWebView;
@property(nonatomic, copy) NSString *jsScheme;

@property(nonatomic, readonly, getter=canGoBack) BOOL canGoBack;
@property(nonatomic, readonly, getter=canGoForward) BOOL canGoForward;

+ (instancetype)webViewWrapper;

- (void)setVisible:(bool)visible;

- (UIImage*)getWebViewImage;

- (void)setFrameWith_X:(float)x Y:(float)y Width:(float)width Height:(float)height;

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme;

- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL;

- (void)loadUrl:(const std::string &)urlString;

- (void)loadFile:(const std::string &)filePath;

- (void)stopLoading;

- (void)reload;

- (std::string)evaluateJS:(const std::string &)js;

- (void)goBack;

- (void)goForward;

- (void)setScalesPageToFit:(const bool)scalesPageToFit;
@end

@implementation UIWebViewWrapper
{
    
}

+ (instancetype)webViewWrapper
{
    return [[[self alloc] init] autorelease];
}

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        self.uiWebView = nil;
    }
    return self;
}

- (void)dealloc
{
    self.uiWebView.delegate = nil;
    [self.uiWebView removeFromSuperview];
    self.jsScheme = nil;
    [super dealloc];
}

- (void)setupWebView
{
    if (!self.uiWebView)
    {
        self.uiWebView = [[[UIWebView alloc] init] autorelease];
        self.uiWebView.delegate = self;
        self.uiWebView.scalesPageToFit = YES;
    }
    if (!self.uiWebView.superview)
    {
        EAGLView * eaglview = [EAGLView sharedEGLView];
        [eaglview addSubview:self.uiWebView];
        [eaglview bringSubviewToFront: self.uiWebView];
    }

	NSURLCache *sharedCache = [[[NSURLCache alloc] initWithMemoryCapacity:0 diskCapacity:0 diskPath:nil] autorelease];
    [NSURLCache setSharedURLCache:sharedCache];
}

- (void)setVisible:(bool)visible
{
    self.uiWebView.hidden = !visible;
}

- (UIImage*)getWebViewImage
{
    if (UIGraphicsBeginImageContextWithOptions!=NULL)
    {
        UIGraphicsBeginImageContextWithOptions(self.uiWebView.frame.size, NO, 0);
    }
    else
    {
        UIGraphicsBeginImageContext(self.uiWebView.frame.size);
    }
    
    [self.uiWebView.layer renderInContext:UIGraphicsGetCurrentContext()];
    return UIGraphicsGetImageFromCurrentImageContext();
}


- (void)setFrameWith_X:(float)x Y:(float)y Width:(float)width Height:(float)height
{
    if (!self.uiWebView)
    {
        [self setupWebView];
    }
    
    CGRect newFrame = CGRectMake(x, y, width, height);
    self.uiWebView.frame = newFrame;
}

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme
{
    self.jsScheme = @(scheme.c_str());
}


- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL
{
    [self.uiWebView loadHTMLString:@(string.c_str()) baseURL:[NSURL URLWithString:@(baseURL.c_str())]];
}

- (void)loadUrl:(const std::string &)urlString
{
    if (!self.uiWebView)
    {
        [self setupWebView];
    }
    
    NSURL *url = [NSURL URLWithString:@(urlString.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.uiWebView loadRequest:request];
}

- (void)loadFile:(const std::string &)filePath
{
    if (!self.uiWebView)
    {
        [self setupWebView];
    }
    
    NSURL *url = [NSURL fileURLWithPath:@(filePath.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.uiWebView loadRequest:request];
}

- (void)stopLoading
{
    [self.uiWebView stopLoading];
}

- (void)reload
{
    [self.uiWebView reload];
}

- (BOOL)canGoForward
{
    return self.uiWebView.canGoForward;
}

- (BOOL)canGoBack
{
    return self.uiWebView.canGoBack;
}

- (void)goBack
{
    [self.uiWebView goBack];
}

- (void)goForward
{
    [self.uiWebView goForward];
}

- (std::string)evaluateJS:(const std::string &)js
{
    if (!self.uiWebView) {[self setupWebView];}
    NSString* s = [self.uiWebView stringByEvaluatingJavaScriptFromString:@(js.c_str())];
    return [s UTF8String];
}

- (void)setScalesPageToFit:(const bool)scalesPageToFit
{
    self.uiWebView.scalesPageToFit = scalesPageToFit;
}

#pragma mark - UIWebViewDelegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString *url = [[request URL] absoluteString];
    if ([[[request URL] scheme] isEqualToString:self.jsScheme])
    {
        CAWebViewImpl::onJsCallback(self, [url UTF8String]);
        return NO;
    }
    if (url)
    {
        return CAWebViewImpl::shouldStartLoading(self, [url UTF8String]);
    }
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    NSString *url = [[webView.request URL] absoluteString];
    CAWebViewImpl::didFinishLoading(self, [url UTF8String]);
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSString *url = error.userInfo[NSURLErrorFailingURLStringErrorKey];
    if (url) {
        CAWebViewImpl::didFailLoading(self, [url UTF8String]);
    }
}

@end




NS_CC_BEGIN

std::map<UIWebViewWrapper*, CAWebViewImpl*> s_WebViewImpls;

CAWebViewImpl::CAWebViewImpl(CAWebView *webView)
:m_pWebViewWrapper([UIWebViewWrapper webViewWrapper])
,m_pWebView(webView)
{
    [WebViewWrapper retain];
    [WebViewWrapper setVisible:YES];
    s_WebViewImpls [WebViewWrapper] = this;
}

CAWebViewImpl::~CAWebViewImpl()
{
    s_WebViewImpls.erase(WebViewWrapper);
    [WebViewWrapper release];
    m_pWebViewWrapper = NULL;
}

typedef std::map<UIWebViewWrapper*, CAWebViewImpl*>::iterator WEB_MAP;

bool CAWebViewImpl::shouldStartLoading(void* pWebViewWrapper, const std::string &url)
{
    WEB_MAP it = s_WebViewImpls.find((UIWebViewWrapper*)pWebViewWrapper);
    if (it != s_WebViewImpls.end())
    {
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_pWebViewDelegate)
        {
            if (!webView->m_pWebViewDelegate->onShouldStartLoading(webView, url))
                return false;
        }
    }
    return true;
}

void CAWebViewImpl::didFinishLoading(void* pWebViewWrapper, const std::string &url)
{
    WEB_MAP it=s_WebViewImpls.find((UIWebViewWrapper*)pWebViewWrapper);
    
    if (it != s_WebViewImpls.end()) {
        
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_pWebViewDelegate)
        {
            webView->m_pWebViewDelegate->onDidFinishLoading(webView, url);
        }
    }
}

void CAWebViewImpl::didFailLoading(void* pWebViewWrapper, const std::string &url)
{
    WEB_MAP it=s_WebViewImpls.find((UIWebViewWrapper*)pWebViewWrapper);
    if (it != s_WebViewImpls.end())
    {
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_pWebViewDelegate)
        {
            webView->m_pWebViewDelegate->onDidFailLoading(webView, url);
        }
    }
}

void CAWebViewImpl::onJsCallback(void* pWebViewWrapper, const std::string &message)
{
    WEB_MAP it=s_WebViewImpls.find((UIWebViewWrapper*)pWebViewWrapper);
    
    if (it != s_WebViewImpls.end())
    {
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_pWebViewDelegate)
        {
            webView->m_pWebViewDelegate->onJSCallback(webView, message);
        }
    }
}

void CAWebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme)
{
    [WebViewWrapper setJavascriptInterfaceScheme:scheme];
}


void CAWebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL)
{
    [WebViewWrapper loadHTMLString:string baseURL:baseURL];
}

void CAWebViewImpl::loadURL(const std::string &url)
{
    [WebViewWrapper loadUrl:url];
}

void CAWebViewImpl::loadFile(const std::string &fileName)
{
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    [WebViewWrapper loadFile:fullPath];
}

void CAWebViewImpl::stopLoading()
{
    [WebViewWrapper stopLoading];
}

void CAWebViewImpl::reload()
{
    [WebViewWrapper reload];
}

bool CAWebViewImpl::canGoBack()
{
    return WebViewWrapper.canGoBack;
}

bool CAWebViewImpl::canGoForward()
{
    return WebViewWrapper.canGoForward;
}

void CAWebViewImpl::goBack()
{
    [WebViewWrapper goBack];
}

void CAWebViewImpl::goForward()
{
    [WebViewWrapper goForward];
}

std::string CAWebViewImpl::evaluateJS(const std::string &js)
{
    return [WebViewWrapper evaluateJS:js];
}

void CAWebViewImpl::setScalesPageToFit(const bool scalesPageToFit)
{
    [WebViewWrapper setScalesPageToFit:scalesPageToFit];
}

void CAWebViewImpl::update(float dt)
{
    DRect cRect = m_pWebView->convertRectToWorldSpace(m_pWebView->getBounds());
    cRect.origin.x = s_dip_to_px(cRect.origin.x);
    cRect.origin.y = s_dip_to_px(cRect.origin.y);
    cRect.size.width = s_dip_to_px(cRect.size.width);
    cRect.size.height = s_dip_to_px(cRect.size.height);
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGFloat x = cRect.origin.x/scale;
    CGFloat y = cRect.origin.y/scale;
    CGFloat width = cRect.size.width/scale;
    CGFloat height = cRect.size.height/scale;
    
    [WebViewWrapper setFrameWith_X:x Y:y Width:width Height:height];
}

void CAWebViewImpl::setVisible(bool visible)
{
    [WebViewWrapper setVisible:visible];
}

CAImageView* CAWebViewImpl::getWebViewImage()
{
	UIImage* image = [WebViewWrapper getWebViewImage];
    if (image != NULL)
    {
        NSData* data = UIImagePNGRepresentation(image);
        if (data != NULL)
        {
            Byte* bytesData = (Byte*)[data bytes];
            if (bytesData != NULL)
            {
                return CAImageView::createWithImage(CAImage::createWithImageDataNoCache(bytesData, data.length));
            }
        }
    }
    return NULL;
}

NS_CC_END



