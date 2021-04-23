 
#include "CommonHttpManager.h"

#define REQUEST_JSON_COUNT 2
#define REQUEST_IMAGE_COUNT 2

static const char* loading_bg              = "";
static const char* loading_icon            = "";

static std::map<std::string, CAObject*> _httpResponses;

static bool compareHttpClient(CAHttpClient* itr1, CAHttpClient* itr2)
{
    return (itr1)->getRequestCount() < (itr2)->getRequestCount();
}


#pragma CommonImageCacheManager

class CommonImageCacheManager: public CAView
{
public:
    
    static CommonImageCacheManager* getInstance();
    
    static void destroyInstance();
    
    void pushImage(CAImage* image);
    
    void removeImage(CAImage* image);
    
private:
    
    CADeque<CAImage*> m_dImageQueue;
};

static CommonImageCacheManager* _imageCacheManager = nullptr;

CommonImageCacheManager* CommonImageCacheManager::getInstance()
{
    if (_imageCacheManager == NULL)
    {
        _imageCacheManager = new CommonImageCacheManager();
    }
    return _imageCacheManager;
}

void CommonImageCacheManager::destroyInstance()
{
    if (_imageCacheManager)
    {
        delete _imageCacheManager;
        _imageCacheManager = NULL;
    }
}


void CommonImageCacheManager::pushImage(CAImage* image)
{
    CC_RETURN_IF(m_dImageQueue.contains(image));
    m_dImageQueue.pushBack(image);
    
    CAViewAnimation::beginAnimations("");
    CAViewAnimation::setAnimationDuration(10);
    CAViewAnimation::setAnimationDidStopSelector([&]
    {
        CAApplication::getApplication()->getImageCache()->removeImage(m_dImageQueue.front());
        m_dImageQueue.popFront();
    });
    CAViewAnimation::commitAnimations();
}

void CommonImageCacheManager::removeImage(CAImage* image)
{
    m_dImageQueue.eraseObject(image);
}

#pragma CommonHttpManager

CommonHttpManager* _HttpManager = nullptr;

CommonHttpManager* CommonHttpManager::getInstance()
{
    if (_HttpManager == nullptr)
    {
        _HttpManager = new CommonHttpManager();
    }
    return _HttpManager;
}

void CommonHttpManager::destroyInstance()
{
    if (_HttpManager)
    {
        delete _HttpManager;
        _HttpManager = nullptr;
    }
}

CommonHttpManager::CommonHttpManager()
: m_pActivityIndicatorView(nullptr)
{
    for (int i=0; i<REQUEST_JSON_COUNT; i++)
    {
        CAHttpClient* httpClient = CAHttpClient::getInstance(15 - i);
        httpClient->setTimeoutForConnect(5);
        httpClient->setTimeoutForRead(5);
        m_pHttpJsonClients.push_back(httpClient);
    }
    for (int i=REQUEST_JSON_COUNT; i<REQUEST_JSON_COUNT + REQUEST_IMAGE_COUNT; i++)
    {
        CAHttpClient* httpClient = CAHttpClient::getInstance(15 - i);
        httpClient->setTimeoutForConnect(5);
        httpClient->setTimeoutForRead(5);
        m_pHttpImageClients.push_back(httpClient);
    }
    

    std::string fullPath = FileUtils::getInstance()->getWritablePath() + "commonHttpManager.db";
    localStorageInit(fullPath.c_str());
    CommonImageCacheManager::getInstance();
}



CommonHttpManager::~CommonHttpManager()
{
    for (int i=0; i<REQUEST_JSON_COUNT + REQUEST_IMAGE_COUNT; i++)
    {
        CAHttpClient::destroyInstance(15 - i);
    }
    m_pHttpJsonClients.clear();
    m_pHttpImageClients.clear();
}


