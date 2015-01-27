

#ifndef __CAHttpRequest_H__
#define __CAHttpRequest_H__

#include "CrossApp.h"
#include "ExtensionMacros.h"

#include "HttpRequest.h"
#include "HttpResponse.h"

NS_CC_EXT_BEGIN


class CAHttpClient : public CAObject
{
public:
    /** Return the shared instance **/
    static CAHttpClient *getInstance(int thread = 0);
    
    /** Relase the shared instance **/
    static void destroyInstance(int thread = 0);
        
    /**
     * Add a get request to task queue
     * @param request a CAHttpRequest object, which includes url, response callback etc.
                      please make sure request->_requestData is clear before calling "send" here.
     * @return NULL
     */
    void send(CAHttpRequest* request);
  
    
    /**
     * Change the connect timeout
     * @param timeout 
     * @return NULL
     */
    inline void setTimeoutForConnect(int value) {_timeoutForConnect = value;};
    
    /**
     * Get connect timeout
     * @return int
     *
     */
    inline int getTimeoutForConnect() {return _timeoutForConnect;}
    
    
    /**
     * Change the download timeout
     * @param value
     * @return NULL
     */
    inline void setTimeoutForRead(int value) {_timeoutForRead = value;};
    

    /**
     * Get download timeout
     * @return int
     */
    inline int getTimeoutForRead() {return _timeoutForRead;};
    
    
    unsigned int getRequestCount();
    
private:
    CAHttpClient(int thread);
    virtual ~CAHttpClient();
    bool init(void);
    
    /**
     * Init pthread mutex, semaphore, and create new thread for http requests
     * @return bool
     */
    bool lazyInitThreadSemphore();
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks(float delta);
    
private:
    int _timeoutForConnect;
    int _timeoutForRead;
    int _threadID;
    // std::string reqId;
};

// end of Network group
/// @}

CC_DEPRECATED_ATTRIBUTE typedef CAHttpClient CCHttpClient;
CC_DEPRECATED_ATTRIBUTE typedef CAHttpResponse CCHttpResponse;
CC_DEPRECATED_ATTRIBUTE typedef CAHttpRequest CCHttpRequest;
NS_CC_EXT_END

#endif //__CAHttpRequest_H__
