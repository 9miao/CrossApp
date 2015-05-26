 
#include "CommonHttpManager.h"

static const char* common_loadingBackground = "dm_resource/loading_background.png";
static const char* common_loadingIcon = "dm_resource/loading_icon.png";

#define REQUEST_JSON_COUNT 2
#define REQUEST_IMAGE_COUNT 2

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
    
protected:
    
    CommonImageCacheManager();
    
    virtual ~CommonImageCacheManager();
    
    void update();
    
private:
    
    CADeque<CAImage*> m_dImageQueue;
};

static CommonImageCacheManager* _imageCacheManager = NULL;

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

CommonImageCacheManager::CommonImageCacheManager()
{
    
}

CommonImageCacheManager::~CommonImageCacheManager()
{
    
}

void CommonImageCacheManager::update()
{
    CAImageCache::sharedImageCache()->removeImage(m_dImageQueue.front());
    m_dImageQueue.popFront();
}

void CommonImageCacheManager::pushImage(CAImage* image)
{
    CC_RETURN_IF(m_dImageQueue.contains(image));
    m_dImageQueue.pushBack(image);
    CCArray* array = CCArray::create();
    array->addObject(CCDelayTime::create(10));
    array->addObject(CCCallFunc::create(this, callfunc_selector(CommonImageCacheManager::update)));
    this->runAction(CCSequence::create(array));
}

void CommonImageCacheManager::removeImage(CAImage* image)
{
    m_dImageQueue.eraseObject(image);
}


#pragma CommonHttpManager

CommonHttpManager* _HttpManager = NULL;

CommonHttpManager* CommonHttpManager::getInstance()
{
    if (_HttpManager == NULL)
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
        _HttpManager = NULL;
    }
}

CommonHttpManager::CommonHttpManager()
: m_pActivityIndicatorView(NULL)
{
    for (int i=0; i<REQUEST_JSON_COUNT; i++)
    {
        CAHttpClient* httpClient = CAHttpClient::getInstance(15 - i);
        httpClient->setTimeoutForConnect(10);
        httpClient->setTimeoutForRead(10);
        m_pHttpJsonClients.push_back(httpClient);
    }
    for (int i=REQUEST_JSON_COUNT; i<REQUEST_JSON_COUNT + REQUEST_IMAGE_COUNT; i++)
    {
        CAHttpClient* httpClient = CAHttpClient::getInstance(15 - i);
        httpClient->setTimeoutForConnect(10);
        httpClient->setTimeoutForRead(10);
        m_pHttpImageClients.push_back(httpClient);
    }
    

    std::string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "cartoon_house_url.db";
    localStorageInit(fullPath.c_str());
    CommonImageCacheManager::getInstance();
}



CommonHttpManager::~CommonHttpManager()
{
    std::vector<CAHttpClient*>::iterator itr1;
    for (itr1 = m_pHttpJsonClients.begin(); itr1!=m_pHttpJsonClients.end(); itr1++)
    {
        (*itr1)->destroyInstance();
    }
    m_pHttpJsonClients.clear();
    
    for (itr1 = m_pHttpImageClients.begin(); itr1!=m_pHttpImageClients.end(); itr1++)
    {
        (*itr1)->destroyInstance();
    }
    m_pHttpImageClients.clear();
}


void CommonHttpManager::send_get(const std::string& url,std::map<std::string,
                                 std::string> key_value,
                                 CAObject* pTarget,
                                 SEL_CommonHttpJson pSelector,
                                 bool isLoading)
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

    CAHttpRequest* httpRequest = new CAHttpRequest();
    httpRequest->setUrl(getRul.c_str());
    httpRequest->setRequestType(CAHttpRequest::kHttpGet);
    CommonHttpResponseCallBack* callBack = CommonHttpResponseCallBack::create(pTarget, pSelector, url, CommonHttpResponseCallBack::CommonHttpResponseJson);
    httpRequest->setResponseCallback(callBack, httpresponse_selector(CommonHttpResponseCallBack::onResponse));
    
    std::sort(m_pHttpJsonClients.begin(), m_pHttpJsonClients.end(), compareHttpClient);
    m_pHttpJsonClients.front()->send(httpRequest);
    httpRequest->release();
    
    if (isLoading)
    {
        this->starActivityIndicatorView();
    }
}