void CommonHttpManager::send_get(const std::string& url,std::map<std::string,
                                 std::string> key_value,
                                 const JsonCallback& callback,
                                 CAView* view)
{
    std::string getRul = url;
    if (!key_value.empty())
    {
        getRul += "?";
        
        std::map<std::string, std::string>::iterator itr = key_value.begin();
        
        do
        {
            getRul = getRul + itr->first + "=" + itr->second;
            itr++;
            CC_BREAK_IF(itr==key_value.end());
            getRul = getRul + "&";
        }
        while (1);
    }
    CCLog("url==%s",getRul.c_str());
    CAHttpRequest* httpRequest = new CAHttpRequest();
    httpRequest->setUrl(getRul.c_str());
    httpRequest->setRequestType(CAHttpRequest::Type::Get);
    
    httpRequest->setResponseCallback([=](CAHttpClient* client, CAHttpResponse* response)
    {
        if (response->isSucceed())
        {
            std::string data(response->getResponseData()->toString());
            if (!data.empty())
            {
                localStorageSetItem(MD5(url + "AAA").md5().c_str(), data.c_str());
                
                CCLog("\n \n \n---------HttpResponse--json---------\n<<<\n%s\n>>>\n--------------END--------------\n \n \n",data.c_str());
                
                CSJson::Reader read;
                CSJson::Value root;
                bool succ = read.parse(data, root);
                if (succ == false)
                {
                    CCLog("GetParseError \n");
                }
                
                if (callback)
                {
                    callback(CommonHttpManager::ResponseStatus::Succeed, root);
                }
                
            }
            else
            {
                if (callback)
                {
                    callback(CommonHttpManager::ResponseStatus::Faild, CSJson::Value());
                }
            }
            
        }
        else 
        {
            const char* data = localStorageGetItem(MD5(url + "AAA").md5().c_str());
            
            do
            {
                CC_BREAK_IF(data == NULL);
                CSJson::Reader read;
                CSJson::Value root;
                bool succ = read.parse(data, root);
                if (succ == false)
                {
                    CCLog("GetParseError \n");
                    break;
                }
                
                if (callback)
                {
                    callback(CommonHttpManager::ResponseStatus::Succeed, root);
                }
                return;
            }
            while (0);
            
            if (callback)
            {
                callback(CommonHttpManager::ResponseStatus::Faild, CSJson::Value());
            }
        }

    });
    
    std::sort(m_pHttpJsonClients.begin(), m_pHttpJsonClients.end(), compareHttpClient);
    m_pHttpJsonClients.front()->send(httpRequest);
    httpRequest->release();
    
    if (view)
    {
        this->starActivityIndicatorView(view);
    }
}



void CommonHttpManager::send_post(const std::string& url,
                                  std::map<std::string,std::string> key_value,
                                  const JsonCallback& callback,
                                  CAView* view)
{
    std::string postData;
    if (!key_value.empty())
    {
        std::map<std::string, std::string>::iterator itr = key_value.begin();
        
        do
        {
            postData = postData + itr->first + "=" + itr->second;
            itr++;
            CC_BREAK_IF(itr==key_value.end());
            postData = postData + "&";
        }
        while (1);
    }
    CCLog("send_post url:%s postData:%s",url.c_str(), postData.c_str());

    CAHttpRequest* httpRequest = new CAHttpRequest();
    httpRequest->setUrl(url.c_str());
    httpRequest->setRequestType(CAHttpRequest::Type::Post);
    httpRequest->setRequestData(postData.c_str(), (unsigned int)postData.length());
    
    httpRequest->setResponseCallback([=](CAHttpClient* client, CAHttpResponse* response)
    {
        if (response->isSucceed())
        {
            std::string data(response->getResponseData()->toString());
            if (!data.empty())
            {
                CCLog("\n \n \n---------HttpResponse--json---------\n<<<\n%s\n>>>\n--------------END--------------\n \n \n", data.c_str());
                
                CSJson::Reader read;
                CSJson::Value root;
                bool succ = read.parse(data, root);
                if (succ == false)
                {
                    CCLog("GetParseError \n");
                }
                
                if (callback)
                {
                    callback(CommonHttpManager::ResponseStatus::Succeed, root);
                }
            }
            else
            {
                if (callback)
                {
                    callback(CommonHttpManager::ResponseStatus::Faild, CSJson::Value());
                }
            }
            
        }
        else
        {
            CCLog("ResponseCode:%ld, Error:%s",response->getResponseCode(),response->getErrorBuffer());
            if (callback)
            {
                callback(CommonHttpManager::ResponseStatus::Faild, CSJson::Value());
            }
        }
    });
    
    std::sort(m_pHttpJsonClients.begin(), m_pHttpJsonClients.end(), compareHttpClient);
    m_pHttpJsonClients.front()->send(httpRequest);
    httpRequest->release();
    
    if (view)
    {
        this->starActivityIndicatorView(view);
    }
}

