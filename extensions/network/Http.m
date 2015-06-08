
#import "Http.h"

@implementation Http
@synthesize srcURL;
@synthesize sslFile;
@synthesize responseHeader;
@synthesize responseData;
@synthesize getDataTime;
@synthesize responseCode;
@synthesize statusString;
@synthesize responseError;
@synthesize connError;
@synthesize conn;
@synthesize finish;
@synthesize runLoop;

- (void)dealloc
{
    [srcURL release];
    [sslFile release];
    [responseHeader release];
    [responseData release];
    [statusString release];
    [responseError release];
    [conn release];
    [runLoop release];
    
    [super dealloc];
}

- (void) startRequest:(NSURLRequest *)request
{
    finish = false;

    self.responseData = [NSMutableData data];
    getDataTime = 0;

    self.responseError = nil;
    self.connError = nil;
    
    self.conn = [[[NSURLConnection alloc] initWithRequest:request delegate:self startImmediately:NO] autorelease];
    [self.conn scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];

    [self.conn start];
}

- (void) connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {

    NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
    self.responseHeader = [httpResponse allHeaderFields];

    responseCode = httpResponse.statusCode;
    self.statusString = [NSHTTPURLResponse localizedStringForStatusCode:responseCode];
    if(responseCode == 200)
        self.statusString = @"OK";

    if (responseCode < 200 || responseCode >= 300)
    {
        self.responseError = [NSError errorWithDomain:@"CCBackendDomain" code:responseCode userInfo:@{NSLocalizedDescriptionKey: @"Bad HTTP Response Code"}];
    }
    
    [responseData setLength:0];
}

- (void)connection:(NSURLConnection *)connection 
    didReceiveData:(NSData *)data
{
    [responseData appendData:data];
    getDataTime++;
}

- (void)connection:(NSURLConnection *)connection 
  didFailWithError:(NSError *)error
{
    self.connError = error;
    finish = true;
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    finish = true;
}

- (BOOL) shouldTrustProtectionSpace:(NSURLProtectionSpace*)protectionSpace
{
    if(sslFile == nil)
        return YES;
    NSString *certPath = [[NSBundle mainBundle] pathForResource:sslFile ofType:@"der"];
    NSData *certData = [[NSData alloc] initWithContentsOfFile:certPath];
    CFDataRef certDataRef = (CFDataRef)certData;
    SecCertificateRef cert = SecCertificateCreateWithData(NULL, certDataRef);
    
    CFArrayRef certArrayRef = CFArrayCreate(NULL, (void*)&cert, 1, NULL);
    SecTrustRef serverTrust = protectionSpace.serverTrust;
    SecTrustSetAnchorCertificates(serverTrust, certArrayRef);
    
    SecTrustResultType trustResult;
    SecTrustEvaluate(serverTrust, &trustResult);
    
    if(trustResult == kSecTrustResultRecoverableTrustFailure)
    {
        CFDataRef errDataRef = SecTrustCopyExceptions(serverTrust);
        SecTrustSetExceptions(serverTrust, errDataRef);
        
        SecTrustEvaluate(serverTrust, &trustResult);
    }
    
    return trustResult = kSecTrustResultUnspecified || trustResult == kSecTrustResultProceed;
}

- (void) connection:(NSURLConnection *)connection willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
{
    id <NSURLAuthenticationChallengeSender> sender = challenge.sender;
    NSURLProtectionSpace *protectionSpace = challenge.protectionSpace;
    
    if([self shouldTrustProtectionSpace:protectionSpace])
    {
        SecTrustRef trust = [protectionSpace serverTrust];

        NSURLCredential *credential = [NSURLCredential credentialForTrust:trust];
        [sender useCredential:credential forAuthenticationChallenge:challenge];
    }
    else
    {
        [sender cancelAuthenticationChallenge:challenge];
    }
}

@end
