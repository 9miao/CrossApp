

#include "HttpClient.h"
#import "Http.h"

#define MAX_Thread 16

NS_CC_EXT_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef int int32_t;
#endif

static CAHttpClient *s_pHttpClient[MAX_Thread] = {0};
 // pointer to singleton


typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);

// Callback function used by libcurl for collect response data
static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}

// Callback function used by libcurl for collect header data
static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}


//Process Request
static int processTask(CAHttpRequest *request, NSString* requestType, void *stream, long *responseCode, void *headerStream, char *errorBuffer)
{
    CAHttpClient* httpClient = s_pHttpClient[request->getThreadID()];
    
    //create request with url
    NSString* urlstring = [NSString stringWithUTF8String:request->getUrl().c_str()];
    NSURL *url = [NSURL URLWithString:urlstring];
    
    NSMutableURLRequest *nsrequest = [NSMutableURLRequest requestWithURL:url
                                                            cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData
                                                            timeoutInterval:httpClient->getTimeoutForConnect()];
    
    //set request type
    [nsrequest setHTTPMethod:requestType];
    
    /* get custom header data (if set) */
    std::vector<std::string> headers=request->getHeaders();
    if(!headers.empty())
    {
        /* append custom headers one by one */
        for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
        {
            unsigned long i = it->find(':', 0);
            unsigned long length = it->size();
            std::string field = it->substr(0, i);
            std::string value = it->substr(i+1, length-i);
            NSString *headerField = [NSString stringWithUTF8String:field.c_str()];
            NSString *headerValue = [NSString stringWithUTF8String:value.c_str()];
            [nsrequest setValue:headerValue forHTTPHeaderField:headerField];
        }
    }
    
    //if request type is post or put,set header and data
    if([requestType  isEqual: @"POST"] || [requestType isEqual: @"PUT"])
    {
        if ([requestType isEqual: @"PUT"])
        {
            [nsrequest setValue: @"application/x-www-form-urlencoded" forHTTPHeaderField: @"Content-Type"];
        }
        
        char* requestDataBuffer = request->getRequestData();
        if (NULL !=  requestDataBuffer && 0 != request->getRequestDataSize())
        {
            NSData *postData = [NSData dataWithBytes:requestDataBuffer length:request->getRequestDataSize()];
            [nsrequest setHTTPBody:postData];
        }
    }
    
    
    Http *httpAsynConn = [[[Http alloc] init] autorelease];
    httpAsynConn.srcURL = urlstring;
    httpAsynConn.sslFile = nil;
    if(!httpClient->_sslCaFilename.empty())
    {
        long len = httpClient->_sslCaFilename.length();
        long pos = httpClient->_sslCaFilename.rfind('.', len-1);
        
        httpAsynConn.sslFile = [NSString stringWithUTF8String:httpClient->_sslCaFilename.substr(0, pos-1).c_str()];
    }
    [httpAsynConn startRequest:nsrequest];
    
    while( httpAsynConn.finish != true)
    {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
    
    //if http connection return error
    if (httpAsynConn.connError != nil)
    {
        NSString* errorString = [httpAsynConn.connError localizedDescription];
        strcpy(errorBuffer, [errorString UTF8String]);
        NSLog(@"=========%@", errorString);
        return 1;
    }

    //if http response got error, just log the error
    if (httpAsynConn.responseError != nil)
    {
        NSString* errorString = [httpAsynConn.responseError localizedDescription];
        strcpy(errorBuffer, [errorString UTF8String]);
    }
    
    *responseCode = httpAsynConn.responseCode;
    
    //handle response header
    NSMutableString *header = [NSMutableString string];
    [header appendFormat:@"HTTP/1.1 %ld %@\n", (long)httpAsynConn.responseCode, httpAsynConn.statusString];
    for (id key in httpAsynConn.responseHeader)
    {
        [header appendFormat:@"%@: %@\n", key, [httpAsynConn.responseHeader objectForKey:key]];
    }
    if (header.length > 0)
    {
        NSRange range = NSMakeRange(header.length-1, 1);
        [header deleteCharactersInRange:range];
    }
    NSData *headerData = [header dataUsingEncoding:NSUTF8StringEncoding];
    std::vector<char> *headerBuffer = (std::vector<char>*)headerStream;
    const void* headerptr = [headerData bytes];
    long headerlen = [headerData length];
    headerBuffer->insert(headerBuffer->end(), (char*)headerptr, (char*)headerptr+headerlen);
    
    //handle response data
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    const void* ptr = [httpAsynConn.responseData bytes];
    long len = [httpAsynConn.responseData length];
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+len);
    
    return 0;
}