void CommonHttpManager::send_postFile(const std::string& url,
                                      std::map<std::string, std::string> key_value,
                                      const std::string& file,
                                      const JsonCallback& callback,
                                      CAView* view)
{
    std::string postData;
    if (!key_value.empty())
    {
        std::map<std::string, std::string>::iterator itr = key_value.begin();
        
        do
        {
            postData = postData + itr->first + "=" + itr->second;
            itr++;
            CC_BREAK_IF(itr==key_value.end());
            postData = postData + "&";
        }
        while (1);
    }
    CCLog("url==%s",postData.c_str());
    CAHttpRequest* httpRequest = new CAHttpRequest();
    httpRequest->setUrl(url.c_str());
    httpRequest->setRequestType(CAHttpRequest::Type::PostFile);
    httpRequest->setRequestData(postData.c_str(), (unsigned int)postData.length());
    httpRequest->setFileNameToPost(file);

    httpRequest->setResponseCallback([=](CAHttpClient* client, CAHttpResponse* response)
    {
        if (response->isSucceed())
        {
            std::string data(response->getResponseData()->toString());
            if (!data.empty())
            {
                CCLog("\n \n \n---------HttpResponse--json---------\n<<<\n%s\n>>>\n--------------END--------------\n \n \n", data.c_str());
                
                CSJson::Reader read;
                CSJson::Value root;
                bool succ = read.parse(data, root);
                if (succ == false)
                {
                    CCLog("GetParseError \n");
                }
                
                if (callback)
                {
                    callback(CommonHttpManager::ResponseStatus::Succeed, root);
                }
            }
            else
            {
                if (callback)
                {
                    callback(CommonHttpManager::ResponseStatus::Faild, CSJson::Value());
                }
            }
            
        }
        else
        {
            CCLog("ResponseCode:%ld, Error:%s",response->getResponseCode(),response->getErrorBuffer());
            if (callback)
            {
                callback(CommonHttpManager::ResponseStatus::Faild, CSJson::Value());
            }
        }
    });
    
    std::sort(m_pHttpJsonClients.begin(), m_pHttpJsonClients.end(), compareHttpClient);
    m_pHttpJsonClients.front()->send(httpRequest);
    httpRequest->release();
    
    if (view)
    {
        this->starActivityIndicatorView(view);
    }
    
}

