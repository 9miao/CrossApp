//
//  CAVideoPlayerView.h
//  CrossApp
//
//  Created by dai xinping on 14-11-6.
//  Modified by zhujian on 15-9-6
//
//  Copyright (c) 2015年 http://www.9miao.com. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerView__
#define __CrossApp__CAVideoPlayerView__

#include "view/CAView.h"
#include "view/CAImageView.h"
#include "view/CARenderImage.h"
#include "CAVideoPlayerRender.h"
#include "CAVideoPlayerDecoder.h"

NS_CC_BEGIN

class VPDecoder;
class CC_DLL CAVideoPlayerView : public CAView 
{
public:
    CAVideoPlayerView(VPDecoder* decoder);
    ~CAVideoPlayerView();
    
    static CAVideoPlayerView* create(VPDecoder* decoder);
    static CAVideoPlayerView* createWithFrame(const CCRect& rect, VPDecoder* decoder);
    static CAVideoPlayerView* createWithCenter(const CCRect& rect, VPDecoder* decoder);

    virtual bool init();
    virtual void visit();
    virtual void draw();
    virtual void setContentSize(const CCSize& size);
    virtual void setImageCoords(CCRect rect);
    virtual void updateImageRect();
    
    virtual void setCurrentFrame(VPVideoFrame* frame);
    
        
   
private:
    GLuint          _program;
    GLint           _uniformMatrix;
    GLfloat         _vertices[8];
    
    VPDecoder       *_decoder;
    VPFrameRender   *_renderer;
    VPVideoFrame    *_currFrame;
    
    CCRect          _pictRect;
    
private:
    bool loadShaders();
};

NS_CC_END


#endif /* defined(__CrossApp__CAVideoPlayerView__) */
