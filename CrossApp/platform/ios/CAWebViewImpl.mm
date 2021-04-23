
#import <UIKit/UIKit.h>
#import <JavaScriptCore/JavaScriptCore.h>
#include "CAWebViewImpl.h"
#include "EAGLView.h"
#include "basics/CAApplication.h"
#include "platform/CAFileUtils.h"
#include "platform/CADensityDpi.h"
#include "support/Json/lib_json/json_lib.h"
#import <WebKit/WebKit.h>

USING_NS_CC;

#define WebViewWrapper ((UIWebViewWrapper*)m_pWebViewWrapper)

@interface UIWebViewWrapper : NSObject<WKUIDelegate,WKNavigationDelegate,WKScriptMessageHandler>
@property(nonatomic, retain) WKWebView *uiWebView;
@property(nonatomic, retain) NSURL *requestUrl;

@property(nonatomic, readonly, getter=canGoBack) BOOL canGoBack;
@property(nonatomic, readonly, getter=canGoForward) BOOL canGoForward;

+ (instancetype)webViewWrapper;

- (void)setVisible:(bool)visible;

- (UIImage*)getWebViewImage;

- (void)setFrameWith_X:(float)x Y:(float)y Width:(float)width Height:(float)height;

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
    //移除观察者
    [self.uiWebView removeObserver:self
                      forKeyPath:NSStringFromSelector(@selector(estimatedProgress))];
    [self.uiWebView removeObserver:self
                      forKeyPath:NSStringFromSelector(@selector(title))];
    
    [[self.uiWebView configuration].userContentController removeScriptMessageHandlerForName:@"jsToOcNoPrams"];
    [[self.uiWebView configuration].userContentController removeScriptMessageHandlerForName:@"jsToOcWithPrams"];
    [self.uiWebView setUIDelegate:nil];
    [self.uiWebView removeFromSuperview];
    [self.uiWebView loadHTMLString:@"" baseURL:nil];
    [self.uiWebView stopLoading];
    [self.uiWebView release];
    [[NSURLCache sharedURLCache] removeAllCachedResponses];
    [super dealloc];
}

