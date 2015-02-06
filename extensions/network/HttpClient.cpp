

#include "HttpClient.h"

#define MAX_Thread 16

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)

#include <queue>
#include <pthread.h>
#include <errno.h>
#include "curl/curl.h"

NS_CC_EXT_BEGIN

static pthread_t        s_networkThread[MAX_Thread] =
{
    pthread_t(),pthread_t(),pthread_t(),pthread_t(),pthread_t(),
    pthread_t(),pthread_t(),pthread_t(),pthread_t(),pthread_t()
};
static pthread_mutex_t  s_requestQueueMutex[MAX_Thread] =
{
    pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),
    pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t()
};
static pthread_mutex_t  s_responseQueueMutex[MAX_Thread] =
{
    pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),
    pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t()
};
static pthread_mutex_t		s_SleepMutex[MAX_Thread] =
{
    pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),
    pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t(),pthread_mutex_t()
};
static pthread_cond_t		s_SleepCondition[MAX_Thread] =
{
    pthread_cond_t(),pthread_cond_t(),pthread_cond_t(),pthread_cond_t(),pthread_cond_t(),
    pthread_cond_t(),pthread_cond_t(),pthread_cond_t(),pthread_cond_t(),pthread_cond_t()
};

static unsigned long    s_asyncRequestCount[MAX_Thread] = {0};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef int int32_t;
#endif

static bool need_quit[MAX_Thread] = {0};

static CADeque<CAHttpRequest*> s_requestQueue[MAX_Thread];

static CADeque<CAHttpResponse*> s_responseQueue[MAX_Thread];


static CAHttpClient *s_pHttpClient[MAX_Thread] = {0};
 // pointer to singleton

static char s_errorBuffer[MAX_Thread][CURL_ERROR_SIZE];

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


static int processGetTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *errorCode, write_callback headerCallback, void *headerStream);
static int processPostTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *errorCode, write_callback headerCallback, void *headerStream);
static int processPutTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *errorCode, write_callback headerCallback, void *headerStream);
static int processDeleteTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *errorCode, write_callback headerCallback, void *headerStream);
static int processPostFileTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *errorCode, write_callback headerCallback, void *headerStream);
// int processDownloadTask(HttpRequest *task, write_callback callback, void *stream, int32_t *errorCode);


// Worker thread
static void* networkThread(void *data)
{
    int thread = *(int*)data;

    CAHttpRequest *request = NULL;
    
    while (true) 
    {
        if (need_quit[thread])
        {
            break;
        }
        
        // step 1: send http request if the requestQueue isn't empty
        request = NULL;
        
        pthread_mutex_lock(&s_requestQueueMutex[thread]); //Get request task from queue
        if (!s_requestQueue[thread].empty())
        {
            request = s_requestQueue[thread].front();
            s_requestQueue[thread].popFront();
            // request's refcount = 1 here
        }
        pthread_mutex_unlock(&s_requestQueueMutex[thread]);
        
        if (NULL == request)
        {
        	// Wait for http request tasks from main thread
        	pthread_cond_wait(&s_SleepCondition[thread], &s_SleepMutex[thread]);
            continue;
        }
        
        // step 2: libcurl sync access
        
        // Create a HttpResponse object, the default setting is http access failed
        CAHttpResponse *response = new CAHttpResponse(request);
        
        // request's refcount = 2 here, it's retained by HttpRespose constructor
        request->release();
        // ok, refcount = 1 now, only HttpResponse hold it.
        
        int32_t responseCode = -1;
        int retValue = 0;

        // Process the request -> get response packet
        switch (request->getRequestType())
        {
            case CAHttpRequest::kHttpGet: // HTTP GET
                retValue = processGetTask(request,
                                          writeData, 
                                          response->getResponseData(), 
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader());
                break;
            
            case CAHttpRequest::kHttpPost: // HTTP POST
                retValue = processPostTask(request,
                                           writeData, 
                                           response->getResponseData(), 
                                           &responseCode,
                                           writeHeaderData,
                                           response->getResponseHeader());
                break;

            case CAHttpRequest::kHttpPut:
                retValue = processPutTask(request,
                                          writeData,
                                          response->getResponseData(),
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader());
                break;

            case CAHttpRequest::kHttpDelete:
                retValue = processDeleteTask(request,
                                             writeData,
                                             response->getResponseData(),
                                             &responseCode,
                                             writeHeaderData,
                                             response->getResponseHeader());
                break;

			case CAHttpRequest::kHttpPostFile:
				retValue = processPostFileTask(request,
											writeData,
											response->getResponseData(),
											&responseCode,
											writeHeaderData,
											response->getResponseHeader());
				break;

            default:
                CCAssert(true, "CAHttpClient: unkown request type, only GET and POSt are supported");
                break;
        }

        // write data to HttpResponse
        response->setResponseCode(responseCode);
        if (retValue != 0)
        {
            response->setSucceed(false);
            response->setErrorBuffer(s_errorBuffer[thread]);
        }
        else
        {
            response->setSucceed(true);
        }

        
        // add response packet into queue
        pthread_mutex_lock(&s_responseQueueMutex[thread]);
        s_responseQueue[thread].pushBack(response);
        pthread_mutex_unlock(&s_responseQueueMutex[thread]);
        
        // resume dispatcher selector
        CAScheduler::getScheduler()->resumeTarget(CAHttpClient::getInstance(thread));
    }
    
    // cleanup: if worker thread received quit signal, clean up un-completed request queue
    pthread_mutex_lock(&s_requestQueueMutex[thread]);
    s_requestQueue[thread].clear();
    pthread_mutex_unlock(&s_requestQueueMutex[thread]);
    s_asyncRequestCount[thread] -= s_requestQueue[thread].size();
    
    if (s_requestQueue != NULL) {
        
        pthread_mutex_destroy(&s_requestQueueMutex[thread]);
        pthread_mutex_destroy(&s_responseQueueMutex[thread]);
        
        pthread_mutex_destroy(&s_SleepMutex[thread]);
        pthread_cond_destroy(&s_SleepCondition[thread]);

        s_requestQueue[thread].clear();
        s_responseQueue[thread].clear();
    }

    pthread_exit(NULL);
    
    return 0;
}