void CommonHttpManager::send_post(const std::string& url,
                                  std::map<std::string,
                                  std::string> key_value,
                                  CAObject* pTarget,
                                  SEL_CommonHttpJson pSelector,
                                  bool isLoading)
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
    //CCLog("---%s %s",url.c_str(), postData.c_str());

    CAHttpRequest* httpRequest = new CAHttpRequest();
    httpRequest->setUrl(url.c_str());
    httpRequest->setRequestType(CAHttpRequest::kHttpPost);
    httpRequest->setRequestData(postData.c_str(), postData.length());
    CommonHttpResponseCallBack* callBack = CommonHttpResponseCallBack::create(pTarget, pSelector, url, CommonHttpResponseCallBack::CommonHttpResponseJsonNoCache);
    httpRequest->setResponseCallback(callBack, httpresponse_selector(CommonHttpResponseCallBack::onResponse));
    
    std::sort(m_pHttpJsonClients.begin(), m_pHttpJsonClients.end(), compareHttpClient);
    m_pHttpJsonClients.front()->send(httpRequest);
    httpRequest->release();
    
    if (isLoading)
    {
        this->starActivityIndicatorView();
    }
}

void CommonHttpManager::send_postFile(const std::string& url,
                                                             std::map<std::string, std::string> key_value,
                                                             const std::string& file,
                                                             CAObject* pTarget,
                                                             SEL_CommonHttpJson pSelector,
                                                             bool isLoading)
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
    
    CAHttpRequest* httpRequest = new CAHttpRequest();
    httpRequest->setUrl(url.c_str());
    httpRequest->setRequestType(CAHttpRequest::kHttpPostFile);
    httpRequest->setRequestData(postData.c_str(), postData.length());
    httpRequest->setFileNameToPost(file);
    CommonHttpResponseCallBack* callBack = CommonHttpResponseCallBack::create(pTarget, pSelector, url, CommonHttpResponseCallBack::CommonHttpResponseJsonNoCache);
    httpRequest->setResponseCallback(callBack, httpresponse_selector(CommonHttpResponseCallBack::onResponse));
    
    std::sort(m_pHttpJsonClients.begin(), m_pHttpJsonClients.end(), compareHttpClient);
    m_pHttpJsonClients.front()->send(httpRequest);
    httpRequest->release();
    
    if (isLoading)
    {
        this->starActivityIndicatorView();
    }
    
}

void CommonHttpManager::get_image(const std::string& url,
                                                         CAObject* pTarget,
                                                         SEL_CommonHttpImage pSelector,
                                                         const HttpGetImageType& type)
{
    std::string key = MD5(url).md5();
    
    CAImage* image = CAImageCache::sharedImageCache()->imageForKey(key);
    CommonHttpResponseCallBack* callBack = NULL;
    
    if (image)
    {
        (pTarget->*pSelector)(image, url.c_str());
    }
    else if (_httpResponses.count(url) > 0)
    {
        callBack = CommonHttpResponseCallBack::scheduleCallBack(pTarget, pSelector, url);
    }
    else
    {
        std::string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/" + key;
        
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
            callBack = CommonHttpResponseCallBack::imagePathAsync(pTarget, pSelector, imagePath, url, type);
        }
        else
        {
            CAHttpRequest* httpRequest = new CAHttpRequest();
            httpRequest->setUrl(url.c_str());
            httpRequest->setRequestType(CAHttpRequest::kHttpGet);
            std::vector<std::string> header;
            //header.push_back("Referer:");
            httpRequest->setHeaders(header);
            callBack = CommonHttpResponseCallBack::create(pTarget, pSelector, url, type);
            httpRequest->setResponseCallback(callBack, httpresponse_selector(CommonHttpResponseCallBack::onResponse));
            
            std::sort(m_pHttpImageClients.begin(), m_pHttpImageClients.end(), compareHttpClient);
            m_pHttpImageClients.front()->send(httpRequest);
            httpRequest->release();
        }
    }
}

void CommonHttpManager::starActivityIndicatorView()
{
    if (m_pActivityIndicatorView == NULL)
    {
        CAWindow* window = CAApplication::getApplication()->getRootWindow();
        CCRect rect = window->getBounds();
        
        m_pActivityIndicatorView = CAActivityIndicatorView::createWithFrame(rect);
        CAImageView* indicator = CAImageView::createWithFrame(CADipRect(0, 0, 50, 50));
        indicator->setImage(CAImage::create(common_loadingIcon));
        m_pActivityIndicatorView->setActivityIndicatorView(indicator);
        CAView* bg = CAView::createWithFrame(CADipRect(0, 0, 275, 300), CAColor_clear);
        CAImageView* bg2 = CAImageView::createWithFrame(CADipRect(0, 0, 275, 100));
        bg2->setImage(CAImage::create(common_loadingBackground));
        bg->addSubview(bg2);
        m_pActivityIndicatorView->setActivityBackView(bg);
        m_pActivityIndicatorView->setLoadingMinTime(0.3f);
        window->insertSubview(m_pActivityIndicatorView, CAWindowZoderTop);
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
    }
}


