
#ifndef __CCRENDER_TEXTURE_H__
#define __CCRENDER_TEXTURE_H__

#include "CAView.h"
#include "CAImageView.h"
#include "renderer/CCGroupCommand.h"
#include "renderer/CCCustomCommand.h"

NS_CC_BEGIN

class CC_DLL CARenderImage : public CAView
{
public:

    CARenderImage();

    virtual ~CARenderImage();
    
    virtual void visitEve() override;
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

    static CARenderImage * create(int w ,int h, CAImage::PixelFormat eFormat, GLuint uDepthStencilFormat);

    static CARenderImage * create(int w, int h, CAImage::PixelFormat eFormat);

    static CARenderImage * create(int w, int h);

    static CARenderImage* printscreen();

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
    
    virtual void setContentSize(const DSize& contentSize) override;

    float to_px(float dip);
    
protected:

    GroupCommand  m_obGroupCommand;
    CustomCommand m_obBeginWithClearCommand;
    CustomCommand m_tClearDepthCommand;
    CustomCommand m_obClearCommand;
    CustomCommand m_obBeginCommand;
    CustomCommand m_obEndCommand;

    CustomCommand m_obSaveToFileCommand;
    std::function<void(CARenderImage*, const std::string&)> m_fSaveFileCallback;
    
    
    GLuint       m_uFBO;
    GLuint       m_uDepthRenderBufffer;
    GLuint       m_uStencilRenderBufffer;
    GLint        m_uOldFBO;
    
    unsigned long m_uPixelsWide;
    unsigned long m_uPixelsHigh;
    GLuint m_uName;
    GLuint m_uNameCopy;

    CAImage::PixelFormat m_ePixelFormat;
    
    // code for "auto" update
    GLbitfield   m_uClearFlags;
    CAColor4F    m_sClearColor;
    GLclampf     m_fClearDepth;
    GLint        m_nClearStencil;
    bool         m_bAutoDraw;
    bool         m_bInTheScreenshot;
    
    
    friend class CAImage;
    
protected:
    //renderer caches and callbacks
    void onBegin();
    void onEnd();
    
    void onClear();
    void onClearDepth();
    
    void onSaveToFile(const std::string& fileName, bool isRGBA = true);
    
    Mat4 m_tOldTransMatrix, m_tOldProjMatrix;
    Mat4 m_tTransformMatrix, m_tProjectionMatrix;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(CARenderImage);
};

// end of textures group
/// @}

NS_CC_END

#endif //__CCRENDER_TEXTURE_H__
