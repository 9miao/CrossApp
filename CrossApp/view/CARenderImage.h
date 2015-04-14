
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
    CC_PROPERTY(CAImageView*, m_pSprite, Sprite)
public:
    /**
     * @js ctor
     */
    CARenderImage();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CARenderImage();
    
    virtual void visit();
    virtual void draw();

    /** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
    static CARenderImage * create(int w ,int h, CAImage::PixelFormat eFormat, GLuint uDepthStencilFormat);

    /** creates a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
    static CARenderImage * create(int w, int h, CAImage::PixelFormat eFormat);

    /** creates a RenderTexture object with width and height in Points, pixel format is RGBA8888 */
    static CARenderImage * create(int w, int h);

    /** initializes a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
    bool initWithWidthAndHeight(int w, int h, CAImage::PixelFormat eFormat);

    /** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
    bool initWithWidthAndHeight(int w, int h, CAImage::PixelFormat eFormat, GLuint uDepthStencilFormat);

    /** starts grabbing */
    void begin();

    /** starts rendering to the Image while clearing the Image first.
    This is more efficient then calling -clear first and then -begin */
    void beginWithClear(float r, float g, float b, float a);

    /** starts rendering to the Image while clearing the Image first.
     This is more efficient then calling -clear first and then -begin */
    void beginWithClear(float r, float g, float b, float a, float depthValue);

    /** starts rendering to the Image while clearing the Image first.
     This is more efficient then calling -clear first and then -begin */
    void beginWithClear(float r, float g, float b, float a, float depthValue, int stencilValue);

    /** end is key word of lua, use other name to export to lua. */
    inline void endToLua(){ end();};

    /** ends grabbing*/
    void end();

    /** clears the Image with a color */
    void clear(float r, float g, float b, float a);

    /** clears the Image with a specified depth value */
    void clearDepth(float depthValue);

    /** clears the Image with a specified stencil value */
    void clearStencil(int stencilValue);

    /** saves the Image into a file using JPEG format. The file will be saved in the Documents folder.
        Returns YES if the operation is successful.
     */
    bool saveToFile(const char *szFilePath);

    /** Listen "come to background" message, and save render texture.
     It only has effect on Android.
     */
    void listenToBackground(CAObject *obj);
    
    /** Listen "come to foreground" message and restore the frame buffer object
     It only has effect on Android.
     */
    void listenToForeground(CAObject *obj);
    
    /** Valid flags: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT. They can be OR'ed. Valid when "autoDraw is YES. */
    unsigned int getClearFlags() const;
    void setClearFlags(unsigned int uClearFlags);
    
    /** Clear color value. Valid only when "autoDraw" is true. */
    const CAColor4F& getClearColor() const;
    void setClearColor(const CAColor4F &clearColor);
    
    /** Value for clearDepth. Valid only when autoDraw is true. */
    float getClearDepth() const;
    void setClearDepth(float fClearDepth);
    
    /** Value for clear Stencil. Valid only when autoDraw is true */
    int getClearStencil() const;
    void setClearStencil(float fClearStencil);
    
    /** When enabled, it will render its children into the Image automatically. Disabled by default for compatiblity reasons.
     Will be enabled in the future.
     */
    bool isAutoDraw() const;
    void setAutoDraw(bool bAutoDraw);

private:
    void beginWithClear(float r, float g, float b, float a, float depthValue, int stencilValue, GLbitfield flags);

protected:
    GLuint       m_uFBO;
    GLuint       m_uDepthRenderBufffer;
    GLint        m_nOldFBO;
    CAImage* m_pImage;
    CAImage* m_pImageCopy;    // a copy of m_pImage

    GLenum       m_ePixelFormat;
    
    // code for "auto" update
    GLbitfield   m_uClearFlags;
    CAColor4F    m_sClearColor;
    GLclampf     m_fClearDepth;
    GLint        m_nClearStencil;
    bool         m_bAutoDraw;
};

// end of textures group
/// @}

NS_CC_END

#endif //__CCRENDER_TEXTURE_H__
