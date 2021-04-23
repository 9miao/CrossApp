

#include "renderer/CCPrimitiveCommand.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCRenderer.h"
#include "basics/CAApplication.h"
#include "support/xxhash/xxhash.h"

NS_CC_BEGIN

PrimitiveCommand::PrimitiveCommand()
: _materialID(0)
, _textureID(0)
, _glProgramState(nullptr)
, _blendType(BlendFunc_disable)
, _primitive(nullptr)
{
    _type = RenderCommand::Type::PRIMITIVE_COMMAND;
}

PrimitiveCommand::~PrimitiveCommand()
{
}

void PrimitiveCommand::init(float globalOrder, GLuint textureID, GLProgramState* glProgramState, BlendFunc blendType, Primitive* primitive, const Mat4& mv, uint32_t flags)
{
    CCASSERT(glProgramState, "Invalid GLProgramState");
    CCASSERT(glProgramState->getVertexAttribsFlags() == 0, "No custom attributes are supported in PrimitiveCommand");
    CCASSERT(primitive != nullptr, "Could not render null primitive");
    
    RenderCommand::init(globalOrder, mv, flags);
    
    _primitive = primitive;
    
    _mv = mv;
    
    if( _textureID != textureID || _blendType.src != blendType.src || _blendType.dst != blendType.dst || _glProgramState != glProgramState) {
        
        _textureID = textureID;
        _blendType = blendType;
        _glProgramState = glProgramState;
        
    }
}

void PrimitiveCommand::execute() const
{
    //Set texture
    GL::bindTexture2D(_textureID);
    
    //set blend mode
    GL::blendFunc(_blendType.src, _blendType.dst);
    
    _glProgramState->apply(_mv);
    
    _primitive->draw();
    
    do {
        auto __renderer__ = CAApplication::getApplication()->getRenderer();
        __renderer__->addDrawnBatches(1);
        __renderer__->addDrawnVertices(_primitive->getCount());
    } while(0);
    
}

NS_CC_END
