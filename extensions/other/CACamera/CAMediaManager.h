//
//  CAMediaManager.h
//  LocationTest
//
//  Created by lh on 14-5-29.
//
//

#ifndef __LocationTest__CAMediaManager__
#define __LocationTest__CAMediaManager__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

NS_CC_BEGIN

class CAMediaDelegate
{
public:
    virtual void getSelectedImage(CCImage *image) = 0;
};

class CAMediaManager:public CCObject
{
public:
    
    CAMediaManager();
    
    virtual ~CAMediaManager();
    
    static CAMediaManager* shareCAMediaManager();
    
    void addCameraView();
    
    void addAlbumView();
    
    void *_camera;
    
    void getSelectedImage(const char*path);
    
    
protected:
    
    CC_SYNTHESIZE(CAMediaDelegate *, m_pMediaDelegate, MediaDelegate);
    
    void *m_pBuffer;
    
    unsigned long n_fileSize;
    
private:
    
    void initAndroidImage();
    
};

NS_CC_END
#endif /* defined(__LocationTest__CAMediaManager__) */
