//
//  LHttpDownload.cpp
//  DDHero
//
//  Created by mac on 13-8-20.
//
//

#include "LHttpDownload.h"
#include <pthread.h>
#include "FSDirectoryManager.h"

#define fileName "picture/"

LHttpDownload::LHttpDownload(LHttpDownloadDelegate *var, const char* url, const char* UID)
:urlString(NULL)
,m_UIDString(NULL)
,delegate(NULL)
,request(NULL)
,isCanRelease(false)
{

//    CCLOG("main thread:%x",pthread_self());
    this->setDelegate(var);
    
    urlString=CCString::create(url);
    urlString->retain();
    m_UIDString=CCString::create(UID);
    m_UIDString->retain();
    
    std::string desPath = FSDirectoryManager::getRootDirectoryByType(kFSDirTypeDocument);
    desPath += fileName;
    desPath += url;
    
    std::string stdUrl=std::string(url);
    
    int position = stdUrl.rfind("/");
    stdUrl = stdUrl.substr(0,position);
    position = stdUrl.rfind("/");
    stdUrl = stdUrl.substr(position+1,stdUrl.length()-position-1);
    
    m_sPhotoFile = std::string(FSDirectoryManager::getRootDirectoryByType(kFSDirTypeDocument) + "picture/" + stdUrl + m_UIDString->getCString());
    
    do
    {
        CC_BREAK_IF(this->getImage());
        
        isCanRelease = false;
        
        request = new CCHttpRequest();
        request->setUrl(url);
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback(this, httpresponse_selector(LHttpDownload::onHttpRequestCompleted));
        CCHttpClient::getInstance()->setTimeoutForConnect(4000);
        CCHttpClient::getInstance()->send(request);
    }
    while (0);
}

LHttpDownload::~LHttpDownload()
{
    if (request)
    {
        request->release();
        request=NULL;
    }
    
    if (delegate)
    {
        delegate->removeDelegate(this);
    }

    CC_SAFE_RELEASE_NULL(urlString);
    CC_SAFE_RELEASE_NULL(m_UIDString);
}

LHttpDownload* LHttpDownload::create(LHttpDownloadDelegate *var, const char* url, const char* UID)
{
    
    LHttpDownload* httpDownload=new LHttpDownload(var, url, UID);
    
    if (httpDownload && httpDownload->init())
    {
        return httpDownload;
    }
    
    CC_SAFE_DELETE(httpDownload);
    
    return NULL;;
}

bool LHttpDownload::getImage()
{
    do
    {
        CC_BREAK_IF(delegate == NULL);
        
        std::string filePathPNG = m_sPhotoFile + ".png";
        
        FILE* fpPNG=fopen(filePathPNG.c_str(), "rt");
        if (fpPNG)
        {
            delegate->downLoadFinish(filePathPNG.c_str());
            isCanRelease = true;
            return true;
        }
        
        std::string filePathJPG = m_sPhotoFile + ".jpg";
        
        FILE* fpJPG=fopen(filePathJPG.c_str(), "rt");
        if (fpJPG)
        {
            delegate->downLoadFinish(filePathJPG.c_str());
            isCanRelease = true;
            return true;
        }
        
        std::string filePathGIF = m_sPhotoFile + ".gif";
        
        FILE* fpGIF=fopen(filePathGIF.c_str(), "rt");
        if (fpGIF)
        {

            delegate->downLoadFinish(filePathGIF.c_str());
            isCanRelease = true;
            return true;
        }
    }
    while (0);
    
    return false;
}

void LHttpDownload::update(float delta)
{
    this->getImage();
}

void LHttpDownload::onHttpRequestCompleted(cocos2d::CCNode* pSender,void* data)
{

    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response)
    {
        //提示更新失败
        if (delegate)
        {
            delegate->downLoadFaild();
        }
        isCanRelease = true;
        return;
    }
    
    if (response && !response->isSucceed())
    {
        //提示更新失败
        if (delegate)
        {
            delegate->downLoadFaild();
        }
        isCanRelease = true;
        return;
    }

    std::vector<char> *buffer = ((CCHttpResponse*)data)->getResponseData();
    std::string buff_(buffer->begin(),buffer->end());

    FSDirectoryManager::creatChildDirectoryAtPath(fileName, kFSDirTypeDocument);
    std::string stdUrl=urlString->m_sString;

    int position = stdUrl.rfind("/");
    stdUrl = stdUrl.substr(0,position);
    position = stdUrl.rfind("/");
    stdUrl = stdUrl.substr(position+1,stdUrl.length()-position-1);

    unsigned char* pData=((unsigned char*)(const_cast<char*>(buff_.c_str())));
    unsigned long pSize=buffer->size();
    
    std::string filePath;

    if (0 == this->getImageType(pData, &pSize))
    {
        filePath = m_sPhotoFile + ".png";
    }
    else if (1 == this->getImageType(pData, &pSize))
    {
        filePath = m_sPhotoFile + ".jpg";
    }
    else
    {
        filePath = m_sPhotoFile + ".gif";
    }
    
    FILE *fp = fopen(filePath.c_str(), "wb+");
	if (!fp) return;
    
    fwrite(pData, sizeof(char), pSize, fp);
	fclose(fp);

    this->scheduleOnce(schedule_selector(LHttpDownload::update), 1/60.0f);
}

int LHttpDownload::getImageType(unsigned char * pData, unsigned long * pSize)
{

    if (pData[0]==137 && pData[1]==80 && pData[2]==78)
    {
        return 0;
    }
    if (pData[0]==11 && pData[1]==2 && pData[2]==1)
    {
        return 1;
    }
    return 2;
}

void LHttpDownload::setDelegate(LHttpDownloadDelegate *var)
{
    delegate = var;
}

void LHttpDownload::renunciation()
{

    if (isCanRelease)
    {
        this->stopAllActions();
        this->release();
    }
    else
    {
        this->runAction(CCSequence::create(CCDelayTime::create(0.3),CCCallFunc::create(this, callfunc_selector(LHttpDownload::renunciation)),NULL));
    }
}

void LHttpDownloadDelegate::removeDelegate(LHttpDownload* sender)
{
    
    sender->setDelegate(NULL);
}
