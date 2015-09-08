//
//  CAVideoPlayerRender.h
//  CrossApp
//
//  Created by dai xinping on 14-11-6.
//  Modified by zhujian on 15-9-6
//
//  Copyright (c) 2015年 http://www.9miao.com. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerRender__
#define __CrossApp__CAVideoPlayerRender__

#include <stdio.h>
#include <string>
#include "basics/CAObject.h"
#include "basics/CAGeometry.h"
#include "CCGL.h"

NS_CC_BEGIN

class VPVideoFrame;
class VPFrameRender 
{    
public:
    VPFrameRender();
    virtual ~VPFrameRender();
    
    virtual bool isValid() = 0;
    virtual const char* fragmentShader() = 0;
    virtual const char* key() = 0;
    virtual void resolveUniforms(GLuint program) = 0;
    virtual void setFrame(VPVideoFrame* frame) = 0;
    virtual bool prepareRender() = 0;
    virtual CCRect updateVertices(float width, float height, float screen_w, float screen_h);
    virtual bool loadShaders();
    
    virtual void draw(VPVideoFrame* frame, long offset);
    
protected:
    
    GLuint _program;
    GLint  _uniformMatrix;
    GLfloat _vertices[8];
    
    GLuint _renderBufer;
    
    std::string _key;
};

class VPFrameRenderRGB : public VPFrameRender 
{
    GLint _uniformSampler;
    GLuint _texture;

public:
    VPFrameRenderRGB();
    virtual ~VPFrameRenderRGB();
    
    virtual bool isValid();
    virtual const char* fragmentShader();
    virtual void resolveUniforms(GLuint program);
    virtual void setFrame(VPVideoFrame* frame);
    virtual bool prepareRender();
    virtual const char* key();
};

class VPFrameRenderYUV : public VPFrameRender 
{
    GLint _uniformSamplers[3];
    GLuint _textures[3];
    
public:
    VPFrameRenderYUV();
    virtual ~VPFrameRenderYUV();
    
    virtual bool isValid();
    virtual const char* fragmentShader();
    virtual void resolveUniforms(GLuint program);
    virtual void setFrame(VPVideoFrame* frame);
    virtual bool prepareRender();
    virtual const char* key();
};

NS_CC_END

#endif /* defined(__CrossApp__CAVideoPlayerRender__) */
