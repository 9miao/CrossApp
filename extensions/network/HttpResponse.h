

#ifndef __HTTP_RESPONSE__
#define __HTTP_RESPONSE__

#include "CrossApp.h"
#include "ExtensionMacros.h"
#include "HttpRequest.h"

NS_CC_EXT_BEGIN

class CAHttpResponse : public CAObject
{
public:
    /** Constructor, it's used by CCHttpClient internal, users don't need to create HttpResponse manually
     @param request the corresponding HttpRequest which leads to this response 
     */
    CAHttpResponse(CAHttpRequest* request)
    {
        _pHttpRequest = request;
        if (_pHttpRequest)
        {
            _pHttpRequest->retain();
        }
        
        _succeed = false;
        _responseData.clear();
        _errorBuffer.clear();
    }
    
    /** Destructor, it will be called in CCHttpClient internal,
     users don't need to desturct HttpResponse object manully 
     */
    virtual ~CAHttpResponse()
    {
        if (_pHttpRequest)
        {
            _pHttpRequest->release();
        }
    }
    
    /** Override autorelease method to prevent developers from calling it */
    CAObject* autorelease(void)
    {
        CCAssert(false, "HttpResponse is used between network thread and ui thread \
                        therefore, autorelease is forbidden here");
        return NULL;
    }
    
    // getters, will be called by users
    
    /** Get the corresponding HttpRequest object which leads to this response 
        There's no paired setter for it, coz it's already setted in class constructor
     */
    inline CAHttpRequest* getHttpRequest()
    {
        return _pHttpRequest;
    }
        
    /** To see if the http reqeust is returned successfully,
        Althrough users can judge if (http return code = 200), we want an easier way
        If this getter returns false, you can call getResponseCode and getErrorBuffer to find more details
     */
    inline bool isSucceed()
    {
        return _succeed;
    };
    
    /** Get the http response raw data */
    inline std::vector<char>* getResponseData()
    {
        return &_responseData;
    }
    
    /** get the Rawheader **/
    inline std::vector<char>* getResponseHeader()
    {
        return &_responseHeader;
    }

    /** Get the http response errorCode
     *  I know that you want to see http 200 :)
     */
    inline int getResponseCode()
    {
        return (int)_responseCode;
    }

    /** Get the rror buffer which will tell you more about the reason why http request failed
     */
    inline const char* getErrorBuffer()
    {
        return _errorBuffer.c_str();
    }
    
    // setters, will be called by CCHttpClient
    // users should avoid invoking these methods
    
    
    /** Set if the http request is returned successfully,
     Althrough users can judge if (http code == 200), we want a easier way
     This setter is mainly used in CCHttpClient, users mustn't set it directly
     */
    inline void setSucceed(bool value)
    {
        _succeed = value;
    };
    
    
    /** Set the http response raw buffer, is used by CCHttpClient
     */
    inline void setResponseData(std::vector<char>* data)
    {
        _responseData = *data;
    }
    
    /** Set the http response Header raw buffer, is used by CCHttpClient
     */
    inline void setResponseHeader(std::vector<char>* data)
    {
        _responseHeader = *data;
    }
    
    
    /** Set the http response errorCode
     */
    inline void setResponseCode(long value)
    {
        _responseCode = value;
    }
    
    
    /** Set the error buffer which will tell you more the reason why http request failed
     */
    inline void setErrorBuffer(const char* value)
    {
        _errorBuffer.clear();
        _errorBuffer.assign(value);
    };
    
    inline CAHttpRequest* getRequest()
    {
        return _pHttpRequest;
    }
    
protected:
    bool initWithRequest(CAHttpRequest* request);
    
    // properties
    CAHttpRequest*      _pHttpRequest;  /// the corresponding HttpRequest pointer who leads to this response
    bool                _succeed;       /// to indecate if the http reqeust is successful simply
    std::vector<char>   _responseData;  /// the returned raw data. You can also dump it as a string
    std::vector<char>   _responseHeader;  /// the returned raw header data. You can also dump it as a string
    long                _responseCode;    /// the status code returned from libcurl, e.g. 200, 404
    std::string         _errorBuffer;   /// if _responseCode != 200, please read _errorBuffer to find the reason 
    
};

NS_CC_EXT_END

#endif //__HTTP_RESPONSE_H__