// Worker thread
static void* networkThread(void *data)
{
    CAHttpClient* httpClient = (CAHttpClient*)data;

    CAHttpRequest *request = NULL;
    
    while (true) 
    {
        if (httpClient->need_quit)
        {
            break;
        }
        
        // step 1: send http request if the requestQueue isn't empty
        request = NULL;
        
        pthread_mutex_lock(&httpClient->s_requestQueueMutex); //Get request task from queue
        if (!httpClient->s_requestQueue.empty())
        {
            request = httpClient->s_requestQueue.front();
            httpClient->s_requestQueue.popFront();
            // request's refcount = 1 here
        }
        pthread_mutex_unlock(&httpClient->s_requestQueueMutex);
        
        if (NULL == request)
        {
        	// Wait for http request tasks from main thread
        	pthread_cond_wait(&httpClient->s_SleepCondition, &httpClient->s_SleepMutex);
            continue;
        }
        
        // step 2: libcurl sync access
        
        // Create a HttpResponse object, the default setting is http access failed
        CAHttpResponse *response = new CAHttpResponse(request);
        
        // request's refcount = 2 here, it's retained by HttpRespose constructor
        request->release();
        // ok, refcount = 1 now, only HttpResponse hold it.
        
        long responseCode = -1;
        int retValue = 0;
        
        NSString* requestType = nil;
        
        switch (request->getRequestType())
        {
            case CAHttpRequest::kHttpGet: // HTTP GET
                requestType = @"GET";
                break;
            
            case CAHttpRequest::kHttpPost: // HTTP POST
                requestType = @"POST";
                break;

            case CAHttpRequest::kHttpPut:
                requestType = @"PUT";
                break;

            case CAHttpRequest::kHttpDelete:
                requestType = @"DELETE";
                break;

			case CAHttpRequest::kHttpPostFile:
				requestType = @"POST";
				break;

            default:
                CCAssert(true, "CAHttpClient: unkown request type, only GET and POSt are supported");
                break;
        }
        
        retValue = processTask(request,
                               requestType,
                               response->getResponseData(),
                               &responseCode,
                               response->getResponseHeader(),
                               httpClient->s_errorBuffer);
        
        CCLog("--- %d", retValue);
        // write data to HttpResponse
        response->setResponseCode(responseCode);
        if (retValue != 0)
        {
            response->setSucceed(false);
            response->setErrorBuffer(httpClient->s_errorBuffer);
        }
        else
        {
            response->setSucceed(true);
        }
        
        // add response packet into queue
        pthread_mutex_lock(&httpClient->s_responseQueueMutex);
        httpClient->s_responseQueue.pushBack(response);
        pthread_mutex_unlock(&httpClient->s_responseQueueMutex);

        // resume dispatcher selector
        CAScheduler::getScheduler()->resumeTarget(httpClient);
        
    }
    
    // cleanup: if worker thread received quit signal, clean up un-completed request queue
    pthread_mutex_lock(&httpClient->s_requestQueueMutex);
    httpClient->s_requestQueue.clear();
    pthread_mutex_unlock(&httpClient->s_requestQueueMutex);
    httpClient->_asyncRequestCount -= httpClient->s_requestQueue.size();
    
    if (!httpClient->s_requestQueue.empty())
    {
        
        pthread_mutex_destroy(&httpClient->s_requestQueueMutex);
        pthread_mutex_destroy(&httpClient->s_responseQueueMutex);
        
        pthread_mutex_destroy(&httpClient->s_SleepMutex);
        pthread_cond_destroy(&httpClient->s_SleepCondition);

        httpClient->s_requestQueue.clear();
        httpClient->s_responseQueue.clear();
    }

    pthread_exit(NULL);
    
    return 0;
}


