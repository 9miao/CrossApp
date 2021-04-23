#ifndef __HelloCpp__DMHttpManager__
#define __HelloCpp__DMHttpManager__

#include <iostream>
#include <CrossApp.h>
#include <map>

enum class ImageCacheType
{
    Default,
    NoMemoryCache,
    NoAllCache
};

class CommonHttpManager
{
    
public:
    
    enum class ResponseStatus
    {
        Succeed,
        Faild,
        TimeOut,
        WebDisabled
    };

    typedef std::function<void(const ResponseStatus&, const CSJson::Value&)> JsonCallback;
    
    typedef std::function<void(CAImage*, const std::string&)> ImageCallback;
    
    
    static CommonHttpManager* getInstance();
    
    static void destroyInstance();
    
    void send_get(const std::string& url,
                  std::map<std::string, std::string> key_value,
                  const JsonCallback& callback,
                  CAView* view);
    
    void send_post(const std::string& url,
                   std::map<std::string, std::string> key_value,
                   const JsonCallback& callback,
                   CAView* view);
    
    void send_postFile(const std::string& url,
                       std::map<std::string, std::string> key_value,
                       const std::string& file,
                       const JsonCallback& callback,
                       CAView* view);
    
    void get_image(const std::string& url,
                   const ImageCallback& callback,
                   ImageCacheType type = ImageCacheType::Default);
    
    void starActivityIndicatorView(CAView* view);
    
    void stopActivityIndicatorView();
    
private:
    
    CommonHttpManager();
    
    ~CommonHttpManager();

private:

    std::vector<CAHttpClient*> m_pHttpJsonClients;
    
    std::vector<CAHttpClient*> m_pHttpImageClients;
    
    CAActivityIndicatorView* m_pActivityIndicatorView;

};



#pragma CommonUrlImageView
class CommonUrlImageView: public CAImageView
{
    
public:
    
    CommonUrlImageView();
    
    virtual ~CommonUrlImageView();
    
    static CommonUrlImageView* createWithImage(CAImage* image);
    
    static CommonUrlImageView* createWithFrame(const DRect& rect);
    
    static CommonUrlImageView* createWithCenter(const DRect& rect);
    
    static CommonUrlImageView* createWithLayout(const DLayout &layout);
    
    void setUrl(const std::string& url);

    void setImageAndUrl(CAImage* image, const std::string& url);
    
    void setUrlOnlyReadCache(const std::string& url);
    
    virtual void setImageAsyncWithFile(const std::string& path);
    
    std::pair<std::string, CAImage*> getUrlPair();
        
    CC_SYNTHESIZE_PASS_BY_REF(ImageCacheType, m_eType, ImageType);
    
    void onFinishedCallback(const std::function<void()>& callback){ m_callback = callback; }

protected:

    void onRequestFinished(CAImage* image, const std::string& url);
    
    std::function<void()> m_callback;
    
    std::string m_sUrl;
};

#endif /* defined(__HelloCpp__DMHttpManager__) */
