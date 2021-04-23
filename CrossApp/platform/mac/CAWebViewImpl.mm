
#include "CAWebViewImpl.h"
#include "view/CAWebView.h"
#include "basics/CAApplication.h"
#include "platform/CAFileUtils.h"
#include "platform/CADensityDpi.h"
#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import "EAGLView.h"

#define WebViewWrapper ((NSWebView_MAC*)m_pWebViewWrapper)



@interface NSWebView_MAC : NSObject <WebFrameLoadDelegate>
@property(nonatomic, retain) WebView *webView;
@property(nonatomic, copy) NSString *jsScheme;

@property(nonatomic, readonly, getter=canGoBack) BOOL canGoBack;
@property(nonatomic, readonly, getter=canGoForward) BOOL canGoForward;

+ (instancetype)webViewWrapper;

- (void)setVisible:(bool)visible;

- (NSImage*)getWebViewImage;

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

std::map<NSWebView_MAC*, CrossApp::CAWebViewImpl*> s_WebViewImpls;

@implementation NSWebView_MAC
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
        self.webView = nil;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(fullScreenChanged) name:@"FULL_SCREEN_CHANGED" object:nil];
    }
    return self;
}

- (void)dealloc
{
    [self.webView removeFromSuperview];
    [self.webView release];
    [[NSURLCache sharedURLCache] removeAllCachedResponses];
    self.jsScheme = nil;
    [super dealloc];
}

- (void)setupWebView
{
    if (!self.webView)
    {
        self.webView = [[WebView alloc] init];
        [self.webView setWantsLayer:YES];
        self.webView.frameLoadDelegate = self;
    }
    if (!self.webView.superview)
    {
        EAGLView * eaglview = [EAGLView sharedEGLView];
        [eaglview addSubview:self.webView];
    }
    
    NSURLCache *sharedCache = [[[NSURLCache alloc] initWithMemoryCapacity:0 diskCapacity:0 diskPath:nil] autorelease];
    [NSURLCache setSharedURLCache:sharedCache];
}

- (void)fullScreenChanged
{
    s_WebViewImpls.at(self)->update(0);
}

- (void)setVisible:(bool)visible
{
    [self.webView setWantsLayer:!visible];
    self.webView.hidden = !visible;
}

- (NSImage*)getWebViewImage
{
    return nil;
}


- (void)setFrameWith_X:(float)x Y:(float)y Width:(float)width Height:(float)height
{
    if (!self.webView)
    {
        [self setupWebView];
    }
    
    NSRect newFrame = NSMakeRect(x, y, width, height);
    self.webView.frame = newFrame;
}

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme
{
    self.jsScheme = @(scheme.c_str());
}


- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL
{
    if (!self.webView)
    {
        [self setupWebView];
    }
    NSURL *url = [NSURL URLWithString:@(baseURL.c_str())];
    NSString *request = [NSString stringWithUTF8String:string.c_str()];
    
    [[self.webView mainFrame] loadHTMLString:request baseURL:url];
}

- (void)loadUrl:(const std::string &)urlString
{
    if (!self.webView)
    {
        [self setupWebView];
    }
    
    NSURL *url = [NSURL URLWithString:@(urlString.c_str())];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url cachePolicy: NSURLRequestReturnCacheDataElseLoad timeoutInterval:30.0f];
    [[self.webView mainFrame] loadRequest:request];
    [request autorelease];
}

- (void)loadFile:(const std::string &)filePath
{
    if (!self.webView)
    {
        [self setupWebView];
    }
    
    NSURL *url = [NSURL fileURLWithPath:@(filePath.c_str())];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url cachePolicy: NSURLRequestReturnCacheDataElseLoad timeoutInterval:30.0f];
    [[self.webView mainFrame] loadRequest:request];
    [request autorelease];
}

- (void)stopLoading
{
    [self.webView stopLoading:self];
}

- (void)reload
{
    //刷新
    [self.webView reload:self];
}

- (BOOL)canGoForward
{
    return self.webView.canGoForward;
}

- (BOOL)canGoBack
{
    return self.webView.canGoBack;
}

- (void)goBack
{
    [self.webView goBack:self];
}

- (void)goForward
{
    [self.webView goForward:self];
}

- (std::string)evaluateJS:(const std::string &)js
{
    if (!self.webView) {[self setupWebView];}
    NSString* s = [self.webView stringByEvaluatingJavaScriptFromString:@(js.c_str())];
    return [s UTF8String];
}

- (void)setScalesPageToFit:(const bool)scalesPageToFit
{
    
}

#pragma mark - NSWebViewDelegate

//获取每次加载页面的request
- (void)webView:(WebView *)webView decidePolicyForMIMEType:(NSString *)type request:(NSURLRequest *)request frame:(WebFrame *)frame decisionListener:(id<WebPolicyDecisionListener>)listener
{
    
    
}