// HttpClient implementation
CAHttpClient* CAHttpClient::getInstance(int thread)
{
    if (thread >= MAX_Thread)
    {
        return NULL;
    }
    if (s_pHttpClient[thread] == NULL)
    {
        s_pHttpClient[thread] = new CAHttpClient(thread);
    }
    
    return s_pHttpClient[thread];
}

void CAHttpClient::destroyInstance(int thread)
{
    CCAssert(s_pHttpClient[thread], "");
    CAScheduler::unschedule(schedule_selector(CAHttpClient::dispatchResponseCallbacks), s_pHttpClient[thread]);
    s_pHttpClient[thread]->release();
}

CAHttpClient::CAHttpClient(int thread)
: _timeoutForConnect(30)
, _timeoutForRead(60)
, _threadID(thread)
, s_networkThread(pthread_t())
, s_requestQueueMutex(pthread_mutex_t())
, s_responseQueueMutex(pthread_mutex_t())
, s_SleepMutex(pthread_mutex_t())
, s_SleepCondition(pthread_cond_t())
, _asyncRequestCount(0)
, need_quit(0)
{
    CAScheduler::schedule(schedule_selector(CAHttpClient::dispatchResponseCallbacks), this, 0);
    CAScheduler::getScheduler()->pauseTarget(this);
}

CAHttpClient::~CAHttpClient()
{
    need_quit = true;
    
    if (!s_requestQueue.empty())
    {
    	pthread_cond_signal(&s_SleepCondition);
    }
    
    s_pHttpClient[_threadID] = NULL;
}

//Lazy create semaphore & mutex & thread
bool CAHttpClient::lazyInitThreadSemphore()
{
    if (!s_requestQueue.empty())
    {
        return true;
    }
    else
    {
        pthread_mutex_init(&s_requestQueueMutex, NULL);
        pthread_mutex_init(&s_responseQueueMutex, NULL);
        
        pthread_mutex_init(&s_SleepMutex, NULL);
        pthread_cond_init(&s_SleepCondition, NULL);

        pthread_create(&s_networkThread, NULL, networkThread, this);
        pthread_detach(s_networkThread);
        
        need_quit = false;
    }
    
    return true;
}

//Add a get task to queue
void CAHttpClient::send(CAHttpRequest* request)
{
    lazyInitThreadSemphore();
    
    CC_RETURN_IF(!request);
    
    ++_asyncRequestCount;
    
    request->setThreadID(_threadID);
    request->retain();
    
    pthread_mutex_lock(&s_requestQueueMutex);
    s_requestQueue.pushBack(request);
    pthread_mutex_unlock(&s_requestQueueMutex);
    
    // Notify thread start to work
    pthread_cond_signal(&s_SleepCondition);
}

// Poll and notify main thread if responses exists in queue
void CAHttpClient::dispatchResponseCallbacks(float delta)
{
    // CCLog("CAHttpClient::dispatchResponseCallbacks is running");
    
    CAHttpResponse* response = NULL;
    
    pthread_mutex_lock(&s_responseQueueMutex);
    if (!s_responseQueue.empty())
    {
        response = s_responseQueue.front();
        s_responseQueue.popFront();
    }
    pthread_mutex_unlock(&s_responseQueueMutex);
    
    if (response)
    {
        --_asyncRequestCount;
        
        CAHttpRequest *request = response->getHttpRequest();
        CC_RETURN_IF(request == NULL);
        CAObject *pTarget = request->getTarget();
        SEL_HttpResponse pSelector = request->getSelector();
        
        if (pTarget && pSelector)
        {
            (pTarget->*pSelector)(this, response);
        }
        
        response->release();
    }
    
    if (0 == _asyncRequestCount)
    {
        CAScheduler::getScheduler()->pauseTarget(this);
    }
    
}



NS_CC_EXT_END



