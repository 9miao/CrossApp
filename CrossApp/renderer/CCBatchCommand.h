

#ifndef _CC_BATCHCOMMAND_H_
#define _CC_BATCHCOMMAND_H_

#include "renderer/CCRenderCommand.h"

NS_CC_BEGIN

class CAImageAtlas;
class GLProgram;

class CC_DLL BatchCommand : public RenderCommand
{
public:
    /**Constructor.*/
    BatchCommand();
    /**Destructor.*/
    ~BatchCommand();
    /**Init the batch command.
    @param zOrder GlobalZOrder of the render command.
    @param shader Shader used for draw the texture atlas.
    @param blendType Blend function for texture atlas.
    @param textureAtlas Texture atlas, will contain both quads and texture for rendering.
    @param modelViewTransform Model view transform used for rendering.
    @param flags Indicate the render command should be rendered in 3D mode or not.
    */
    void init(float zOrder, GLProgram* shader, BlendFunc blendType, CAImageAtlas *imageAtlas, const Mat4& modelViewTransform, uint32_t flags);

    /**Execute the command, which will call openGL function to draw the texture atlas.*/
    void execute();

protected:
    //TODO: This member variable is not used. It should be removed.
    int32_t _materialID;
    /**Texture ID used for texture atlas rendering.*/
    GLuint _textureID;
    /**Shaders used for rendering.*/
    GLProgram* _shader;
    /**Blend function for rendering.*/
    BlendFunc _blendType;
    /**Texture atlas for rendering.*/
    CAImageAtlas *_imageAtlas;

    /**ModelView transform.*/
    Mat4 _mv;
};
NS_CC_END

#endif //_CC_BATCHCOMMAND_H_