void CommonHttpManager::get_image(const std::string& url,
                                  const ImageCallback& callback,
                                  ImageCacheType type)
{
    std::string key = MD5(url + "AAA").md5();
    
    CAImage* image = CAApplication::getApplication()->getImageCache()->imageForKey(key);

    if (image)
    {
        if (callback)
        {
            callback(image, url);
        }
    }
    else if (_httpResponses.count(url) > 0)
    {
        std::string animationID = crossapp_format_string("wait:%s", url.c_str());
        CACustomAnimation::schedule([=](const CACustomAnimation::Model& model)
        {
            std::string key = MD5(url + "AAA").md5();
            
            CAImage* image = CAApplication::getApplication()->getImageCache()->imageForKey(key);
            
            if (image == nullptr)
            {
                key = FileUtils::getInstance()->getWritablePath() + "image/" + key;
                image = CAApplication::getApplication()->getImageCache()->imageForKey(key);
            }
            
            if (image)
            {
                if (callback)
                {
                    callback(image, url);
                }
                CACustomAnimation::unschedule(animationID);
            }
        }, animationID, 1.0f);
    }
    else
    {
        std::string imagePath = FileUtils::getInstance()->getWritablePath() + "image/" + key;
        
        unsigned long pSize = 0;

        FILE* fp = fopen(imagePath.c_str(), "rb");
        if (fp)
        {
            fseek(fp, 0L, SEEK_END);
            pSize = ftell(fp);
            fseek(fp,0,SEEK_SET);
            fclose(fp);
        }
        
        if (pSize > 1)
        {
            CAApplication::getApplication()->getImageCache()->addImageFullPathAsync(imagePath, [=](CAImage* image)
            {
                if (callback)
                {
                    callback(image, url);
                }
            });
        }
        else
        {
            CAHttpRequest* httpRequest = new CAHttpRequest();
            httpRequest->setUrl(url.c_str());
            httpRequest->setRequestType(CAHttpRequest::Type::Get);
            std::vector<std::string> header;
            //header.push_back("Referer:");
            httpRequest->setHeaders(header);
            
            httpRequest->setResponseCallback([=](CAHttpClient* client, CAHttpResponse* response)
            {
                if (response->isSucceed())
                {
                    std::string data(response->getResponseData()->toString());
                    unsigned char* pData = ((unsigned char*)(const_cast<char*>(data.c_str())));
                    size_t pSize = data.length();
                    
                    std::string key = MD5(url + "AAA").md5();
                    CAImage* image = nullptr;
                    
                    if (type != ImageCacheType::NoAllCache)
                    {
                        std::string imagePath = FileUtils::getInstance()->getWritablePath() + "image/";
                        FileUtils::getInstance()->createDirectory(imagePath.c_str());
                        imagePath = imagePath + key;
                        FILE *fp = fopen(string(imagePath).c_str(), "wb+");
                        if (fp)
                        {
                            fwrite(pData, sizeof(unsigned char), pSize, fp);
                            fclose(fp);
                        }
                        
                        CAApplication::getApplication()->getImageCache()->addImageFullPathAsync(imagePath, [=](CAImage* image)
                        {
                            if (callback)
                            {
                                callback(image, url);
                            }
                        });
                    }
                    else
                    {
                        CAData* data = CAData::create();
                        data->copy(pData, pSize);
                        image = CAImage::createWithImageData(data, key);
                        
                        if (callback)
                        {
                            callback(image, url);
                        }
                    }
                    
                    if (type != ImageCacheType::Default)
                    {
                        CommonImageCacheManager::getInstance()->pushImage(image);
                    }
                }

            });
            
            std::sort(m_pHttpImageClients.begin(), m_pHttpImageClients.end(), compareHttpClient);
            m_pHttpImageClients.front()->send(httpRequest);
            httpRequest->release();
        }
    }
}

void CommonHttpManager::starActivityIndicatorView(CAView* view)
{
    if (m_pActivityIndicatorView == nullptr)
    {
        m_pActivityIndicatorView = CAActivityIndicatorView::createWithLayout(DLayoutFill);
        CC_SAFE_RETAIN(m_pActivityIndicatorView);
        m_pActivityIndicatorView->setStyle(CAActivityIndicatorView::Style::Image);
        
        CAImageView* bg = CAImageView::createWithLayout(DLayout(DHorizontalLayout_W_C(270, 0.5), DVerticalLayout_H_C(151, 0.5)));
        bg->setImage(CAImage::create(loading_bg));
        m_pActivityIndicatorView->setActivityBackView(bg);
        
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create(loading_icon));
        imageView->setCenter(DRect(0, 0, 64, 64));
        m_pActivityIndicatorView->setActivityIndicatorView(imageView);
        m_pActivityIndicatorView->setActivityIndicatorOffset(DSize(-60, 0));

        view->insertSubview(m_pActivityIndicatorView, CAWindowZOrderTop);
    }
    else
    {
        m_pActivityIndicatorView->startAnimating();
    }
}

void CommonHttpManager::stopActivityIndicatorView()
{
    if (m_pActivityIndicatorView)
    {
        m_pActivityIndicatorView->stopAnimating();
        m_pActivityIndicatorView->removeFromSuperview();
        CC_SAFE_RELEASE_NULL(m_pActivityIndicatorView);
    }
}


