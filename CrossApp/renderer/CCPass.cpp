

#include "renderer/CCPass.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgram.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCTechnique.h"
#include "renderer/CCMaterial.h"
#include "images/CAImage.h"
#include "ccTypes.h"
#include "view/CAView.h"
#include "support/xxhash/xxhash.h"

NS_CC_BEGIN


Pass* Pass::create(Technique* technique)
{
    auto pass = new (std::nothrow) Pass();
    if (pass && pass->init(technique))
    {
        pass->autorelease();
        return pass;
    }
    CC_SAFE_DELETE(pass);
    return nullptr;
}

Pass* Pass::createWithGLProgramState(Technique* technique, GLProgramState* programState)
{
    auto pass = new (std::nothrow) Pass();
    if (pass && pass->initWithGLProgramState(technique, programState))
    {
        pass->autorelease();
        return pass;
    }
    CC_SAFE_DELETE(pass);
    return nullptr;
}

bool Pass::init(Technique* technique)
{
    _parent = technique;
    return true;
}

bool Pass::initWithGLProgramState(Technique* technique, GLProgramState *glProgramState)
{
    _parent = technique;
    _glProgramState = glProgramState;
    CC_SAFE_RETAIN(_glProgramState);
    return true;
}

Pass::Pass()
: _glProgramState(nullptr)
{
}

Pass::~Pass()
{
    CC_SAFE_RELEASE(_glProgramState);
}

Pass* Pass::clone() const
{
    auto pass = new (std::nothrow) Pass();
    if (pass)
    {
        RenderState::cloneInto(pass);
        pass->_glProgramState = _glProgramState->clone();
        CC_SAFE_RETAIN(pass->_glProgramState);

        pass->autorelease();
    }
    return pass;
}

GLProgramState* Pass::getGLProgramState() const
{
    return _glProgramState;
}

void Pass::setGLProgramState(GLProgramState* glProgramState)
{
    if ( _glProgramState != glProgramState) {
        CC_SAFE_RELEASE(_glProgramState);
        _glProgramState = glProgramState;
        CC_SAFE_RETAIN(_glProgramState);

        _hashDirty = true;
    }
}

uint32_t Pass::getHash() const
{
    if (_hashDirty || _state->isDirty()) {
        uint32_t glProgram = (uint32_t)_glProgramState->getGLProgram()->getProgram();
        uint32_t textureid = _image ? _image->getName() : -1;
        uint32_t stateblockid = _state->getHash();

        _hash = glProgram ^ textureid ^ stateblockid;

//        _hash = XXH32((const void*)intArray, sizeof(intArray), 0);
        _hashDirty = false;
    }

    return _hash;
}

void Pass::bind(const Mat4& modelView)
{
    bind(modelView, true);
}

void Pass::bind(const Mat4& modelView, bool bindAttributes)
{

    GLProgramState* glprogramstate = nullptr;
    if (_glProgramState)
    {
        glprogramstate = _glProgramState;
    }
    
    if (CAView* v = dynamic_cast<CAView*>((CAObject*)this->getTarget()))
    {
        glprogramstate = v->getGLProgramState();
    }

    glprogramstate->applyGLProgram(modelView);
    glprogramstate->applyUniforms();

    //set render state
    RenderState::bind(this);

}

void* Pass::getTarget() const
{
    CCASSERT(_parent && _parent->_parent, "Pass must have a Technique and Material");

    Material *material = static_cast<Material*>(_parent->_parent);
    return material->_target;
}

void Pass::unbind()
{
    RenderState::StateBlock::restore(0);

}


NS_CC_END
