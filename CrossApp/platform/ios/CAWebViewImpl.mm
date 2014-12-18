
#import <UIKit/UIKit.h>
#include "CAWebViewImpl.h"
#include "EAGLView.h"
#include "platform/CCFileUtils.h"
#include "basics/CAApplication.h"


USING_NS_CC;

@interface UIWebViewWrapper : NSObject

@property(nonatomic, readonly, getter=canGoBack) BOOL canGoBack;
@property(nonatomic, readonly, getter=canGoForward) BOOL canGoForward;

+ (instancetype)webViewWrapper;

- (void)setVisible:(bool)visible;

- (UIImage*)getWebViewImage;

- (void)setFrameWithX:(float)x y:(float)y width:(float)width height:(float)height;

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme;

- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL;

- (void)loadUrl:(const std::string &)urlString;

- (void)loadFile:(const std::string &)filePath;

- (void)stopLoading;

- (void)reload;

- (void)evaluateJS:(const std::string &)js;

- (void)goBack;

- (void)goForward;

- (void)setScalesPageToFit:(const bool)scalesPageToFit;
@end



@interface UIWebViewWrapper () <UIWebViewDelegate>
@property(nonatomic, retain) UIWebView *uiWebView;
@property(nonatomic, copy) NSString *jsScheme;
@end

@implementation UIWebViewWrapper {
    
}

+ (instancetype)webViewWrapper {
    return [[[self alloc] init] autorelease];
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.uiWebView = nil;
    }
    return self;
}

- (void)dealloc {
    self.uiWebView.delegate = nil;
    [self.uiWebView removeFromSuperview];
    self.jsScheme = nil;
    [super dealloc];
}

- (void)setupWebView {
    if (!self.uiWebView) {
        self.uiWebView = [[[UIWebView alloc] init] autorelease];
        self.uiWebView.delegate = self;
    }
    if (!self.uiWebView.superview) {
        EAGLView * eaglview = [EAGLView sharedEGLView];
        [eaglview addSubview:self.uiWebView];
        [eaglview bringSubviewToFront: self.uiWebView];
    }
}

- (void)setVisible:(bool)visible {
    self.uiWebView.hidden = !visible;
}

- (UIImage*)getWebViewImage{
    if (UIGraphicsBeginImageContextWithOptions!=NULL) {
        UIGraphicsBeginImageContextWithOptions(self.uiWebView.frame.size, NO, 0);
    }
    else{
        UIGraphicsBeginImageContext(self.uiWebView.frame.size);
    }
    
    [self.uiWebView.layer renderInContext:UIGraphicsGetCurrentContext()];
    return UIGraphicsGetImageFromCurrentImageContext();
}


- (void)setFrameWithX:(float)x y:(float)y width:(float)width height:(float)height {
    if (!self.uiWebView) {[self setupWebView];}
    CGRect newFrame = CGRectMake(x, y, width, height);
    if (!CGRectEqualToRect(self.uiWebView.frame, newFrame)) {
        self.uiWebView.frame = CGRectMake(x, y, width, height);
    }
}

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme {
    self.jsScheme = @(scheme.c_str());
}


- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL {
    [self.uiWebView loadHTMLString:@(string.c_str()) baseURL:[NSURL URLWithString:@(baseURL.c_str())]];
}