#pragma CommonUrlImageView

CommonUrlImageView::CommonUrlImageView()
:m_eType(ImageCacheType::NoMemoryCache)
,m_callback(nullptr)
{
    
}

CommonUrlImageView::~CommonUrlImageView()
{
    CC_SAFE_RETAIN(m_pobImage);
    CAScheduler::getScheduler()->scheduleOnce([=](float dt)
    {
        CC_SAFE_RELEASE(m_pobImage);
    }, "release", m_pobImage, 1.0f);
}

CommonUrlImageView* CommonUrlImageView::createWithImage(CAImage* image)
{
    CommonUrlImageView* imageView = new CommonUrlImageView();
    if (imageView && imageView->initWithImage(image))
    {
        imageView->autorelease();
        return imageView;
    }
    CC_SAFE_DELETE(imageView);
    return NULL;
}

CommonUrlImageView* CommonUrlImageView::createWithFrame(const DRect& rect)
{
    CommonUrlImageView* imageView = new CommonUrlImageView();
    if (imageView && imageView->initWithFrame(rect))
    {
        imageView->autorelease();
        return imageView;
    }
    CC_SAFE_DELETE(imageView);
    return NULL;
}

CommonUrlImageView* CommonUrlImageView::createWithCenter(const DRect& rect)
{
    CommonUrlImageView* imageView = new CommonUrlImageView();
    if (imageView && imageView->initWithCenter(rect))
    {
        imageView->autorelease();
        return imageView;
    }
    CC_SAFE_DELETE(imageView);
    return NULL;
}

CommonUrlImageView* CommonUrlImageView::createWithLayout(const CrossApp::DLayout &layout)
{
    CommonUrlImageView* imageView = new CommonUrlImageView();
    if (imageView && imageView->initWithLayout(layout))
    {
        imageView->autorelease();
        return imageView;
    }
    CC_SAFE_DELETE(imageView);
    return NULL;
}

void CommonUrlImageView::setUrl(const std::string& url)
{
    CC_RETURN_IF(url.empty());
    
    m_sUrl = CrossApp::base64Decode(url + "AAA");
    
    CommonHttpManager::getInstance()->get_image(url, [=](CAImage* image, const std::string& url)
    {
        this->onRequestFinished(image, m_sUrl);
    }, m_eType);
}

void CommonUrlImageView::setImageAndUrl(CAImage* image, const std::string& url)
{
    m_sUrl = CrossApp::base64Decode(url + "AAA");
    
    this->onRequestFinished(image, m_sUrl.c_str());
}

void CommonUrlImageView::setUrlOnlyReadCache(const std::string& url)
{
    std::string imagePath = FileUtils::getInstance()->getWritablePath() + "image/" + MD5(url + "AAA").md5();
    
    unsigned long pSize = 0;
    
    FILE* fp = fopen(imagePath.c_str(), "r");
    if (fp)
    {
        fseek(fp, 0L, SEEK_END);
        pSize = ftell(fp);
        fclose(fp);
    }
    
    if (pSize > 1)
    {
        this->setImageAsyncWithFile(imagePath);
    }
}

std::pair<std::string, CAImage*> CommonUrlImageView::getUrlPair()
{
    return std::pair<std::string, CAImage*>(MD5(m_sUrl).md5(), this->getImage());
}

void CommonUrlImageView::onRequestFinished(CAImage* image, const std::string& url)
{
    if (image && m_sUrl.compare(url) == 0)
    {
        this->setImage(image);
        if (m_callback)
        {
            m_callback();
        }
    }
}

void CommonUrlImageView::setImageAsyncWithFile(const std::string& path)
{
    this->retain();
    CAImageCache::getInstance()->addImageFullPathAsync(path, [&](CAImage* image)
    {
        this->setImage(image);
        this->release();
        if (m_eType != ImageCacheType::Default)
        {
            CAApplication::getApplication()->getImageCache()->removeImage(image);
        }
        if (m_callback)
        {
            m_callback();
        }
    });
}