- (void)setupWebView
{
    if (!self.uiWebView)
    {
        //创建网页配置对象
        WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
        // 创建设置对象
        WKPreferences *preference = [[WKPreferences alloc]init];
        //最小字体大小 当将javaScriptEnabled属性设置为NO时，可以看到明显的效果
        preference.minimumFontSize = 0;
        //设置是否支持javaScript 默认是支持的
        preference.javaScriptEnabled = YES;
        // 在iOS上默认为NO，表示是否允许不经过用户交互由javaScript自动打开窗口
        preference.javaScriptCanOpenWindowsAutomatically = YES;
        config.preferences = preference;
        // 是使用h5的视频播放器在线播放, 还是使用原生播放器全屏播放
        config.allowsInlineMediaPlayback = YES;
        //设置是否允许画中画技术 在特定设备上有效
        config.allowsPictureInPictureMediaPlayback = YES;
        //设置请求的User-Agent信息中应用程序名称 iOS9后可用
        config.applicationNameForUserAgent = @"ChinaDailyForiPad";
//         //自定义的WKScriptMessageHandler 是为了解决内存不释放的问题
//        WeakWebViewScriptMessageDelegate *weakScriptMessageDelegate = [[WeakWebViewScriptMessageDelegate alloc] initWithDelegate:self];
        //这个类主要用来做native与JavaScript的交互管理
        WKUserContentController * wkUController = [[WKUserContentController alloc] init];
        //注册一个name为jsToOcNoPrams的js方法
        [wkUController addScriptMessageHandler:self  name:@"jsToOcNoPrams"];
        [wkUController addScriptMessageHandler:self  name:@"jsToOcWithPrams"];
       config.userContentController = wkUController;
        
        NSString *jSString = @"var meta = document.createElement('meta'); meta.setAttribute('name', 'viewport'); meta.setAttribute('content', 'width=device-width'); document.getElementsByTagName('head')[0].appendChild(meta);";
                //用于进行JavaScript注入
                WKUserScript *wkUScript = [[WKUserScript alloc] initWithSource:jSString injectionTime:WKUserScriptInjectionTimeAtDocumentEnd forMainFrameOnly:YES];
                [config.userContentController addUserScript:wkUScript];
        
        
        CGRect rect =  [[UIScreen mainScreen] bounds];
        rect.origin.x = rect.size.width;
        self.uiWebView = [[WKWebView alloc] initWithFrame:rect configuration:config];
        [self.uiWebView setUIDelegate:self];
        self.uiWebView.allowsBackForwardNavigationGestures = YES;
        
        
        //添加监测网页加载进度的观察者
        [self.uiWebView addObserver:self
                           forKeyPath:@"estimatedProgress"
                              options:0
                              context:nil];
           //添加监测网页标题title的观察者
        [self.uiWebView addObserver:self
                           forKeyPath:@"title"
                              options:NSKeyValueObservingOptionNew
                              context:nil];
        
        
        
        
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
    static float scale = [UIScreen mainScreen].scale;
    UIGraphicsBeginImageContextWithOptions(self.uiWebView.bounds.size, NO, scale);
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

- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL
{
    if (!self.uiWebView)
    {
        [self setupWebView];
    }
    [self.uiWebView loadHTMLString:@(string.c_str()) baseURL:[NSURL URLWithString:@(baseURL.c_str())]];
}

- (void)loadUrl:(const std::string &)urlString
{
    if (!self.uiWebView)
    {
        [self setupWebView];
    }
    NSURL *url = [NSURL URLWithString:@(urlString.c_str())];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url cachePolicy: NSURLRequestReturnCacheDataElseLoad timeoutInterval:30.0f];
    [self.uiWebView loadRequest:request];
    self.requestUrl = url;
    [request autorelease];
}

- (void)loadFile:(const std::string &)filePath
{
    if (!self.uiWebView)
    {
        [self setupWebView];
    }
    
    NSURL *url = [NSURL fileURLWithPath:@(filePath.c_str())];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url cachePolicy: NSURLRequestReturnCacheDataElseLoad timeoutInterval:30.0f];
    [self.uiWebView loadRequest:request];
    self.requestUrl = url;
    [request autorelease];
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

//kvo 监听进度 必须实现此方法
-(void)observeValueForKeyPath:(NSString *)keyPath
                  ofObject:(id)object
                    change:(NSDictionary<NSKeyValueChangeKey,id> *)change
                   context:(void *)context{
 if ([keyPath isEqualToString:NSStringFromSelector(@selector(estimatedProgress))]
     && object == self.uiWebView)
 {
    NSLog(@"网页加载进度 = %f", self.uiWebView.estimatedProgress);
//     self.progressView.progress = self.uiWebView.estimatedProgress;
     if (self.uiWebView.estimatedProgress >= 1.0f)
     {
         dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//             self.progressView.progress = 0;
         });
     }
 }
 else if([keyPath isEqualToString:@"title"]
          && object == self.uiWebView)
 {
//     self.navigationItem.title = self.uiWebView.title;
 }
 else
 {
     [super observeValueForKeyPath:keyPath
                          ofObject:object
                            change:change
                           context:context];
 }
}

//通过接收JS传出消息的name进行捕捉的回调方法
- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message
{
     NSLog(@"name:%@\\\\n body:%@\\\\n frameInfo:%@\\\\n",message.name,message.body,message.frameInfo);
     //用message.body获得JS传出的参数体
     NSDictionary * parameter = message.body;
     //JS调用OC
     if([message.name isEqualToString:@"jsToOcNoPrams"])
     {
         UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"js调用到了oc" message:@"不带参数" preferredStyle:UIAlertControllerStyleAlert];
         [alertController addAction:([UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
         }])];
         
         UIWindow * window = [UIApplication sharedApplication].keyWindow;
         [window.rootViewController presentViewController:alertController animated:YES completion:nil];
         
     }
     else if([message.name isEqualToString:@"jsToOcWithPrams"])
     {
         UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"js调用到了oc" message:parameter[@"params"] preferredStyle:UIAlertControllerStyleAlert];
         [alertController addAction:([UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action){}])];
         UIWindow * window = [UIApplication sharedApplication].keyWindow;
         [window.rootViewController presentViewController:alertController animated:YES completion:nil];
     }
    else
    {
        JSContext *context = [self.uiWebView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
        

        NSString *url = [self.requestUrl absoluteString];
        CAWebViewImpl::didFinishLoading(self, [url UTF8String]);
    }
}