//获取加载页面的Title
- (void)webView:(WebView *)sender didReceiveTitle:(NSString *)title forFrame:(WebFrame *)frame
{
    
}

//加载完成
- (void)webView:(WebView *)sender didFinishLoadForFrame:(WebFrame *)frame
{
    NSString *url = [sender mainFrameURL];
    CrossApp::CAWebViewImpl::didFinishLoading(self, [url UTF8String]);
}

//加载失败
- (void)webView:(WebView *)sender didFailLoadWithError:(NSError *)error forFrame:(WebFrame *)frame
{
    NSString *url = [sender mainFrameURL];
    if (url)
    {
        CrossApp::CAWebViewImpl::didFailLoading(self, [url UTF8String]);
    }
}

@end

NS_CC_BEGIN

CAWebViewImpl::CAWebViewImpl(CAWebView *webView)
: m_pWebView(webView)
, m_pWebViewWrapper([NSWebView_MAC webViewWrapper])
{
    [WebViewWrapper retain];
    [WebViewWrapper setVisible:YES];
    s_WebViewImpls [WebViewWrapper] = this;
}

CAWebViewImpl::~CAWebViewImpl() 
{
    s_WebViewImpls.erase(WebViewWrapper);
    [[NSNotificationCenter defaultCenter] removeObserver:WebViewWrapper];
    [WebViewWrapper release];
    m_pWebViewWrapper = NULL;
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
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
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

typedef std::map<NSWebView_MAC*, CAWebViewImpl*>::iterator WEB_MAP;

bool CAWebViewImpl::shouldStartLoading(void* pWebViewWrapper, const std::string &url)
{
    WEB_MAP it = s_WebViewImpls.find((NSWebView_MAC*)pWebViewWrapper);
    if (it != s_WebViewImpls.end())
    {
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_obStartLoading)
        {
            if (!webView->m_obStartLoading(url))
                return false;
        }
    }
    return true;
}

void CAWebViewImpl::didFinishLoading(void* pWebViewWrapper, const std::string &url)
{
    WEB_MAP it=s_WebViewImpls.find((NSWebView_MAC*)pWebViewWrapper);
    
    if (it != s_WebViewImpls.end()) {
        
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_obFinishLoading)
        {
            webView->m_obFinishLoading(url);
        }
    }
}

void CAWebViewImpl::didFailLoading(void* pWebViewWrapper, const std::string &url)
{
    WEB_MAP it=s_WebViewImpls.find((NSWebView_MAC*)pWebViewWrapper);
    if (it != s_WebViewImpls.end())
    {
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_obFailLoading)
        {
            webView->m_obFailLoading(url);
        }
    }
}

void CAWebViewImpl::onJsCallback(void* pWebViewWrapper, const std::string &message)
{
    WEB_MAP it=s_WebViewImpls.find((NSWebView_MAC*)pWebViewWrapper);
    
    if (it != s_WebViewImpls.end())
    {
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_obJSCallback)
        {
            webView->m_obJSCallback(message);
        }
    }
}

void CAWebViewImpl::update(float dt)
{
    do
    {
        CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->m_pWebView->convertToWorldSpace(DPointZero);
        point.y = CAApplication::getApplication()->getWinSize().height - point.y;
        point.y = point.y - m_pWebView->m_obContentSize.height;
        
        NSPoint origin;
        origin.x = s_dip_to_px(point.x) / 1;
        origin.y = s_dip_to_px(point.y) / 1;
        
        DSize worldContentSize = this->m_pWebView->convertToWorldSize(this->m_pWebView->m_obContentSize);
    
        NSSize size;
        size.width = s_dip_to_px(worldContentSize.width) / 1;
        size.height =  s_dip_to_px(worldContentSize.height) / 1;
        
        //        CC_BREAK_IF(m_obLastPoint.equals(point));
        
        [WebViewWrapper setFrameWith_X:origin.x Y:origin.y Width:size.width Height:size.height];
    }
    while (0);
}

void CAWebViewImpl::setVisible(bool visible) 
{
    [WebViewWrapper setVisible:visible];
}

void CAWebViewImpl::getWebViewImage(const std::function<void(CAImage*)>& callback)
{
    NSImage* image_MAC = [[[NSImage alloc]initWithData:[WebViewWrapper.webView dataWithPDFInsideRect:[WebViewWrapper.webView bounds]]]autorelease];
    
    NSData* data_MAC = [image_MAC TIFFRepresentationUsingCompression:NSTIFFCompressionNone factor:1];
    
    unsigned char* pData = (unsigned char*)malloc([data_MAC length]);
    [data_MAC getBytes:pData length:[data_MAC length]];
    
    CAData* data = CAData::create();
    data->fastSet(pData, [data_MAC length]);
    CAImage *image = CAImage::createWithImageDataNoCache(data);

    if (callback)
    {
        callback(image);
    }
}

NS_CC_END