#pragma CommonHttpResponseCallBack

CommonHttpResponseCallBack::~CommonHttpResponseCallBack()
{
    _httpResponses.erase(m_sUrl);
    CC_SAFE_RELEASE_NULL(m_pTarget);
}

CommonHttpResponseCallBack::CommonHttpResponseCallBack(CAObject* pTarget, SEL_CommonHttpJson pSelector, const std::string& url, const CommonHttpResponseType& type)
:m_eType(type)
,m_pTarget(pTarget)
,m_pSelectorJson(pSelector)
,m_pSelectorImage(NULL)
,m_sUrl(url)
,m_eGetImageType(HttpGetImageDefault)
{
    CC_SAFE_RETAIN(pTarget);
    if (_httpResponses.count(m_sUrl) == 0)
    {
        _httpResponses[m_sUrl] = m_pTarget;
    }
}

CommonHttpResponseCallBack* CommonHttpResponseCallBack::create(CAObject* pTarget, SEL_CommonHttpJson pSelector, const std::string& url, const CommonHttpResponseType& type)
{
    CommonHttpResponseCallBack* callBack = new CommonHttpResponseCallBack(pTarget, pSelector, url, type);
    callBack->autorelease();
    return callBack;
}



CommonHttpResponseCallBack::CommonHttpResponseCallBack(CAObject* pTarget, SEL_CommonHttpImage pSelector, const std::string& url, const HttpGetImageType& type)
:m_eType(CommonHttpResponseImage)
,m_pTarget(pTarget)
,m_pSelectorJson(NULL)
,m_pSelectorImage(pSelector)
,m_sUrl(url)
,m_eGetImageType(type)
{
    CC_SAFE_RETAIN(m_pTarget);
    if (_httpResponses.count(m_sUrl) == 0)
    {
        _httpResponses[m_sUrl] = m_pTarget;
    }
}

CommonHttpResponseCallBack* CommonHttpResponseCallBack::create(CAObject* pTarget,
                                                               SEL_CommonHttpImage pSelector,
                                                               const std::string& url,
                                                               const HttpGetImageType& type)
{
    CommonHttpResponseCallBack* callBack = new CommonHttpResponseCallBack(pTarget, pSelector, url, type);
    callBack->autorelease();
    return callBack;
}

CommonHttpResponseCallBack* CommonHttpResponseCallBack::scheduleCallBack(CAObject* pTarget,
                                                                         SEL_CommonHttpImage pSelector,
                                                                         const std::string& url)
{
    if (pTarget == NULL) return NULL;
    CommonHttpResponseCallBack* callBack = new CommonHttpResponseCallBack(pTarget, pSelector, url);
    callBack->m_nTimes = 0;
    CAScheduler::schedule(schedule_selector(CommonHttpResponseCallBack::update), callBack, 1/10.0f);
    return callBack;
}

void CommonHttpResponseCallBack::update(float dt)
{
    m_nTimes++;
    
    std::string key = MD5(m_sUrl).md5();
    
    CAImage* image = CAImageCache::sharedImageCache()->imageForKey(key);
    
    if (image == NULL)
    {
        key = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/" + key;
        image = CAImageCache::sharedImageCache()->imageForKey(key);
    }
    
    if (image && m_pTarget)
    {
        (m_pTarget->*m_pSelectorImage)(image, m_sUrl.c_str());
    }
    
    if (m_nTimes > 100 || image)
    {
        CAScheduler::unschedule(schedule_selector(CommonHttpResponseCallBack::update), this);
        this->release();
    }
}

CommonHttpResponseCallBack* CommonHttpResponseCallBack::imagePathAsync(CrossApp::CAObject *pTarget,
                                                                       SEL_CommonHttpImage pSelector,
                                                                       const std::string &path,
                                                                       const std::string& url,
                                                                       const HttpGetImageType& type)
{
    if (pTarget == NULL) return NULL;
    CommonHttpResponseCallBack* callBack = new CommonHttpResponseCallBack(pTarget, pSelector, url, type);
    CAImageCache::sharedImageCache()->addImageFullPathAsync(path, callBack, callfuncO_selector(CommonHttpResponseCallBack::imagePathAsyncFinish));
    return callBack;
}