// 页面开始加载时调用
- (void)webView:(WKWebView *)webView didStartProvisionalNavigation:(WKNavigation *)navigation
{
}

   // 页面加载失败时调用
- (void)webView:(WKWebView *)webView didFailProvisionalNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
    NSString *url = error.userInfo[NSURLErrorFailingURLStringErrorKey];
    if (url) {
        CAWebViewImpl::didFailLoading(self, [url UTF8String]);
    }
}

   // 当内容开始返回时调用
- (void)webView:(WKWebView *)webView didCommitNavigation:(WKNavigation *)navigation
{
    NSString *url = [self.requestUrl absoluteString];
    CAWebViewImpl::shouldStartLoading(self, [url UTF8String]);
}
   // 页面加载完成之后调用
- (void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation
{
    NSString *url = [self.requestUrl absoluteString];
    CAWebViewImpl::didFinishLoading(self, [url UTF8String]);
}

   //提交发生错误时调用
- (void)webView:(WKWebView *)webView didFailNavigation:(WKNavigation *)navigation withError:(NSError *)error {
}
  // 接收到服务器跳转请求即服务重定向时之后调用
- (void)webView:(WKWebView *)webView didReceiveServerRedirectForProvisionalNavigation:(WKNavigation *)navigation {
}
   // 根据WebView对于即将跳转的HTTP请求头信息和相关信息来决定是否跳转
- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler {
   
   NSString * urlStr = navigationAction.request.URL.absoluteString;
   NSLog(@"发送跳转请求：%@",urlStr);
   //自己定义的协议头
   NSString *htmlHeadString = @"github://";
   if([urlStr hasPrefix:htmlHeadString])
   {
       UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"通过截取URL调用OC" message:@"你想前往我的Github主页?" preferredStyle:UIAlertControllerStyleAlert];
       [alertController addAction:([UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
       }])];
       [alertController addAction:([UIAlertAction actionWithTitle:@"打开" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
           NSURL * url = [NSURL URLWithString:[urlStr stringByReplacingOccurrencesOfString:@"github://callName_?" withString:@""]];
           [[UIApplication sharedApplication] openURL:url options:@{} completionHandler:nil];
       }])];
       UIWindow * window = [UIApplication sharedApplication].keyWindow;
       [window.rootViewController presentViewController:alertController animated:YES completion:nil];       decisionHandler(WKNavigationActionPolicyCancel);
   }
   else
   {
       decisionHandler(WKNavigationActionPolicyAllow);
   }
}
   
   // 根据客户端受到的服务器响应头以及response相关信息来决定是否可以跳转
- (void)webView:(WKWebView *)webView decidePolicyForNavigationResponse:(WKNavigationResponse *)navigationResponse decisionHandler:(void (^)(WKNavigationResponsePolicy))decisionHandler{
   NSString * urlStr = navigationResponse.response.URL.absoluteString;
   NSLog(@"当前跳转地址：%@",urlStr);
   //允许跳转
   decisionHandler(WKNavigationResponsePolicyAllow);
   //不允许跳转
   //decisionHandler(WKNavigationResponsePolicyCancel);
}
   //需要响应身份验证时调用 同样在block中需要传入用户身份凭证
