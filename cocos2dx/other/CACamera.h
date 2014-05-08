//
//  CACamera.h
//
//  Created by lh on 14-5-5.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#ifndef __iBook__CACamera__
#define __iBook__CACamera__

#include <iostream>
#include "cocoa/CCObject.h"
#include "platform/CCImage.h"

NS_CC_BEGIN

class CACameraDelegate
{
public:
    virtual void getSelectedImage(CCImage *image) = 0;
};

class CACamera:public CCObject
{
public:
    
    CACamera();
    
    virtual ~CACamera();
    
    static CACamera* shareCamera();
   
    void addCameraView();
    
    void addAlbumView();
    
    void *_camera;
    
    void getSelectedImage(const char*path);
    
    
protected:
    
    CC_SYNTHESIZE(CACameraDelegate *, m_pCameraDelegate, CameraDelegate);
    
    void *m_pBuffer;
    
    unsigned long n_fileSize;
    
private:
    
    void initAndroidImage();
    
};

NS_CC_END

#endif /* defined(__iBook__CACamera__) */
