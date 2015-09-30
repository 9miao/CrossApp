
#ifndef __CCRENDER_TEXTURE_H__
#define __CCRENDER_TEXTURE_H__

#include "CAView.h"
#include "CAImageView.h"
#include "kazmath/mat4.h"

NS_CC_BEGIN

/**
 * @addtogroup textures
 * @{
 */

typedef enum eImageFormat
{
    kCCImageFormatJPEG      = 0,
    kCCImageFormatPNG       = 1,
} tCCImageFormat;
/**
@brief CARenderImage is a generic rendering target. To render things into it,
simply construct a render target, call begin on it, call visit on any cocos
scenes or objects to render them, and call end. For convenience, render texture
adds a sprite as it's display child with the results, so you can simply add
the render Image to your scene and treat it like any other CocosNode.
There are also functions for saving the render Image to disk in PNG or JPG format.

@since v0.8.1
*/
class CC_DLL CARenderImage : public CAView 
{
    /** The CAImageView being used.
    The sprite, by default, will use the following blending function: GL_ONE, GL_ONE_MINUS_SRC_ALPHA.
    The blending function can be changed in runtime by calling:
    - [[renderTexture sprite] setBlendFunc:(ccBlendFunc){GL_ONE, GL_ONE_MINUS_SRC_ALPHA}];
    */
    CC_PROPERTY(CAImageView*, m_pImageView, ImageView)
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
