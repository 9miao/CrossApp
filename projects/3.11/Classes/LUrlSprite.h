//
//  LUrlSprite.h
//  project
//
//  Created by 栗元峰 on 14-3-21.
//
//

#ifndef __project__LUrlSprite__
#define __project__LUrlSprite__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LHttpDownload.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LUrlSprite
: public CCSprite
, public LHttpDownloadDelegate
{
    LHttpDownload *m_httpDownload;
    
    virtual void downLoadFinish(const char* fileName);
    
    virtual void downLoadFaild();
    
    std::string m_url;
    
    void downLoad();
    
public:
    
    static LUrlSprite* create(const char* url);
    
    void setTextureForUrl(const char* url);
    
    void onEnterTransitionDidFinish();
    
    void onExitTransitionDidStart();
    
};


#endif /* defined(__project__LUrlSprite__) */
