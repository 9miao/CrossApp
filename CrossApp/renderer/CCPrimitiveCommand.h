

#ifndef _CC_PRIMITIVE_COMMAND_H__
#define _CC_PRIMITIVE_COMMAND_H__

#include "renderer/CCPrimitive.h"
#include "renderer/CCRenderCommand.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_CC_BEGIN
class GLProgramState;
/**
 Command used to render primitive, similar to QuadCommand.
 Every QuadCommand will have generate material ID by give textureID, glProgramState, Blend function. 
 However, primitive command could not be batched.
 */
class CC_DLL PrimitiveCommand : public RenderCommand
{
public:
    /**@{
     Constructor and Destructor.
     */
    PrimitiveCommand();
    ~PrimitiveCommand();
    
    /**@}*/
    
    /** Initializes the command.
     @param globalOrder GlobalZOrder of the command.
     @param textureID The openGL handle of the used texture.
     @param glProgramState The specified glProgram and its uniform.
     @param blendType Blend function for the command.
     @param primitive Rendered primitive for the command.
     @param mv ModelView matrix for the command.
     @param flags to indicate that the command is using 3D rendering or not.
     */
    void init(float globalOrder, GLuint textureID, GLProgramState* glProgramState, BlendFunc blendType, Primitive* primitive, const Mat4& mv, uint32_t flags);

    
    /**Get the generated material ID.*/
    inline uint32_t getMaterialID() const { return _materialID; }
    /**Get the texture ID used for drawing.*/
    inline GLuint getTextureID() const { return _textureID; }
    /**Get the glprogramstate used for drawing.*/
    inline GLProgramState* getGLProgramState() const { return _glProgramState; }
    /**Get the blend function for drawing.*/
    inline BlendFunc getBlendType() const { return _blendType; }
    /**Get the modelview matrix when draw the primitive.*/
    inline const Mat4& getModelView() const { return _mv; }
    /**Execute and draw the command, called by renderer.*/
    void execute() const;
protected:
    
    uint32_t _materialID;
    GLuint _textureID;
    GLProgramState* _glProgramState;
    BlendFunc _blendType;
    Primitive* _primitive;
    Mat4 _mv;
};

NS_CC_END

/**
 end of support group
 @}
 */
#endif //_CC_PRIMITIVE_COMMAND_H__
