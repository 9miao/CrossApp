

#ifndef __CCHTTPREQUEST_H__
#define __CCHTTPREQUEST_H__

#include "CrossApp.h"
#include "ExtensionMacros.h"

#include "HttpRequest.h"
#include "HttpResponse.h"

NS_CC_EXT_BEGIN

/**
 * @addtogroup Network
 * @{
 */


/** @brief Singleton that handles asynchrounous http requests
 * Once the request completed, a callback will issued in main thread when it provided during make request
 * @js NA
 * @lua NA
 */
class CCHttpClient : public CAObject
{
public:
    /** Return the shared instance **/
    static CCHttpClient *getInstance(int thread = 0);
    
    /** Relase the shared instance **/
    static void destroyInstance(int thread = 0);
        
    /**
     * Add a get request to task queue
     * @param request a CCHttpRequest object, which includes url, response callback etc.
                      please make sure request->_requestData is clear before calling "send" here.
     * @return NULL
     */
    void send(CCHttpRequest* request);
  
    
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
    
    
    int getRequestCount();
    
private:
    CCHttpClient(int thread);
    virtual ~CCHttpClient();
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

NS_CC_EXT_END

#endif //__CCHTTPREQUEST_H__