void CommonHttpResponseCallBack::imagePathAsyncFinish(CAObject* var)
{
    CAImage* image = static_cast<CAImage*>(var);
    if (image)
    {
        if (m_pTarget)
        {
            (m_pTarget->*m_pSelectorImage)(image, m_sUrl.c_str());
        }
        
        if (m_eGetImageType != HttpGetImageDefault)
        {
            CommonImageCacheManager::getInstance()->pushImage(image);
        }
    }
    this->release();
}


void CommonHttpResponseCallBack::onResponse(CAHttpClient* client, CAHttpResponse* response)
{
    CC_RETURN_IF(!m_pTarget);
    switch (m_eType)
    {
        case CommonHttpResponseJsonNoCache:
            this->onResponseJsonNoCache(client, response);
            break;
        case CommonHttpResponseJson:
            this->onResponseJson(client, response);
            break;
        case CommonHttpResponseImage:
            this->onResponseImage(client, response);
            break;
        default:
            break;
    }
}

void CommonHttpResponseCallBack::onResponseJsonNoCache(CAHttpClient* client, CAHttpResponse* response)
{
    CommonHttpManager::getInstance()->stopActivityIndicatorView();
    
    CC_RETURN_IF(!m_pSelectorJson);
    if (response->isSucceed())
    {
        std::string data(response->getResponseData()->begin(), response->getResponseData()->end());
        if (!data.empty())
        {
            if (CAViewController* viewController = dynamic_cast<CAViewController*>(m_pTarget))
            {
                CC_RETURN_IF(viewController->isViewRunning() == false);
            }
            
            CCLog("\n \n \n---------HttpResponse--json---------\n<<<\n%s\n>>>\n--------------END--------------\n \n \n",data.c_str());
            
            CSJson::Reader read;
            CSJson::Value root;
            bool succ = read.parse(data, root);
            if (succ == false)
            {
                CCLog("GetParseError \n");
            }
            
            if (m_pTarget)
            {
                (m_pTarget->*m_pSelectorJson)(HttpResponseSucceed, root);
            }
        }
        else
        {
            CSJson::Value root;
            if (m_pTarget)
            {
                (m_pTarget->*m_pSelectorJson)(HttpResponseFaild, root);
            }
        }
        
    }
    else
    {
        CSJson::Value root;
        if (m_pTarget)
        {
            (m_pTarget->*m_pSelectorJson)(HttpResponseFaild, root);
        }
    }
}

void CommonHttpResponseCallBack::onResponseJson(CAHttpClient* client, CAHttpResponse* response)
{
    CommonHttpManager::getInstance()->stopActivityIndicatorView();
    
    CC_RETURN_IF(!m_pSelectorJson);
    if (response->isSucceed())
    {
        std::string data(response->getResponseData()->begin(), response->getResponseData()->end());
        if (!data.empty())
        {
            localStorageSetItem(MD5(m_sUrl).md5().c_str(), data.c_str());
            
            if (CAViewController* viewController = dynamic_cast<CAViewController*>(m_pTarget))
            {
                CC_RETURN_IF(viewController->isViewRunning() == false);
            }
            
            //CCLog("\n \n \n---------HttpResponse--json---------\n<<<\n%s\n>>>\n--------------END--------------\n \n \n",data.c_str());
            
            CSJson::Reader read;
            CSJson::Value root;
            bool succ = read.parse(data, root);
            if (succ == false)
            {
                CCLog("GetParseError \n");
            }
            
            if (m_pTarget)
            {
                (m_pTarget->*m_pSelectorJson)(HttpResponseSucceed, root);
            }
        }
        else
        {
            if (CAViewController* viewController = dynamic_cast<CAViewController*>(m_pTarget))
            {
                CC_RETURN_IF(viewController->isViewRunning() == false);
            }
            
            CSJson::Value root;
            if (m_pTarget)
            {
                (m_pTarget->*m_pSelectorJson)(HttpResponseFaild, root);
            }
        }
        
    }
    else
    {
        if (CAViewController* viewController = dynamic_cast<CAViewController*>(m_pTarget))
        {
            CC_RETURN_IF(viewController->isViewRunning() == false);
        }
        
        const char* data = localStorageGetItem(MD5(m_sUrl).md5().c_str());

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
            
            if (m_pTarget)
            {
                (m_pTarget->*m_pSelectorJson)(HttpResponseSucceed, root);
            }
            
        }
        while (0);
        
        CSJson::Value root;
        if (m_pTarget)
        {
            (m_pTarget->*m_pSelectorJson)(HttpResponseFaild, root);
        }
    }
}