//Configure curl's timeout property
static bool configureCURL(CURL *handle)
{
    if (!handle) {
        return false;
    }
    
    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, s_errorBuffer);
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, CAHttpClient::getInstance()->getTimeoutForRead());
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, CAHttpClient::getInstance()->getTimeoutForConnect());
    if (code != CURLE_OK) {
        return false;
    }
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);

    // FIXED #3224: The subthread of CAHttpClient interrupts main thread if timeout comes.
    // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL 
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    return true;
}

class CURLRaii
{
    /// Instance of CURL
    CURL *m_curl;
    /// Keeps custom header data
    curl_slist *m_headers;
public:
    CURLRaii()
        : m_curl(curl_easy_init())
        , m_headers(NULL)
    {
    }

    ~CURLRaii()
    {
        if (m_curl)
            curl_easy_cleanup(m_curl);
        /* free the linked list for header data */
        if (m_headers)
            curl_slist_free_all(m_headers);
    }

    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(m_curl, option, data);
    }

    /**
     * @brief Inits CURL instance for common usage
     * @param request Null not allowed
     * @param callback Response write callback
     * @param stream Response write stream
     */
    bool init(CAHttpRequest *request, write_callback callback, void *stream, write_callback headerCallback, void *headerStream)
    {
        if (!m_curl)
            return false;
        if (!configureCURL(m_curl))
            return false;

        /* get custom header data (if set) */
       	std::vector<std::string> headers=request->getHeaders();
        if(!headers.empty())
        {
            /* append custom headers one by one */
            for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
                m_headers = curl_slist_append(m_headers,it->c_str());
            /* set custom headers for curl */
            if (!setOption(CURLOPT_HTTPHEADER, m_headers))
                return false;
        }

        return setOption(CURLOPT_URL, request->getUrl())
                && setOption(CURLOPT_WRITEFUNCTION, callback)
                && setOption(CURLOPT_WRITEDATA, stream)
                && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
                && setOption(CURLOPT_HEADERDATA, headerStream);
        
    }

    /// @param responseCode Null not allowed
    bool perform(int *responseCode)
    {
        int ss = curl_easy_perform(m_curl);
        CCLog("---%d", ss);
        if (CURLE_OK != ss)
            return false;
        CURLcode code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, responseCode);
        if (code != CURLE_OK || *responseCode != 200)
            return false;
        
        // Get some mor data.
        
        return true;
    }
};

//Process Get Request
static int processGetTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = 0;
    if (curl.init(request, callback, stream, headerCallback, headerStream))
    {
        ok = 1;
        CCLog("----------------- 1");
    }
    if (curl.setOption(CURLOPT_FOLLOWLOCATION, true))
    {
        ok = 1;
        CCLog("----------------- 2");
    }
    if (curl.perform(responseCode))
    {
        ok = 1;
        CCLog("----------------- 3");
    }
