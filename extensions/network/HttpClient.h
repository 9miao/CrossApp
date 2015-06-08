

#ifndef __CAHttpRequest_H__
#define __CAHttpRequest_H__

#include "CrossApp.h"
#include "ExtensionMacros.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <queue>
#include <pthread.h>
#include <errno.h>


NS_CC_EXT_BEGIN

class CAHttpClient : public CAObject
{
public:

    static CAHttpClient *getInstance(int thread = 0);
    
    static void destroyInstance(int thread = 0);

    void send(CAHttpRequest* request);

    inline void setTimeoutForConnect(int value) {_timeoutForConnect = value;};

    inline int getTimeoutForConnect() {return _timeoutForConnect;}

    inline void setTimeoutForRead(int value) {_timeoutForRead = value;};

    inline int getTimeoutForRead() {return _timeoutForRead;};
    
    void setSSLVerification(const std::string& str) {_sslCaFilename = str;}
    
    unsigned long getRequestCount() {return _asyncRequestCount;}
    
private:
    
    CAHttpClient(int thread);
    
    virtual ~CAHttpClient();
    
    bool init(void);
    
    bool lazyInitThreadSemphore();
    
    void dispatchResponseCallbacks(float delta);
    
private:
    int _timeoutForConnect;
    int _timeoutForRead;
    int _threadID;

public:
    unsigned long               _asyncRequestCount;
    std::string                 _sslCaFilename;
    pthread_t                   s_networkThread;
    pthread_mutex_t             s_requestQueueMutex;
    pthread_mutex_t             s_responseQueueMutex;
    pthread_mutex_t             s_SleepMutex;
    pthread_cond_t              s_SleepCondition;
    bool                        need_quit;
    char                        s_errorBuffer[256];
    CADeque<CAHttpRequest*>     s_requestQueue;
    CADeque<CAHttpResponse*>    s_responseQueue;

};


CC_DEPRECATED_ATTRIBUTE typedef CAHttpClient CCHttpClient;
CC_DEPRECATED_ATTRIBUTE typedef CAHttpResponse CCHttpResponse;
CC_DEPRECATED_ATTRIBUTE typedef CAHttpRequest CCHttpRequest;
NS_CC_EXT_END

#endif //__CAHttpRequest_H__
