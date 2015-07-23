

#include "HttpClient.h"
#include "curl/curl.h"
#define MAX_Thread 16

#ifndef usleep
#include "libwebsockets.h"
#endif

NS_CC_EXT_BEGIN

static int s_httpClientCount = 0;
static CAHttpClient *s_pHttpClient[MAX_Thread] = {0};

typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);

static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}

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
//static int processDownloadTask(HttpRequest *task, write_callback callback, void *stream, int32_t *errorCode);


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
        
        request = NULL;
        
        pthread_mutex_lock(&httpClient->s_requestQueueMutex);
        if (!httpClient->s_requestQueue.empty())
        {
            request = httpClient->s_requestQueue.front();
            httpClient->s_requestQueue.popFront();
        }
        pthread_mutex_unlock(&httpClient->s_requestQueueMutex);
        
        if (NULL == request)
        {
        	pthread_cond_wait(&httpClient->s_SleepCondition, &httpClient->s_SleepMutex);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
			Sleep(100);
#else
			usleep(100000);
#endif
            continue;
        }
        
       
        CAHttpResponse *response = new CAHttpResponse(request);
         request->release();
        
        int32_t responseCode = -1;
        int retValue = 0;

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

        
        pthread_mutex_lock(&httpClient->s_requestQueueMutex);
        httpClient->s_responseQueue.pushBack(response);
        pthread_mutex_unlock(&httpClient->s_requestQueueMutex);
        
        CAScheduler::getScheduler()->resumeTarget(httpClient);
    }
    
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

static bool configureCURL(CURL *handle, CAHttpClient* httpClient)
{
    if (!handle) {
        return false;
    }
    
    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, httpClient->s_errorBuffer);
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
    if (httpClient->_sslCaFilename.empty())
    {
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    else
    {
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(handle, CURLOPT_CAINFO, httpClient->_sslCaFilename.c_str());
    }
    
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    return true;
}

class CURLRaii
{
    CURL *m_curl;
    
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

        if (m_headers)
            curl_slist_free_all(m_headers);
    }

    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(m_curl, option, data);
    }


    bool init(CAHttpRequest *request, write_callback callback, void *stream, write_callback headerCallback, void *headerStream)
    {
        if (!m_curl)
            return false;
        if (!configureCURL(m_curl, s_pHttpClient[request->getThreadID()]))
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

        return setOption(CURLOPT_URL, request->getUrl().c_str())
                && setOption(CURLOPT_WRITEFUNCTION, callback)
                && setOption(CURLOPT_WRITEDATA, stream)
                && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
                && setOption(CURLOPT_HEADERDATA, headerStream);
        
    }

    bool perform(int *responseCode)
    {
        if (CURLE_OK != curl_easy_perform(m_curl))
            return false;
        CURLcode code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, responseCode);
        if (code != CURLE_OK || *responseCode != 200)
            return false;
        
        return true;
    }
};

static int processGetTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

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

static int processDeleteTask(CAHttpRequest *request, write_callback callback, void *stream, int32_t *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

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

	int requestDataSize = request->getRequestDataSize();
	if (requestDataSize>0)
	{
		std::string strReq = request->getRequestData();
        strReq.resize(request->getRequestDataSize());
		std::vector<std::string> vv = CrossApp::Parse2StrVector(strReq, "&");
		for (int i = 0; i < vv.size(); i++)
		{
			std::vector<std::string> v = CrossApp::Parse2StrVector(vv[i], "=", true);
			if (v.size() == 2)
			{
				curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, v[0].c_str(), CURLFORM_COPYCONTENTS, v[1].c_str(), CURLFORM_END);
			}
		}
		curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, "act", CURLFORM_COPYCONTENTS, "end", CURLFORM_END);
	}
	
	ok = curl.setOption(CURLOPT_HTTPPOST, pFormPost)
		&& curl.perform(responseCode);

	if (pFormPost)
	{
		curl_formfree(pFormPost);
		pFormPost = NULL;
	}
	return ok ? 0 : 1;
}


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
, _asyncRequestCount(0)
, need_quit(0)
, s_networkThread(pthread_t())
, s_requestQueueMutex(pthread_mutex_t())
, s_responseQueueMutex(pthread_mutex_t())
, s_SleepMutex(pthread_mutex_t())
, s_SleepCondition(pthread_cond_t())
{
    CAScheduler::schedule(schedule_selector(CAHttpClient::dispatchResponseCallbacks), this, 0);
    CAScheduler::getScheduler()->pauseTarget(this);
    lazyInitThreadSemphore();

    ++s_httpClientCount;
}

CAHttpClient::~CAHttpClient()
{
    need_quit = true;
    
    if (!s_requestQueue.empty())
    {
        pthread_cond_signal(&s_SleepCondition);
    }
    
    s_pHttpClient[_threadID] = NULL;
    
    --s_httpClientCount;
}

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

void CAHttpClient::send(CAHttpRequest* request)
{
    CC_RETURN_IF(!request);
        
    ++_asyncRequestCount;
    
    request->setThreadID(_threadID);
    request->retain();
        
    pthread_mutex_lock(&s_requestQueueMutex);
    s_requestQueue.pushBack(request);
    pthread_mutex_unlock(&s_requestQueueMutex);
    
    pthread_cond_signal(&s_SleepCondition);
}

// Poll and notify main thread if responses exists in queue
void CAHttpClient::dispatchResponseCallbacks(float delta)
{
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


