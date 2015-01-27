//
//  CASyncImageView.h
//  LookLook
//
//  Created by dai xinping on 14-11-20.
//
//

#ifndef __LookLook__CASyncImageView__
#define __LookLook__CASyncImageView__

#include <stdio.h>
#include "view/CAImageView.h"
#include "network/CAHttpClient.h"
#include "images/CAImage.h"

using namespace CrossApp;
using namespace CrossApp::extension;
using namespace std;

class CASyncImageView : public CAImageView {
    
public:
    CASyncImageView();
    virtual ~CASyncImageView();
    
    static CASyncImageView* create();
    static CASyncImageView* createWithUrl(const string& url);
    static CASyncImageView* createWithFrame(const CCRect& rect);
    static CASyncImageView* createWithCenter(const CCRect& rect);
    
    virtual bool init();
    virtual bool initWithUrl(const string& url);

    virtual void setUrl(const string& url);
    virtual const string& getUrl();
    
    virtual void setLoadingImage(CAImage* image);
    virtual CAImage* getLoadingImage();
    
protected:
    using CAImageView::setImageAsyncWithFile;
    
    virtual void setImage(CAImage* image);
    virtual void onGetImageRsp(CAHttpClient* client, CAHttpResponse* response);
    virtual void onCheckIdleHttpClient(float dt);
    virtual void onDecodeImageSelector(CAObject* pSender);
    virtual bool findIdleAndSendReq();
    
private:
    string m_URL;
    CAHttpClient* m_httpClient;
    CAHttpRequest* m_httpReq;
    CAImage* m_loadingImage;
    CAImage* m_decodeImage;
};

#endif /* defined(__LookLook__CASyncImageView__) */
