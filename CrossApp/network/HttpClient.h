

#ifndef __CAHttpClient_H__
#define __CAHttpClient_H__

#include <thread>
#include <condition_variable>
#include "basics/CASTLContainer.h"
#include "basics/CAScheduler.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpCookie.h"

NS_CC_BEGIN

class CC_DLL CAHttpClient: public CAObject
{
public:
    /**
     * The buffer size of _responseMessage
     */
    static const int RESPONSE_BUFFER_SIZE = 256;
    
    /**
     * Get instance of CAHttpClient.
     *
     * @return the instance of CAHttpClient.
     */
    static CAHttpClient *getInstance(ssize_t thread_id);
    
    /**
     * Release the instance of CAHttpClient.
     */
    static void destroyInstance(ssize_t thread_id);
    
    /**
     * Release the instance of All CAHttpClient.
     */
    static void destroyAllInstance();
    
    /**
     * Enable cookie support.
     *
     * @param cookieFile the filepath of cookie file.
     */
    void enableCookies(const char* cookieFile);
    
    /**
     * Get the cookie filename
     *
     * @return the cookie filename
     */
    const std::string& getCookieFilename();
    
    /**
     * Set root certificate path for SSL verification.
     *
     * @param caFile a full path of root certificate.if it is empty, SSL verification is disabled.
     */
    void setSSLVerification(const std::string& caFile);
    
    /**
     * Get the ssl CA filename
     *
     * @return the ssl CA filename
     */
    const std::string& getSSLVerification();
    
    /**
     * Add a get request to task queue
     *
     * @param request a CAHttpRequest object, which includes url, response callback etc.
     please make sure request->_requestData is clear before calling "send" here.
     */
    void send(CAHttpRequest* request);
    
    /**
     * Immediate send a request
     *
     * @param request a CAHttpRequest object, which includes url, response callback etc.
     please make sure request->_requestData is clear before calling "sendImmediate" here.
     */
    void sendImmediate(CAHttpRequest* request);
    
    /**
     * Set the timeout value for connecting.
     *
     * @param value the timeout value for connecting.
     */
    void setTimeoutForConnect(int value);
    
    /**
     * Get the timeout value for connecting.
     *
     * @return int the timeout value for connecting.
     */
    int getTimeoutForConnect();
    
    /**
     * Set the timeout value for reading.
     *
     * @param value the timeout value for reading.
     */
    void setTimeoutForRead(int value);
    
    /**
     * Get the timeout value for reading.
     *
     * @return int the timeout value for reading.
     */
    int getTimeoutForRead();
    
    CAHttpCookie* getCookie() const {return _cookie; }
    
    size_t getRequestCount() {return _requestQueue.size();}
    
//    std::mutex& getCookieFileMutex() {return _cookieFileMutex;}
//    
//    std::mutex& getSSLCaFileMutex() {return _sslCaFileMutex;}
    static bool uploadFile(const std::string &url,
                            const std::map<std::string, std::string> parameters,
                            const std::string &upload_file,
                            const std::string &file_part_name,
                            const std::string &proxy,
                            const std::string &proxy_user_pwd,
                            const std::string &ca_certificate_file,
                            std::string *response_body,
                            long *response_code,
                            std::string *error_description);

private:
    CAHttpClient(ssize_t thread_id);
    virtual ~CAHttpClient();
    bool init();
    
    // Checks that the given list of parameters has only printable
    // ASCII characters in the parameter name, and does not contain
    // any quote (") characters.  Returns true if so.
    static bool CheckParameters(const std::map<std::string, std::string> parameters);
    
    // No instances of this class should be created.
    // Disallow all constructors, destructors, and operator=.
    explicit CAHttpClient(const CAHttpClient &);
    void operator=(const CAHttpClient &);

    /**
     * Init pthread mutex, semaphore, and create new thread for http requests
     * @return bool
     */
    bool lazyInitThreadSemphore();
    void networkThread();
    void networkThreadAlone(CAHttpRequest* request, CAHttpResponse* response);
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks();
    
    void processResponse(CAHttpResponse* response, char* responseMessage);
    void increaseThreadCount();
    void decreaseThreadCountAndMayDeleteThis();
    
private:
    ssize_t _thread_id;
    
    bool _isInited;
    
    int _timeoutForConnect;
    std::mutex _timeoutForConnectMutex;
    
    int _timeoutForRead;
    std::mutex _timeoutForReadMutex;
    
    int  _threadCount;
    std::mutex _threadCountMutex;
    
    CAScheduler* _scheduler;
    std::mutex _schedulerMutex;
    
    CAVector<CAHttpRequest*>  _requestQueue;
    std::mutex _requestQueueMutex;
    
    CAVector<CAHttpResponse*> _responseQueue;
    std::mutex _responseQueueMutex;
    
    std::string _cookieFilename;
    std::mutex _cookieFileMutex;
    
    std::string _sslCaFilename;
    std::mutex _sslCaFileMutex;
    
    CAHttpCookie* _cookie;
    
    std::condition_variable_any _sleepCondition;
    
    char _responseMessage[RESPONSE_BUFFER_SIZE];
    
    CAHttpRequest* _requestSentinel;
};

NS_CC_END

// end group
/// @}

#endif //__CAHttpClient_H__

