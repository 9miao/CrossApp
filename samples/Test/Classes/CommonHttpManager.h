//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
#ifndef __HelloCpp__DMHttpManager__
#define __HelloCpp__DMHttpManager__

#include <iostream>
#include <CrossApp.h>
#include <map>



static char Dec2HexChar(short int n)
{
    if ( 0 <= n && n <= 9 )
    {
        return char( short('0') + n );
    }
    else if ( 10 <= n && n <= 15 )
    {
        return char( short('A') + n - 10 );
    }
    else
    {
        return char(0);
    }
}

static short int HexChar2Dec(char c)
{
    if ( '0'<=c && c<='9' )
    {
        return short(c-'0');
    }
    else if ( 'a'<=c && c<='f' )
    {
        return ( short(c-'a') + 10 );
    }
    else if ( 'A'<=c && c<='F' )
    {
        return ( short(c-'A') + 10 );
    }
    else
    {
        return -1;
    }
}

static string EncodeURL(const string &URL)
{
    string strResult = "";
    for ( unsigned int i=0; i<URL.size(); i++ )
    {
        char c = URL[i];
        if (
            ( '0'<=c && c<='9' ) ||
            ( 'a'<=c && c<='z' ) ||
            ( 'A'<=c && c<='Z' ) ||
            c=='/' || c=='.'
            ) {
            strResult += c;
        }
        else
        {
            int j = (short int)c;
            if ( j < 0 )
            {
                j += 256;
            }
            int i1, i0;
            i1 = j / 16;
            i0 = j - i1*16;
            strResult += '%';
            strResult += Dec2HexChar(i1);
            strResult += Dec2HexChar(i0);
        }
    }
    
    return strResult;
}

static std::string DecodeURL(const std::string &URL)
{
    string result = "";
    for ( unsigned int i=0; i<URL.size(); i++ )
    {
        char c = URL[i];
        if ( c != '%' )
        {
            result += c;
        }
        else
        {
            char c1 = URL[++i];
            char c0 = URL[++i];
            int num = 0;
            num += HexChar2Dec(c1) * 16 + HexChar2Dec(c0);
            result += char(num);
        }
    }
    
    return result;
}



typedef enum
{
    HttpResponseSucceed,
    HttpResponseFaild,
    HttpResponseTimeOut,
    HttpResponseWebDisabled
}HttpResponseStatus;

typedef enum
{
    HttpGetImageDefault,
    HttpGetImageNoMemoryCache,
    HttpGetImageNoAllCache
}HttpGetImageType;

typedef void (CAObject::*SEL_CommonHttpJson)(const HttpResponseStatus&, const CSJson::Value&);
typedef void (CAObject::*SEL_CommonHttpImage)(CAImage*, const char*);

#define CommonHttpJson_selector(_SELECTOR) (SEL_CommonHttpJson)(&_SELECTOR)
#define CommonHttpImage_selector(_SELECTOR) (SEL_CommonHttpImage)(&_SELECTOR)

class CommonHttpResponseCallBack;
class CommonHttpManager
{
    
public:
    
    static CommonHttpManager* getInstance();
    
    static void destroyInstance();
    
    void send_get(const std::string& url,
                  std::map<std::string, std::string> key_value,
                  CAObject* pTarget,
                  SEL_CommonHttpJson pSelector,
                  bool isLoading = false);
    
    void send_post(const std::string& url,
                   std::map<std::string, std::string> key_value,
                   CAObject* pTarget,
                   SEL_CommonHttpJson pSelector,
                   bool isLoading = false);
    
    void send_postFile(const std::string& url,
                       std::map<std::string, std::string> key_value,
                       const std::string& file,
                       CAObject* pTarget,
                       SEL_CommonHttpJson pSelector,
                       bool isLoading = false);
    
    void get_image(const std::string& url,
                   CAObject* pTarget,
                   SEL_CommonHttpImage pSelector,
                   const HttpGetImageType& type = HttpGetImageDefault);
    
    void starActivityIndicatorView();
    
