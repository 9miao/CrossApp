
#ifndef __CCRENDER_TEXTURE_H__
#define __CCRENDER_TEXTURE_H__

#include "CAView.h"
#include "CAImageView.h"
#include "kazmath/mat4.h"

NS_CC_BEGIN

class CC_DLL CARenderImage : public CAView
{
public:

    CARenderImage();

    virtual ~CARenderImage();
    
    virtual void visit();
    virtual void draw();

    static CARenderImage * create(int w ,int h, CAImage::PixelFormat eFormat, GLuint uDepthStencilFormat);

    static CARenderImage * create(int w, int h, CAImage::PixelFormat eFormat);

    static CARenderImage * create(int w, int h);

    bool initWithWidthAndHeight(int w, int h, CAImage::PixelFormat eFormat);

    bool initWithWidthAndHeight(int w, int h, CAImage::PixelFormat eFormat, GLuint uDepthStencilFormat);

    void printscreenWithView(CAView* view);
    
    void printscreenWithView(CAView* view, DPoint offset);
    
    void printscreenWithView(CAView* view, const CAColor4B& backgroundColor);
    
    void printscreenWithView(CAView* view, DPoint offset, const CAColor4B& backgroundColor);
    
    void clear(const CAColor4B& backgroundColor);

    void clearDepth(float depthValue);

    void clearStencil(int stencilValue);

    bool saveToFile(const char *szFilePath);

    void listenToBackground(CAObject *obj);

    void listenToForeground(CAObject *obj);

    unsigned int getClearFlags() const;
    void setClearFlags(unsigned int uClearFlags);

    const CAColor4F& getClearColor() const;
    void setClearColor(const CAColor4F &clearColor);

    float getClearDepth() const;
    void setClearDepth(float fClearDepth);

    int getClearStencil() const;
    void setClearStencil(float fClearStencil);

    bool isAutoDraw() const;
    void setAutoDraw(bool bAutoDraw);

    CC_PROPERTY(CAImageView*, m_pImageView, ImageView)
    
protected:
    
    void begin();
    
    void beginWithClear(const CAColor4B& backgroundColor);
    
    void beginWithClear(const CAColor4B& backgroundColor, float depthValue);

    void beginWithClear(const CAColor4B& backgroundColor, float depthValue, int stencilValue);
    
    void beginWithClear(const CAColor4B& backgroundColor, float depthValue, int stencilValue, GLbitfield flags);
    
    void end();
    
    virtual void setContentSize(const DSize& contentSize);
    
protected:
    GLuint       m_uFBO;
    GLuint       m_uDepthRenderBufffer;
    GLint        m_nOldFBO;
    
    unsigned int m_uPixelsWide;
    unsigned int m_uPixelsHigh;
    GLuint m_uName;
    CAImage* m_pImage;

    CAImage::PixelFormat m_ePixelFormat;
    
    // code for "auto" update
    GLbitfield   m_uClearFlags;
    CAColor4F    m_sClearColor;
    GLclampf     m_fClearDepth;
    GLint        m_nClearStencil;
    bool         m_bAutoDraw;
    
    friend class CAImage;
};

// end of textures group
/// @}

NS_CC_END

#endif //__CCRENDER_TEXTURE_H__
