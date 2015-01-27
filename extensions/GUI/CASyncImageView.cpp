//
//  CASyncImageView.cpp
//  LookLook
//
//  Created by dai xinping on 14-11-20.
//
//

#include "CASyncImageView.h"
#include "basics/CAScheduler.h"
#include "support/md5.h"

#define THREAD_COUNT 5

static CAHttpClient* findIdleHttpClient()
{
    static CAHttpClient* http_table[THREAD_COUNT] = {NULL};
    int thread = 0;
    int targetThread = 0;
    while (thread < THREAD_COUNT) {
        if (http_table[thread] == NULL) {
            http_table[thread] = CAHttpClient::create();
            http_table[thread]->retain();
            targetThread = thread;
            break;
        }
        
        if (http_table[thread]->getRequestCount() < 
            http_table[targetThread]->getRequestCount()) {
            targetThread = thread;
        }
        
        thread++;
    }
    
    return http_table[targetThread];
}

static bool writeImageData(const char* data, int size, const char* name)
{
    string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/cache/");
    if (!CCFileUtils::sharedFileUtils()->isFileExist(path)) {
        if (!CCFileUtils::sharedFileUtils()->createDirectory(path.c_str())) {
            CCLog("%s, create dir error", __FUNCTION__);
            return false;
        }
    }
    path.append(name);
    
    FILE* fp = fopen(path.c_str(), "wb");
    if (fp) {
        fwrite(data, size, 1, fp);
        fclose(fp);
    } else {
        CCLog("%s, create file error", __FUNCTION__);
        return false;
    }

    return true;
}

static string getImagePath(const char* name)
{
    string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/cache/");
    path.append(name);
    return path;
}

CASyncImageView::CASyncImageView()
{
    m_URL = "";
    m_httpClient = NULL;
    m_httpReq = NULL;
    m_loadingImage = NULL;
    m_decodeImage = NULL;
}

CASyncImageView::~CASyncImageView()
{
    CC_SAFE_RELEASE(m_httpClient);
    CC_SAFE_RELEASE(m_httpReq);
    CC_SAFE_RELEASE(m_loadingImage);
    CC_SAFE_RELEASE(m_decodeImage);
}

CASyncImageView* CASyncImageView::create()
{
    CASyncImageView* view = new CASyncImageView();
    if (view && view->init()) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CASyncImageView* CASyncImageView::createWithUrl(const string& url)
{
    CASyncImageView* view = new CASyncImageView();
    if (view && view->initWithUrl(url)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;    
}

CASyncImageView* CASyncImageView::createWithFrame(const CCRect& rect)
{
    CASyncImageView* view = new CASyncImageView();
    if (view && view->initWithFrame(rect, ccc4(0, 0, 0, 0))) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CASyncImageView* CASyncImageView::createWithCenter(const CCRect& rect)
{
    CASyncImageView* view = new CASyncImageView();
    if (view && view->initWithCenter(rect, ccc4(0, 0, 0, 0))) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}


bool CASyncImageView::init()
{
    if (!CAImageView::init()) {
        return false;
    }
    
    return true;
}

bool CASyncImageView::initWithUrl(const string &url)
{
    if (!CAImageView::init()) {
        return false;
    }
    
    setUrl(url);
    
    return true;
}

void CASyncImageView::setUrl(const string &url)
{
    m_URL = url;
    
    string name = MD5(m_URL).md5();
    string path = getImagePath(name.c_str());
    if (CCFileUtils::sharedFileUtils()->isFileExist(path)) {
//        CAImage* image = CAImageCache::sharedImageCache()->addImage(path);
//        onDecodeImageSelector(image);
        CAImage* image = CAImageCache::sharedImageCache()->imageForKey(path);
        if (image) {
            setImage(image);
        } else {
            CAImageCache::sharedImageCache()->addImageAsync(path, this, callfuncO_selector(CASyncImageView::onDecodeImageSelector));            
        }
    } else if (!findIdleAndSendReq()) {
        CAScheduler::schedule(schedule_selector(CASyncImageView::onCheckIdleHttpClient), this, 1.0f);
    }
}

const string& CASyncImageView::getUrl()
{
    return m_URL;
}


void CASyncImageView::onGetImageRsp(CAHttpClient* client, CAHttpResponse* response)
{
    if (response->getHttpRequest() != m_httpReq) {
        return;
    }
    
    if (response->isSucceed()) {
        const char* data = response->getResponseData()->data();
        int size = response->getResponseData()->size();
        string name = MD5(m_URL).md5();
        if (!writeImageData(data, size, name.c_str())) {
            return;
        }
        string path = getImagePath(name.c_str());
        CAImageCache::sharedImageCache()->addImageAsync(path, this, callfuncO_selector(CASyncImageView::onDecodeImageSelector));
    } else {
        CCLog("%s, code = %d, %s", __FUNCTION__, response->getResponseCode(), response->getErrorBuffer());
    }
}

void CASyncImageView::onCheckIdleHttpClient(float dt)
{
    if (findIdleAndSendReq()) {
        CAScheduler::unschedule(schedule_selector(CASyncImageView::onCheckIdleHttpClient), this);
    }
}

bool CASyncImageView::findIdleAndSendReq()
{
    CC_SAFE_RELEASE_NULL(m_httpClient);
    m_httpClient = findIdleHttpClient();
    CC_SAFE_RETAIN(m_httpClient);
    
    if (m_httpClient) {
        CCLog("%s, %s", __FUNCTION__, m_URL.c_str());
        CAHttpRequest* req = new CAHttpRequest();
        req->autorelease();
        req->setRequestType(CAHttpRequest::kHttpGet);
        req->setUrl(m_URL.c_str());
        req->setResponseCallback(this, httprsp_selector(CASyncImageView::onGetImageRsp));
        req->setTag(m_URL.c_str());
        m_httpClient->send(req);
        
        CC_SAFE_RELEASE_NULL(m_httpReq);
        m_httpReq = req;
        m_httpReq->retain();
        
        return true;
    }
    
    return false;
}

void CASyncImageView::setLoadingImage(CAImage* image)
{
    CC_SAFE_RELEASE_NULL(m_loadingImage);
    m_loadingImage = image;
    CC_SAFE_RETAIN(m_loadingImage);
    
    if (m_loadingImage) {
        setImage(image);
    }
}

CAImage* CASyncImageView::getLoadingImage()
{
    return m_loadingImage;
}

void CASyncImageView::setImage(CrossApp::CAImage *image)
{
    CC_SAFE_RELEASE(m_decodeImage);
    m_decodeImage = (CAImage*)image;
    CC_SAFE_RETAIN(m_decodeImage);
    
    if (m_decodeImage) {
        CAImageView::setImage(m_decodeImage);
    } else {
        CAImageView::setImage(m_loadingImage);
    }    
}

void CASyncImageView::onDecodeImageSelector(CrossApp::CAObject *pSender)
{
    setImage((CAImage*)pSender);
    
    setAlpha(0);
    runAction(CCFadeIn::create(0.5f));
}
