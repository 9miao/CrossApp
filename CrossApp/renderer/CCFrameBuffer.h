
#ifndef __CC_FRAME_BUFFER_H__
#define __CC_FRAME_BUFFER_H__

#include "basics/CAObject.h"
#include "CCGL.h"
#include "images/CAImage.h"
#include <set>

NS_CC_BEGIN

class CCEGLViewProtocol;

namespace experimental {

/**
 Viewport is a normalized to FrameBufferObject
 But for default FBO, the size is absolute.
 */
struct CC_DLL Viewport
{
    Viewport(float left, float bottom, float width, float height);
    Viewport();
    
    float _left;
    float _bottom;
    float _width;
    float _height;
};
    
class CC_DLL RenderTargetBase : public CAObject
{
public:
    enum class Type
    {
        RenderBuffer,
        CAImage,
    };
protected:
    RenderTargetBase();
    virtual ~RenderTargetBase();
    bool init(unsigned int width, unsigned int height);

public:
    
    virtual CAImage* getImage() const { return nullptr; }
    virtual GLuint getBuffer() const { return 0; }
    
    unsigned int getWidth() const { return _width; }
    unsigned int getHeight() const { return _height; }
    Type getType() const { return _type; }
protected:
    Type _type;
    unsigned int _width;
    unsigned int _height;
    
};

class CC_DLL RenderTarget : public RenderTargetBase
{
public:
    
    static RenderTarget* create(unsigned int width, unsigned int height, CAImage::PixelFormat format = CAImage::PixelFormat::RGBA8888);
    
    bool init(unsigned int width, unsigned int height, CAImage::PixelFormat format);
    
    virtual CAImage* getImage() const { return _image; }
public:
    RenderTarget();
    virtual ~RenderTarget();
    
protected:
    CAImage* _image;
};

class CC_DLL RenderTargetRenderBuffer : public RenderTargetBase
{
public:
    
    static RenderTargetRenderBuffer* create(unsigned int width, unsigned int height);
    
    bool init(unsigned int width, unsigned int height);
    
    virtual GLuint getBuffer() const { return _colorBuffer; }
    
public:
    RenderTargetRenderBuffer();
    virtual ~RenderTargetRenderBuffer();
    
protected:
    GLenum _format;
    GLuint _colorBuffer;
};

class CC_DLL RenderTargetDepthStencil : public RenderTargetBase
{
public:
    
    static RenderTargetDepthStencil* create(unsigned int width, unsigned int height);
    
    bool init(unsigned int width, unsigned int height);
    
    virtual GLuint getBuffer() const { return _depthStencilBuffer; }
    
    CC_DEPRECATED(3.7) GLuint getDepthStencilBuffer() const { return _depthStencilBuffer; }
public:
    RenderTargetDepthStencil();
    virtual ~RenderTargetDepthStencil();
    
protected:

    GLuint _depthStencilBuffer;
};

class CC_DLL FrameBuffer : public CAObject
{
public:
    static FrameBuffer* create(uint8_t fid, unsigned int width, unsigned int height);
    
    bool init(uint8_t fid, unsigned int width, unsigned int height);
public:
    GLuint getFBO() const { return _fbo; }
    GLuint getFID() const { return _fid; }
    //call glclear to clear frame buffer object
    void clearFBO();
    void applyFBO();
    void restoreFBO();
    void setClearColor(const CAColor4F& color) { _clearColor = color;}
    void setClearDepth(float depth) { _clearDepth = depth; }
    void setClearStencil(int8_t stencil) { _clearStencil = stencil; }
    const CAColor4F& getClearColor() const { return _clearColor; }
    float getClearDepth() const { return _clearDepth; }
    int8_t getClearStencil() const { return _clearStencil; }
    
    RenderTargetBase* getRenderTarget() const { return _rt; }
    RenderTargetDepthStencil* getDepthStencilTarget() const { return _rtDepthStencil; }
    void attachRenderTarget(RenderTargetBase* rt);
    void attachDepthStencilTarget(RenderTargetDepthStencil* rt);
    
    bool isDefaultFBO() const { return _isDefault; }
    unsigned int getWidth() const { return _width; }
    unsigned int getHeight() const { return _height; }

public:
    FrameBuffer();
    virtual ~FrameBuffer();
    bool initWithGLView(CCEGLViewProtocol* view);
private:
    //openGL content for FrameBuffer
    GLuint _fbo;
    GLuint _previousFBO;
    //dirty flag for fbo binding
    bool _fboBindingDirty;
    //
    uint8_t _fid;
    //
    CAColor4F _clearColor;
    float   _clearDepth;
    int8_t  _clearStencil;
    int _width;
    int _height;
    RenderTargetBase* _rt;
    RenderTargetDepthStencil* _rtDepthStencil;
    bool _isDefault;
public:
    static FrameBuffer* getOrCreateDefaultFBO(CCEGLViewProtocol* glView);
    static void applyDefaultFBO();
    static void clearAllFBOs();
private:
    //static GLuint _defaultFBO;
    static FrameBuffer* _defaultFBO;
    static std::set<FrameBuffer*> _frameBuffers;

};
} // end of namespace experimental

NS_CC_END

#endif /* defined(__CC_FRAME_BUFFER_H__) */
