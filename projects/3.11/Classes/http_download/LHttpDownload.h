//
//  LHttpDownload.h
//  DDHero
//
//  Created by mac on 13-8-20.
//
//

#ifndef __DDHero__LHttpDownload__
#define __DDHero__LHttpDownload__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LHttpDownload;
class LHttpDownloadDelegate
{
public:
    void removeDelegate(LHttpDownload* sender);
    
    virtual void downLoadFinish(const char* fileName) = 0;
    
    virtual void downLoadFaild() = 0;
};

class LHttpDownload: public CCNode
{
    void onHttpRequestCompleted(cocos2d::CCNode* pSender,void* data);
    
    std::string m_sPhotoFile;
    
    CCString* urlString;
    
    CCString* m_UIDString;
    
    LHttpDownloadDelegate* delegate;
    
    CCHttpRequest* request;
    
    bool isCanRelease;
    
    /**
     *  ImageType:
     *
     *   0 -> PNG
     *   1 -> JPG
     *   2 -> GIF
     */
    
    int getImageType(unsigned char * pData, unsigned long * pSize);

    bool getImage();
    
    void update(float delta);
    
public:
    
    void setDelegate(LHttpDownloadDelegate* var);
    
    LHttpDownload(LHttpDownloadDelegate *var, const char* url, const char* UID);

    virtual ~LHttpDownload();
    
    static LHttpDownload* create(LHttpDownloadDelegate *var, const char* url, const char* UID);

    void renunciation();
    
};



#endif /* defined(__DDHero__LHttpDownload__) */