    void stopActivityIndicatorView();
    
private:
    
    CommonHttpManager();
    
    ~CommonHttpManager();

private:
    
    std::vector<CAHttpClient*> m_pHttpJsonClients;
    
    std::vector<CAHttpClient*> m_pHttpImageClients;
    
    CAActivityIndicatorView* m_pActivityIndicatorView;
};

#pragma CommonHttpResponseCallBack

class CommonHttpResponseCallBack: public CAObject
{
    
public:
    
    typedef enum
    {
        CommonHttpResponseJsonNoCache = 0,
        CommonHttpResponseJson,
        CommonHttpResponseImage
    }
    CommonHttpResponseType;
    
    ~CommonHttpResponseCallBack();
    
    CommonHttpResponseCallBack(CAObject* pTarget, SEL_CommonHttpJson pSelector, const std::string& url, const CommonHttpResponseType& type);
    
    static CommonHttpResponseCallBack* create(CAObject* pTarget, SEL_CommonHttpJson pSelector, const std::string& url, const CommonHttpResponseType& type);
    
    CommonHttpResponseCallBack(CAObject* pTarget, SEL_CommonHttpImage pSelector, const std::string& url, const HttpGetImageType& type = HttpGetImageDefault);
    
    static CommonHttpResponseCallBack* create(CAObject* pTarget, SEL_CommonHttpImage pSelector, const std::string& url, const HttpGetImageType& type);
    
    static CommonHttpResponseCallBack* scheduleCallBack(CAObject* pTarget, SEL_CommonHttpImage pSelector, const std::string& url);
    
    static CommonHttpResponseCallBack* imagePathAsync(CAObject* pTarget, SEL_CommonHttpImage pSelector, const std::string& path, const std::string& url, const HttpGetImageType& type);
    
    void onResponse(CAHttpClient* client, CAHttpResponse* response);
    
    void onResponseJsonNoCache(CAHttpClient* client, CAHttpResponse* response);
    
    void onResponseJson(CAHttpClient* client, CAHttpResponse* response);
    
    void onResponseImage(CAHttpClient* client, CAHttpResponse* response);

protected:
    
    void update(float dt);
    
    void imagePathAsyncFinish(CAObject* var);
    
private:
    
    SEL_CommonHttpJson m_pSelectorJson;
    
    SEL_CommonHttpImage m_pSelectorImage;
    
    HttpGetImageType m_eGetImageType;
    
    CommonHttpResponseType m_eType;
    
    std::string m_sUrl;
    
    int m_nTimes;
    
    CAObject* m_pTarget;
};



#pragma CommonUrlImageView

class CommonUrlImageView;
class CommonUrlImageViewDelegate
{
public:
    
    CommonUrlImageViewDelegate();
    
    virtual ~CommonUrlImageViewDelegate();
    
    virtual void imageViewRequestFinished(CommonUrlImageView* imageView) = 0;
};

class CommonUrlImageView: public CAImageView
{
    
public:
    
    CommonUrlImageView();
    
    virtual ~CommonUrlImageView();
    
    static CommonUrlImageView* createWithImage(CAImage* image);
    
    static CommonUrlImageView* createWithFrame(const DRect& rect);
    
    static CommonUrlImageView* createWithCenter(const DRect& rect);
    
    static CommonUrlImageView* createWithLayout(const DRectLayout &layout);
    
    void setUrl(const std::string& url);

    void setImageAndUrl(CAImage* image, const std::string& url);
    
    void setUrlOnlyReadCache(const std::string& url);
    
    std::pair<std::string, CAImage*> getUrlPair();
    
    CC_PROPERTY(CommonUrlImageViewDelegate*, m_pDelegate, Delegate);
    
    CC_SYNTHESIZE_PASS_BY_REF(HttpGetImageType, m_eType, ImageType);
    
protected:

    virtual void onRequestFinished(CAImage* image, const char*);

    virtual void asyncFinish(CAObject* var);
    
    std::string m_sUrl;
};

#endif /* defined(__HelloCpp__DMHttpManager__) */