//    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
//            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
//            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process POST Request
static int processPostTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_POST, 1)
            && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process PUT Request
static int processPutTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
            && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process DELETE Request
static int processDeleteTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process POST FILE Request
static int processPostFileTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *responseCode, write_callback headerCallback, void *headerStream)
{
	CURLRaii curl;
	bool ok = curl.init(request, callback, stream, headerCallback, headerStream);
	if (!ok)
		return 1;

	curl_httppost* pFormPost = NULL;
	curl_httppost* pLastElem = NULL;

	curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, "filepath", CURLFORM_FILE, 
		request->getFileNameToPost(), CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);

	std::string strReq = request->getRequestData();
    std::vector<std::string> vv = CrossApp::Parse2StrVector(strReq, "&");
	for (int i = 0; i < vv.size(); i++)
	{
		std::vector<std::string> v = CrossApp::Parse2StrVector(vv[i], "=");
		if (v.size() == 2)
		{
			curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, v[0].c_str(), CURLFORM_COPYCONTENTS, v[1].c_str(), CURLFORM_END);
		}
	}
	curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, "act", CURLFORM_COPYCONTENTS, "end", CURLFORM_END);
	
	ok = curl.setOption(CURLOPT_HTTPPOST, pFormPost)
		&& curl.perform(responseCode);

	if (pFormPost)
	{
		curl_formfree(pFormPost);
		pFormPost = NULL;
	}
	return ok ? 0 : 1;
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
{
    CAScheduler::schedule(schedule_selector(CAHttpClient::dispatchResponseCallbacks), this, 0);
    CAScheduler::getScheduler()->pauseTarget(this);
}

CAHttpClient::~CAHttpClient()
{
    need_quit[_threadID] = true;
    
    if (s_requestQueue != NULL) {
    	pthread_cond_signal(&s_SleepCondition[_threadID]);
    }
    
    s_pHttpClient[_threadID] = NULL;
}

//Lazy create semaphore & mutex & thread
bool CAHttpClient::lazyInitThreadSemphore()
{
    if (!s_requestQueue[_threadID].empty()) {
        return true;
    }
    else
    {
        pthread_mutex_init(&s_requestQueueMutex[_threadID], NULL);
        pthread_mutex_init(&s_responseQueueMutex[_threadID], NULL);
        
        pthread_mutex_init(&s_SleepMutex[_threadID], NULL);
        pthread_cond_init(&s_SleepCondition[_threadID], NULL);

        pthread_create(&s_networkThread[_threadID], NULL, networkThread, (void*)&_threadID);
        pthread_detach(s_networkThread[_threadID]);
        
        need_quit[_threadID] = false;
    }
    
    return true;
}

//Add a get task to queue
void CAHttpClient::send(CAHttpRequest* request)
{    
    lazyInitThreadSemphore();
    
    CC_RETURN_IF(!request);
        
    ++s_asyncRequestCount[_threadID];
    
    request->retain();
        
    pthread_mutex_lock(&s_requestQueueMutex[_threadID]);
    s_requestQueue[_threadID].pushBack(request);
    pthread_mutex_unlock(&s_requestQueueMutex[_threadID]);
    
    // Notify thread start to work
    pthread_cond_signal(&s_SleepCondition[_threadID]);
}

// Poll and notify main thread if responses exists in queue
void CAHttpClient::dispatchResponseCallbacks(float delta)
{
    // CCLog("CAHttpClient::dispatchResponseCallbacks is running");
    
    CAHttpResponse* response = NULL;
    
    pthread_mutex_lock(&s_responseQueueMutex[_threadID]);
    if (!s_responseQueue[_threadID].empty())
    {
        response = s_responseQueue[_threadID].front();
        s_responseQueue[_threadID].popFront();
    }
    pthread_mutex_unlock(&s_responseQueueMutex[_threadID]);
    
    if (response)
    {
        --s_asyncRequestCount[_threadID];
        
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
    
    if (0 == s_asyncRequestCount) 
    {
        CAScheduler::getScheduler()->pauseTarget(this);
    }
    
}

unsigned int CAHttpClient::getRequestCount()
{
    return (unsigned int)s_requestQueue[_threadID].size();
}

NS_CC_EXT_END

#endif // CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)



