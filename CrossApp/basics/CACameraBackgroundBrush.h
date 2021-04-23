
#ifndef _CCCAMERA_BACKGROUND_BRUSH_H__
#define _CCCAMERA_BACKGROUND_BRUSH_H__

#include "ccTypes.h"
#include "basics/CAObject.h"
#include "renderer/CCQuadCommand.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCFrameBuffer.h"

NS_CC_BEGIN

class CACameraBackgroundColorBrush;
class CACameraBackgroundDepthBrush;
class CACameraBackgroundSkyBoxBrush;

class GLProgramState;
class CACamera;

/**
 * Defines a brush to clear the background of camera.
 * There are 4 types of brush. None brush do nothing, Depth brush clear background with given depth, Color brush clear background with given color and depth, Skybox brush clear the background with a skybox. Camera uses depth brush by default.
 */
class CC_DLL CACameraBackgroundBrush : public CAObject
{
public:
    /**
     * Brush types. There are 4 types of brush. See CACameraBackgroundDepthBrush, CACameraBackgroundColorBrush, CACameraBackgroundSkyBoxBrush for more information.
     */
    enum class BrushType
    {
        NONE, //none brush
        DEPTH, // depth brush. See CACameraBackgroundDepthBrush
        COLOR, // color brush. See CACameraBackgroundColorBrush
        SKYBOX, // skybox brush. See CACameraBackgroundSkyBoxBrush
    };
    
    /**
     * get brush type
     * @return BrushType
     */
    virtual BrushType getBrushType() const { return BrushType::NONE; }
    
    /**
     * Creates a none brush, it does nothing when clear the background
     * @return Created brush.
     */
    static CACameraBackgroundBrush* createNoneBrush();
    
    /**
     * Creates a depth brush, which clears depth buffer with a given depth.
     * @param depth Depth used to clear depth buffer
     * @return Created brush
     */
    static CACameraBackgroundDepthBrush* createDepthBrush(float depth = 1.f);
    
    /**
     * Creates a color brush
     * @param color Color of brush
     * @param depth Depth used to clear depth buffer
     * @return Created brush
     */
    static CACameraBackgroundColorBrush* createColorBrush(const CAColor4F& color, float depth);
    
    
    /** Creates a Skybox brush with 6 textures.
     @param positive_x texture for the right side of the texture cube face.
     @param negative_x texture for the up side of the texture cube face.
     @param positive_y texture for the top side of the texture cube face
     @param negative_y texture for the bottom side of the texture cube face
     @param positive_z texture for the forward side of the texture cube face.
     @param negative_z texture for the rear side of the texture cube face.
     @return  A new brush inited with given parameters.
     */
    static CACameraBackgroundSkyBoxBrush* createSkyboxBrush(const std::string& positive_x, const std::string& negative_x,
                                                          const std::string& positive_y, const std::string& negative_y,
                                                          const std::string& positive_z, const std::string& negative_z);
    /**
     * draw the background
     */
    virtual void drawBackground(CACamera* camera) {}

    virtual bool isValid() { return true; }

    CACameraBackgroundBrush();
    virtual ~CACameraBackgroundBrush();

    virtual bool init() { return true; }
    
protected:
    GLProgramState* _glProgramState;
};

/**
 * Depth brush clear depth buffer with given depth
 */
class CC_DLL CACameraBackgroundDepthBrush : public CACameraBackgroundBrush
{
public:
    /**
     * Create a depth brush
     * @param depth Depth used to clear the depth buffer
     * @return Created brush
     */
    static CACameraBackgroundDepthBrush* create(float depth);
    
    /**
     * Get brush type. Should be BrushType::DEPTH
     * @return brush type
     */
    virtual BrushType getBrushType() const override { return BrushType::DEPTH; }
    
    /**
     * Draw background
     */
    virtual void drawBackground(CACamera* camera) override;
    
    /**
     * Set depth
     * @param depth Depth used to clear depth buffer
     */
    void setDepth(float depth) { _depth = depth; }
    
    CACameraBackgroundDepthBrush();
    virtual ~CACameraBackgroundDepthBrush();

    virtual bool init() override;
    
protected:
    float _depth;
    
    GLboolean _clearColor;
    
    ccV3F_C4B_T2F_Quad _quad;
};

/**
 * Color brush clear buffer with given depth and color
 */
class CC_DLL CACameraBackgroundColorBrush : public CACameraBackgroundDepthBrush
{
public:
    /**
     * Get brush type. Should be BrushType::COLOR
     * @return brush type
     */
    virtual BrushType getBrushType() const override { return BrushType::COLOR; }
    
    /**
     * Create a color brush
     * @param color Color used to clear the color buffer
     * @param depth Depth used to clear the depth buffer
     * @return Created brush
     */
    static CACameraBackgroundColorBrush* create(const CAColor4F& color, float depth);
    
    /**
     * Set clear color
     * @param color Color used to clear the color buffer
     */
    void setColor(const CAColor4F& color);
    
    CACameraBackgroundColorBrush();
    virtual ~CACameraBackgroundColorBrush();

    virtual bool init() override;
    
protected:
    CAColor4F _color;
};

class CAImageCube;
class GLProgramState;

/**
 * Skybox brush clear buffer with a skybox
 */
class CC_DLL CACameraBackgroundSkyBoxBrush : public CACameraBackgroundBrush
{
public:
    /**
     * Get brush type. Should be BrushType::SKYBOX
     * @return brush type
     */
    virtual BrushType getBrushType() const override { return BrushType::SKYBOX; }
    
    /** Creates a Skybox brush with 6 textures.
     @param positive_x texture for the right side of the texture cube face.
     @param negative_x texture for the up side of the texture cube face.
     @param positive_y texture for the top side of the texture cube face
     @param negative_y texture for the bottom side of the texture cube face
     @param positive_z texture for the forward side of the texture cube face.
     @param negative_z texture for the rear side of the texture cube face.
     @return  A new brush inited with given parameters.
     */
    static CACameraBackgroundSkyBoxBrush* create(const std::string& positive_x, const std::string& negative_x,
                                        const std::string& positive_y, const std::string& negative_y,
                                        const std::string& positive_z, const std::string& negative_z);
    
    /** Creates a Skybox brush with 6 textures.
     */
    static CACameraBackgroundSkyBoxBrush* create();
    /**
     * Set skybox texture 
     * @param texture Skybox texture
     */
    void setImage(CAImageCube*  image);
    
    /**
     * Draw background
     */
    virtual void drawBackground(CACamera* camera) override;

    bool isActived() const;
    void setActived(bool actived);
    virtual void setTextureValid(bool valid);
    virtual bool isValid()override;

    CACameraBackgroundSkyBoxBrush();
    virtual ~CACameraBackgroundSkyBoxBrush();
    
    /**
     * init Skybox.
     */
    virtual bool init() override;
    
protected:
    void initBuffer();
    
    GLuint      _vao;
    GLuint      _vertexBuffer;
    GLuint      _indexBuffer;
    
    CAImageCube*  _image;

private:
    bool _actived;
    bool _textureValid;
};

NS_CC_END

#endif// _CCCAMERA_BACKGROUND_BRUSH_H__
