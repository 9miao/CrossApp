

#include "renderer/CCTrianglesCommand.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramState.h"
#include "support/xxhash/xxhash.h"
#include "renderer/CCRenderer.h"

NS_CC_BEGIN

TrianglesCommand::TrianglesCommand()
:_materialID(0)
,_textureID(0)
,_glProgramState(nullptr)
,_glProgram(nullptr)
,_blendType(BlendFunc_disable)
,_alphaTextureID(0)
{
    _type = RenderCommand::Type::TRIANGLES_COMMAND;
}

void TrianglesCommand::init(float globalOrder, GLuint textureID, GLProgramState* glProgramState, BlendFunc blendType, const Triangles& triangles,const Mat4& mv, uint32_t flags)
{
    CCASSERT(glProgramState, "Invalid GLProgramState");
    CCASSERT(glProgramState->getVertexAttribsFlags() == 0, "No custom attributes are supported in QuadCommand");

    RenderCommand::init(globalOrder, mv, flags);

    _triangles = triangles;
    if(_triangles.indexCount % 3 != 0)
    {
        int count = _triangles.indexCount;
        _triangles.indexCount = count / 3 * 3;
        CCLOG("Resize indexCount from %zd to %zd, size must be multiple times of 3", count, _triangles.indexCount);
    }
    _mv = mv;
    
    if( _textureID != textureID || _blendType.src != blendType.src || _blendType.dst != blendType.dst ||
       _glProgramState != glProgramState ||
       _glProgram != glProgramState->getGLProgram())
    {
        _textureID = textureID;
        _blendType = blendType;
        _glProgramState = glProgramState;
        _glProgram = glProgramState->getGLProgram();
        
        generateMaterialID();
    }
}

TrianglesCommand::~TrianglesCommand()
{
}

void TrianglesCommand::generateMaterialID()
{
    // do not batch if using custom uniforms (since we cannot batch) it
    if(_glProgramState->getUniformCount() > 0)
    {
        _materialID = Renderer::MATERIAL_ID_DO_NOT_BATCH;
        setSkipBatching(true);
    }
    else
    {
        int glProgram = (int)_glProgram->getProgram();
        int intArray[4] = { glProgram, (int)_textureID, (int)_blendType.src, (int)_blendType.dst};
        _materialID = XXH32((const void*)intArray, sizeof(intArray), 0);
    }
}

void TrianglesCommand::useMaterial() const
{
    //Set texture
    GL::bindTexture2D(_textureID);
    
    if (_alphaTextureID > 0)
    { // ANDROID ETC1 ALPHA supports.
        GL::bindTexture2DN(1, _alphaTextureID);
    }
    //set blend mode
    GL::blendFunc(_blendType.src, _blendType.dst);
    
    _glProgramState->apply(_mv);
}

NS_CC_END