- (void)webView:(WKWebView *)webView didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * _Nullable credential))completionHandler{
   //用户身份信息
   NSURLCredential * newCred = [[NSURLCredential alloc] initWithUser:@"user123" password:@"123" persistence:NSURLCredentialPersistenceNone];
   //为 challenge 的发送方提供 credential
   [challenge.sender useCredential:newCred forAuthenticationChallenge:challenge];
   completionHandler(NSURLSessionAuthChallengeUseCredential,newCred);
}
   //进程被终止时调用
- (void)webViewWebContentProcessDidTerminate:(WKWebView *)webView
{
}

/**
    *  web界面中有弹出警告框时调用
    *
    *  @param webView           实现该代理的webview
    *  @param message           警告框中的内容
    *  @param completionHandler 警告框消失调用
    */
- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(void))completionHandler {
   UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"HTML的弹出框" message:message?:@"" preferredStyle:UIAlertControllerStyleAlert];
   [alertController addAction:([UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
       completionHandler();
   }])];
    UIWindow * window = [UIApplication sharedApplication].keyWindow;
    [window.rootViewController presentViewController:alertController animated:YES completion:nil];
    
}
   // 确认框
   //JavaScript调用confirm方法后回调的方法 confirm是js中的确定框，需要在block中把用户选择的情况传递进去
- (void)webView:(WKWebView *)webView runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(BOOL))completionHandler{
   UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"" message:message?:@"" preferredStyle:UIAlertControllerStyleAlert];
   [alertController addAction:([UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
       completionHandler(NO);
   }])];
   [alertController addAction:([UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
       completionHandler(YES);
   }])];
    UIWindow * window = [UIApplication sharedApplication].keyWindow;
    [window.rootViewController presentViewController:alertController animated:YES completion:nil];
    
}
   // 输入框
   //JavaScript调用prompt方法后回调的方法 prompt是js中的输入框 需要在block中把用户输入的信息传入
- (void)webView:(WKWebView *)webView runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(NSString *)defaultText initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(NSString * _Nullable))completionHandler{
   UIAlertController *alertController = [UIAlertController alertControllerWithTitle:prompt message:@"" preferredStyle:UIAlertControllerStyleAlert];
   [alertController addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
       textField.text = defaultText;
   }];
   [alertController addAction:([UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
       completionHandler(alertController.textFields[0].text?:@"");
   }])];
    UIWindow * window = [UIApplication sharedApplication].keyWindow;
    [window.rootViewController presentViewController:alertController animated:YES completion:nil];;
}
   // 页面是弹出窗口 _blank 处理
- (WKWebView *)webView:(WKWebView *)webView createWebViewWithConfiguration:(WKWebViewConfiguration *)configuration forNavigationAction:(WKNavigationAction *)navigationAction windowFeatures:(WKWindowFeatures *)windowFeatures {
   if (!navigationAction.targetFrame.isMainFrame) {
       [webView loadRequest:navigationAction.request];
   }
   return nil;
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
    WEB_MAP it=s_WebViewImpls.find((UIWebViewWrapper*)pWebViewWrapper);
    
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
    WEB_MAP it=s_WebViewImpls.find((UIWebViewWrapper*)pWebViewWrapper);
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
    WEB_MAP it=s_WebViewImpls.find((UIWebViewWrapper*)pWebViewWrapper);
    
    if (it != s_WebViewImpls.end())
    {
        CAWebView* webView = it->second->m_pWebView;
        if (webView && webView->m_obJSCallback)
        {
            webView->m_obJSCallback(message);
        }
    }
}

void CAWebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme)
{
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

void CAWebViewImpl::getWebViewImage(const std::function<void(CAImage*)>& callback)
{
	UIImage* image = [WebViewWrapper getWebViewImage];
    if (image != NULL)
    {
        NSData* iOSData = UIImagePNGRepresentation(image);
        unsigned char* data = (unsigned char*)malloc([iOSData length]);
        [iOSData getBytes:data];
        
        CAData* ca_data = CAData::create();
        ca_data->fastSet(data, [iOSData length]);
        CAImage *image = CAImage::createWithImageDataNoCache(ca_data);

        if (callback)
        {
            callback(image);
        }
    }
}

NS_CC_END



