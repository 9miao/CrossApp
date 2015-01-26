//
//  CAVideoPlayerView.h
//  CrossApp
//
//  Created by dai xinping on 14-10-27.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerView__
#define __CrossApp__CAVideoPlayerView__

#include "view/CAView.h"
#include "view/CAImageView.h"
#include "view/CARenderImage.h"
#include "CAVideoPlayerRender.h"
#include "CAVideoPlayerDecoder.h"

using namespace CrossApp;
using namespace CAVideoPlayer;
using namespace std;


class CAVideoPlayerView : public CAView {
        
public:
    CAVideoPlayerView(VPDecoder* decoder);
    ~CAVideoPlayerView();
    
    static CAVideoPlayerView* create(VPDecoder* decoder);
    static CAVideoPlayerView* createWithFrame(const CCRect& rect, VPDecoder* decoder);
    static CAVideoPlayerView* createWithCenter(const CCRect& rect, VPDecoder* decoder);

    virtual bool init();
    virtual void visit();
    virtual void draw();
    virtual bool initWithFrame(const CCRect& rect);
    virtual bool initWithCenter(const CCRect& rect);
    virtual void setContentSize(const CCSize& size);
    virtual void setImageCoords(CCRect rect);
    virtual void updateImageRect();
    
    virtual void setCurrentFrame(VPVideoFrame* frame);
    
    virtual void setDecoder(VPDecoder* decoder);
        
protected:
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

    
private:
    enum {
        ATTRIBUTE_VERTEX,
        ATTRIBUTE_TEXCOORD,
    };
    
private:
    GLint           _backingWidth;
    GLint           _backingHeight;
    GLuint          _program;
    GLint           _uniformMatrix;
    GLfloat         _vertices[8];
    
    VPDecoder       *_decoder;
    VPFrameRender   *_renderer;
    VPVideoFrame    *_currFrame;
    
    CAImageView     *_image;
    
    CCRect          _pictRect;
    
private:
    bool loadShaders();
    
private:
    double m_elapsed;               // 用于帧率控制

};


#endif /* defined(__CrossApp__CAVideoPlayerView__) */
