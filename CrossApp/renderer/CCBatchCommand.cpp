
#include "renderer/CCBatchCommand.h"
#include "renderer/ccGLStateCache.h"
#include "images/CAImageCache.h"
#include "images/CAImage.h"
#include "renderer/CCGLProgram.h"

NS_CC_BEGIN

BatchCommand::BatchCommand()
: _textureID(0)
, _blendType(BlendFunc_disable)
, _imageAtlas(nullptr)
{
    _type = RenderCommand::Type::BATCH_COMMAND;
    _shader = nullptr;
}

void BatchCommand::init(float zOrder, GLProgram* shader, BlendFunc blendType, CAImageAtlas *imageAtlas, const Mat4& modelViewTransform, uint32_t flags)
{
    CCASSERT(shader, "shader cannot be null");
    CCASSERT(imageAtlas, "textureAtlas cannot be null");
    
    RenderCommand::init(zOrder, modelViewTransform, flags);
    _textureID = imageAtlas->getImage()->getName();
    _blendType = blendType;
    _shader = shader;
    
    _imageAtlas = imageAtlas;
    
    _mv = modelViewTransform;
}

BatchCommand::~BatchCommand()
{
}

void BatchCommand::execute()
{
    // Set material
    _shader->use();
    _shader->setUniformsForBuiltins(_mv);
    GL::bindTexture2D(_textureID);
    GL::blendFunc(_blendType.src, _blendType.dst);

    // Draw
    _imageAtlas->drawQuads();
}

NS_CC_END
