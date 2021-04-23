
#ifndef _CC_QUADCOMMAND_H_
#define _CC_QUADCOMMAND_H_

#include <vector>
#include "renderer/CCTrianglesCommand.h"
#include "renderer/CCGLProgramState.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_CC_BEGIN

/** 
 Command used to render one or more Quads, similar to TrianglesCommand.
 Every QuadCommand will have generate material ID by give textureID, glProgramState, Blend function
 if the material id is the same, these QuadCommands could be batched to save draw call.
 */
class CC_DLL QuadCommand : public TrianglesCommand
{
public:
    /**Constructor.*/
    QuadCommand();
    /**Destructor.*/
    ~QuadCommand();
    
    /** Initializes the command.
     @param globalOrder GlobalZOrder of the command.
     @param textureID The openGL handle of the used texture.
     @param glProgramState The glProgram with its uniform.
     @param blendType Blend function for the command.
     @param quads Rendered quads for the command.
     @param quadCount The number of quads when rendering.
     @param mv ModelView matrix for the command.
     @param flags to indicate that the command is using 3D rendering or not.
     */
    void init(float globalOrder, GLuint textureID, GLProgramState* glProgramState, const BlendFunc& blendType, ccV3F_C4B_T2F_Quad* quads, ssize_t quadCount,
              const Mat4& mv, uint32_t flags);

protected:
    void reIndex(int indices);

    int _indexSize;
    std::vector<GLushort*> _ownedIndices;

    // shared across all instances
    static int __indexCapacity;
    static GLushort* __indices;
};

NS_CC_END

/**
 end of support group
 @}
 */
#endif //_CC_QUADCOMMAND_H_