- (void)loadUrl:(const std::string &)urlString {
    if (!self.uiWebView) {[self setupWebView];}
    NSURL *url = [NSURL URLWithString:@(urlString.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.uiWebView loadRequest:request];
}

- (void)loadFile:(const std::string &)filePath {
    if (!self.uiWebView) {[self setupWebView];}
    NSURL *url = [NSURL fileURLWithPath:@(filePath.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.uiWebView loadRequest:request];
}

- (void)stopLoading {
    [self.uiWebView stopLoading];
}

- (void)reload {
    [self.uiWebView reload];
}

- (BOOL)canGoForward {
    return self.uiWebView.canGoForward;
}

- (BOOL)canGoBack {
    return self.uiWebView.canGoBack;
}

- (void)goBack {
    [self.uiWebView goBack];
}

- (void)goForward {
    [self.uiWebView goForward];
}

- (void)evaluateJS:(const std::string &)js {
    if (!self.uiWebView) {[self setupWebView];}
    [self.uiWebView stringByEvaluatingJavaScriptFromString:@(js.c_str())];
}

- (void)setScalesPageToFit:(const bool)scalesPageToFit {
    self.uiWebView.scalesPageToFit = scalesPageToFit;
}

#pragma mark - UIWebViewDelegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *url = [[request URL] absoluteString];
    if ([[[request URL] scheme] isEqualToString:self.jsScheme]) {
        CAWebViewImpl::onJsCallback(self, [url UTF8String]);
        return NO;
    }
    if (url) {
        return CAWebViewImpl::shouldStartLoading(self, [url UTF8String]);
    }
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    NSString *url = [[webView.request URL] absoluteString];
    CAWebViewImpl::didFinishLoading(self, [url UTF8String]);
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    NSString *url = error.userInfo[NSURLErrorFailingURLStringErrorKey];
    if (url) {
        CAWebViewImpl::didFailLoading(self, [url UTF8String]);
    }
}

@end




NS_CC_BEGIN

std::map<UIWebViewWrapper*, CAWebViewImpl*> s_WebViewImpls;

CAWebViewImpl::CAWebViewImpl(CAWebView *webView)
        : _uiWebViewWrapper([UIWebViewWrapper webViewWrapper]),
          _webView(webView) {
    [_uiWebViewWrapper retain];
    [_uiWebViewWrapper setVisible:true];
    s_WebViewImpls [_uiWebViewWrapper] = this;
}

CAWebViewImpl::~CAWebViewImpl(){
    s_WebViewImpls.erase(_uiWebViewWrapper);
    [_uiWebViewWrapper release];
    _uiWebViewWrapper = NULL;
}

bool CAWebViewImpl::shouldStartLoading(UIWebViewWrapper* pWebViewWrapper, const std::string &url)
{
    std::map<UIWebViewWrapper*, CAWebViewImpl*>::iterator it=s_WebViewImpls.find(pWebViewWrapper);
    if (it != s_WebViewImpls.end()) {
        CAWebView* webView = it->second->_webView;
        if (webView && webView->m_pWebViewDelegate) {
            return webView->m_pWebViewDelegate->onShouldStartLoading(webView, url);
        }
    }
    return true;
}

void CAWebViewImpl::didFinishLoading(UIWebViewWrapper* pWebViewWrapper, const std::string &url)
{
    std::map<UIWebViewWrapper*, CAWebViewImpl*>::iterator it=s_WebViewImpls.find(pWebViewWrapper);
    if (it != s_WebViewImpls.end()) {
        CAWebView* webView = it->second->_webView;
        if (webView && webView->m_pWebViewDelegate) {
            webView->m_pWebViewDelegate->onDidFinishLoading(webView, url);
        }
    }
}

void CAWebViewImpl::didFailLoading(UIWebViewWrapper* pWebViewWrapper, const std::string &url)
{
    std::map<UIWebViewWrapper*, CAWebViewImpl*>::iterator it=s_WebViewImpls.find(pWebViewWrapper);
    if (it != s_WebViewImpls.end()) {
        CAWebView* webView = it->second->_webView;
        if (webView && webView->m_pWebViewDelegate) {
            webView->m_pWebViewDelegate->onDidFailLoading(webView, url);
        }
    }
}

void CAWebViewImpl::onJsCallback(UIWebViewWrapper* pWebViewWrapper, const std::string &message)
{
    std::map<UIWebViewWrapper*, CAWebViewImpl*>::iterator it=s_WebViewImpls.find(pWebViewWrapper);
    if (it != s_WebViewImpls.end()) {
        CAWebView* webView = it->second->_webView;
        if (webView && webView->m_pWebViewDelegate) {
            webView->m_pWebViewDelegate->onJSCallback(webView, message);
        }
    }
}

void CAWebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme) {
    [_uiWebViewWrapper setJavascriptInterfaceScheme:scheme];
}


void CAWebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL) {
    [_uiWebViewWrapper loadHTMLString:string baseURL:baseURL];
}

void CAWebViewImpl::loadURL(const std::string &url) {
    [_uiWebViewWrapper loadUrl:url];
}

void CAWebViewImpl::loadFile(const std::string &fileName) {
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    [_uiWebViewWrapper loadFile:fullPath];
}

void CAWebViewImpl::stopLoading() {
    [_uiWebViewWrapper stopLoading];
}

void CAWebViewImpl::reload() {
    [_uiWebViewWrapper reload];
}

bool CAWebViewImpl::canGoBack() {
    return _uiWebViewWrapper.canGoBack;
}

bool CAWebViewImpl::canGoForward() {
    return _uiWebViewWrapper.canGoForward;
}

void CAWebViewImpl::goBack() {
    [_uiWebViewWrapper goBack];
}

void CAWebViewImpl::goForward() {
    [_uiWebViewWrapper goForward];
}

void CAWebViewImpl::evaluateJS(const std::string &js) {
    [_uiWebViewWrapper evaluateJS:js];
}

void CAWebViewImpl::setScalesPageToFit(const bool scalesPageToFit) {
    [_uiWebViewWrapper setScalesPageToFit:scalesPageToFit];
}

void CAWebViewImpl::visit() {
    
    CCRect cRect = _webView->convertRectToWorldSpace(_webView->getBounds());
    
    CGFloat scale = [[UIScreen mainScreen] scale];

    [_uiWebViewWrapper setFrameWithX:cRect.origin.x/scale y:cRect.origin.y/scale width:cRect.size.width/scale height:cRect.size.height/scale];
}

void CAWebViewImpl::setVisible(bool visible){
    [_uiWebViewWrapper setVisible:visible];
}

CAImageView* CAWebViewImpl::getWebViewImage(){
	UIImage* image = [_uiWebViewWrapper getWebViewImage];
    if (image != NULL) {
        NSData* data = UIImagePNGRepresentation(image);
        if (data != NULL) {
            Byte* bytesData = (Byte*)[data bytes];
            if (bytesData != NULL) {
                return CAImageView::createWithImage(CAImage::createWithData(bytesData, data.length, ""));
            }
        }
    }
    return NULL;
}

NS_CC_END