void CommonHttpResponseCallBack::onResponseImage(CAHttpClient* client, CAHttpResponse* response)
{
    CC_RETURN_IF(!m_pSelectorImage);
    if (response->isSucceed())
    {
        std::string data(response->getResponseData()->begin(), response->getResponseData()->end());
        unsigned char* pData = ((unsigned char*)(const_cast<char*>(data.c_str())));
        unsigned long pSize = data.length();

        CAImage* image = NULL;
        std::string key = MD5(m_sUrl).md5();

        if (m_eGetImageType == HttpGetImageDefault)
        {
            image = CAImage::createWithImageData(pData, pSize, key);
        }
        else
        {
            image = CAImage::createWithImageDataNoCache(pData, pSize);
        }
        
        if (m_eGetImageType != HttpGetImageNoAllCache)
        {
            std::string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/";
            CCFileUtils::sharedFileUtils()->createDirectory(imagePath.c_str());
            FILE *fp = fopen(string(imagePath + key).c_str(), "wb+");
            if (fp)
            {
                fwrite(pData, sizeof(char), pSize, fp);
                fclose(fp);
            }
        }
        
        if (m_pTarget && m_pSelectorImage && image)
        {
            (m_pTarget->*m_pSelectorImage)(image, m_sUrl.c_str());
        }
    }
}

#pragma CommonUrlImageView

CommonUrlImageViewDelegate::CommonUrlImageViewDelegate()
{

}

CommonUrlImageViewDelegate::~CommonUrlImageViewDelegate()
{

}

CommonUrlImageView::CommonUrlImageView()
:m_pDelegate(NULL)
,m_eType(HttpGetImageDefault)
{
    
}

CommonUrlImageView::~CommonUrlImageView()
{
    CC_SAFE_RELEASE(dynamic_cast<CAObject*>(m_pDelegate));
    m_pDelegate = NULL;
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

CommonUrlImageView* CommonUrlImageView::createWithFrame(const CCRect& rect)
{
    CommonUrlImageView* imageView = new CommonUrlImageView();
    if (imageView && imageView->CAView::initWithFrame(rect))
    {
        imageView->autorelease();
        return imageView;
    }
    CC_SAFE_DELETE(imageView);
    return NULL;
}

CommonUrlImageView* CommonUrlImageView::createWithCenter(const CCRect& rect)
{
    CommonUrlImageView* imageView = new CommonUrlImageView();
    if (imageView && imageView->CAView::initWithCenter(rect))
    {
        imageView->autorelease();
        return imageView;
    }
    CC_SAFE_DELETE(imageView);
    return NULL;
}

void CommonUrlImageView::setUrl(const std::string& url)
{
    m_sUrl = DecodeURL(url);
    CommonHttpManager::getInstance()->get_image(m_sUrl, this, CommonHttpImage_selector(CommonUrlImageView::onRequestFinished), m_eType);
}

void CommonUrlImageView::setImageAndUrl(CAImage* image, const std::string& url)
{
    m_sUrl = DecodeURL(url);
    this->onRequestFinished(image, m_sUrl.c_str());
}

void CommonUrlImageView::setUrlOnlyReadCache(const std::string& url)
{
    std::string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/" + MD5(url).md5();
    
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

void CommonUrlImageView::onRequestFinished(CAImage* image, const char* url)
{
    CC_RETURN_IF(url && m_sUrl.compare(url) != 0);
    this->setImage(image);
    if (m_pDelegate)
    {
        m_pDelegate->imageViewRequestFinished(this);
    }
}

void CommonUrlImageView::asyncFinish(CAObject* var)
{
    CAImageView::asyncFinish(var);
    if (m_eType != HttpGetImageDefault)
    {
        CAImageCache::sharedImageCache()->removeImage(static_cast<CAImage*>(var));
    }
    if (m_pDelegate)
    {
        m_pDelegate->imageViewRequestFinished(this);
    }
}

void CommonUrlImageView::setDelegate(CommonUrlImageViewDelegate *var)
{
    CC_SAFE_RELEASE(dynamic_cast<CAObject*>(m_pDelegate));
    CC_SAFE_RETAIN(dynamic_cast<CAObject*>(var));
    m_pDelegate = var;
}

CommonUrlImageViewDelegate* CommonUrlImageView::getDelegate()
{
    return m_pDelegate;
}

