
#include "basics/CACameraBackgroundBrush.h"
#include "basics/CACamera.h"
#include "basics/CANotificationCenter.h"
#include "ccMacros.h"
#include "basics/CAConfiguration.h"
#include "basics/CAApplication.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCRenderState.h"
#include "images/CAImageCube.h"


NS_CC_BEGIN

CACameraBackgroundBrush::CACameraBackgroundBrush()
: _glProgramState(nullptr)
{
    
}

CACameraBackgroundBrush::~CACameraBackgroundBrush()
{
    CC_SAFE_RELEASE(_glProgramState);
}

CACameraBackgroundBrush* CACameraBackgroundBrush::createNoneBrush()
{
    auto ret = new (std::nothrow) CACameraBackgroundBrush();
    ret->init();
    
    ret->autorelease();
    return ret;
}

CACameraBackgroundColorBrush* CACameraBackgroundBrush::createColorBrush(const CAColor4F& color, float depth)
{
    return CACameraBackgroundColorBrush::create(color, depth);
}

CACameraBackgroundDepthBrush* CACameraBackgroundBrush::createDepthBrush(float depth)
{
    return CACameraBackgroundDepthBrush::create(depth);
}

CACameraBackgroundSkyBoxBrush* CACameraBackgroundBrush::createSkyboxBrush(const std::string& positive_x, const std::string& negative_x, const std::string& positive_y, const std::string& negative_y, const std::string& positive_z, const std::string& negative_z)
{
    return CACameraBackgroundSkyBoxBrush::create(positive_x, negative_x, positive_y, negative_y, positive_z, negative_z);
}

//////////////////////////////////////////////////////////////////////////////////////////

CACameraBackgroundDepthBrush::CACameraBackgroundDepthBrush()
: _depth(0.f)
, _clearColor(GL_FALSE)
{
    
}
CACameraBackgroundDepthBrush::~CACameraBackgroundDepthBrush()
{
    
}

CACameraBackgroundDepthBrush* CACameraBackgroundDepthBrush::create(float depth)
{
    auto ret = new (std::nothrow) CACameraBackgroundDepthBrush();
    ret->_depth = depth;
    ret->init();
    ret->autorelease();
    return ret;
}

bool CACameraBackgroundDepthBrush::init()
{
    auto shader = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_CAMERA_CLEAR);
    _glProgramState = GLProgramState::getOrCreateWithGLProgram(shader);
    _glProgramState->retain();
    
    _quad.bl.vertices = DPoint3D(-1,-1,0);
    _quad.br.vertices = DPoint3D(1,-1,0);
    _quad.tl.vertices = DPoint3D(-1,1,0);
    _quad.tr.vertices = DPoint3D(1,1,0);
    
    _quad.bl.colors = _quad.br.colors = _quad.tl.colors = _quad.tr.colors = CAColor4B(0,0,0,1);
    
    _quad.bl.texCoords = tex2(0,0);
    _quad.br.texCoords = tex2(1,0);
    _quad.tl.texCoords = tex2(0,1);
    _quad.tr.texCoords = tex2(1,1);
    return true;
}

void CACameraBackgroundDepthBrush::drawBackground(CACamera* camera)
{
    GLboolean oldDepthTest;
    GLint oldDepthFunc;
    GLboolean oldDepthMask;
    {
        glColorMask(_clearColor, _clearColor, _clearColor, _clearColor);
        glStencilMask(0);
        
        oldDepthTest = glIsEnabled(GL_DEPTH_TEST);
        glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
        glGetBooleanv(GL_DEPTH_WRITEMASK, &oldDepthMask);
        
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
    }
    
    //draw
    
    _glProgramState->setUniformFloat("depth", _depth);
    _glProgramState->apply(Mat4::IDENTITY);
    GLshort indices[6] = {0, 1, 2, 3, 2, 1};
    
    {
        GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        
        // vertices
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ccV3F_C4B_T2F), &_quad.tl.vertices);
        
        // colors
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV3F_C4B_T2F), &_quad.tl.colors);
        
        // tex coords
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(ccV3F_C4B_T2F), &_quad.tl.texCoords);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    }
    
    
    {
        if(GL_FALSE == oldDepthTest)
        {
            glDisable(GL_DEPTH_TEST);
        }
        glDepthFunc(oldDepthFunc);
        
        if(GL_FALSE == oldDepthMask)
        {
            glDepthMask(GL_FALSE);
        }
        
        /* IMPORTANT: We only need to update the states that are not restored.
         Since we don't know what was the previous value of the mask, we update the RenderState
         after setting it.
         The other values don't need to be updated since they were restored to their original values
         */
        glStencilMask(0xFFFFF);
        //        RenderState::StateBlock::_defaultState->setStencilWrite(0xFFFFF);
        
        /* BUG: RenderState does not support glColorMask yet. */
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

CACameraBackgroundColorBrush::CACameraBackgroundColorBrush()
: _color(CAColor4F(0.f, 0.f, 0.f, 0.f))
{
    
}

CACameraBackgroundColorBrush::~CACameraBackgroundColorBrush()
{
    
}

bool CACameraBackgroundColorBrush::init()
{
    CACameraBackgroundDepthBrush::init();
    this->_clearColor = GL_TRUE;
    return true;
}

void CACameraBackgroundColorBrush::setColor(const CAColor4F& color)
{
    _quad.bl.colors = _quad.br.colors = _quad.tl.colors = _quad.tr.colors = CAColor4B(color);
}

CACameraBackgroundColorBrush* CACameraBackgroundColorBrush::create(const CAColor4F& color, float depth)
{
    auto ret = new (std::nothrow) CACameraBackgroundColorBrush();
    ret->init();
    ret->setColor(color);
    ret->setDepth(depth);
    
    ret->autorelease();
    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CACameraBackgroundSkyBoxBrush::CACameraBackgroundSkyBoxBrush()
: _vao(0)
, _vertexBuffer(0)
, _indexBuffer(0)
, _image(nullptr)
, _actived(true)
, _textureValid(true)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    CANotificationCenter::getInstance()->addObserver([this](CAObject* obj)
    {
        initBuffer();
    }, this, EVENT_COME_TO_FOREGROUND);
#endif
}

CACameraBackgroundSkyBoxBrush::~CACameraBackgroundSkyBoxBrush()
{
    CC_SAFE_RELEASE(_image);
    
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_indexBuffer);
    
    _vertexBuffer = 0;
    _indexBuffer = 0;
    
#if CC_TEXTURE_ATLAS_USE_VAO
    glDeleteVertexArrays(1, &_vao);
    GL::bindVAO(0);
    _vao = 0;
#endif

}

CACameraBackgroundSkyBoxBrush* CACameraBackgroundSkyBoxBrush::create(const std::string& positive_x, const std::string& negative_x, const std::string& positive_y, const std::string& negative_y, const std::string& positive_z, const std::string& negative_z)
{
    auto image = CAImageCube::create(positive_x, negative_x, positive_y, negative_y, positive_z, negative_z);
    if (image == nullptr)
        return nullptr;
    
    CAImage::TexParams tRepeatParams;
    tRepeatParams.magFilter = GL_LINEAR;
    tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_CLAMP_TO_EDGE;
    tRepeatParams.wrapT = GL_CLAMP_TO_EDGE;
    image->setTexParameters(tRepeatParams);
    
    auto ret = new(std::nothrow)CACameraBackgroundSkyBoxBrush();
    
    ret->init();
    ret->setImage(image);
    
    ret->autorelease();
    return ret;
}

CACameraBackgroundSkyBoxBrush* CACameraBackgroundSkyBoxBrush::create()
{
    auto ret = new(std::nothrow)CACameraBackgroundSkyBoxBrush();
    
    ret->init();
    
    ret->autorelease();
    return ret;
}

void CACameraBackgroundSkyBoxBrush::drawBackground(CACamera* camera)
{
    if (!_actived)
        return;

    Mat4 cameraModelMat = camera->getViewToWorldTransform();
    
    Vec4 color(1.f, 1.f, 1.f, 1.f);
    _glProgramState->setUniformVec4("u_color", color);
    cameraModelMat.m[12] = cameraModelMat.m[13] = cameraModelMat.m[14] = 0;
    _glProgramState->setUniformMat4("u_cameraRot", cameraModelMat);
    
    _glProgramState->apply(Mat4::IDENTITY);
    
    glEnable(GL_DEPTH_TEST);
    RenderState::StateBlock::_defaultState->setDepthTest(true);
    
    glDepthMask(GL_TRUE);
    RenderState::StateBlock::_defaultState->setDepthWrite(true);
    
    glDepthFunc(GL_ALWAYS);
    RenderState::StateBlock::_defaultState->setDepthFunction(RenderState::DEPTH_ALWAYS);
    
    glEnable(GL_CULL_FACE);
    RenderState::StateBlock::_defaultState->setCullFace(true);
    
    glCullFace(GL_BACK);
    RenderState::StateBlock::_defaultState->setCullFaceSide(RenderState::CULL_FACE_SIDE_BACK);
    
    glDisable(GL_BLEND);
    RenderState::StateBlock::_defaultState->setBlend(false);
    
    
#if CC_TEXTURE_ATLAS_USE_VAO
    GL::bindVAO(_vao);
#else
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), nullptr);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
#endif
    
    glDrawElements(GL_TRIANGLES, (GLsizei)36, GL_UNSIGNED_BYTE, nullptr);
    
    
#if CC_TEXTURE_ATLAS_USE_VAO
    GL::bindVAO(0);
#else
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 8);
    
    CHECK_GL_ERROR_DEBUG();
}

bool CACameraBackgroundSkyBoxBrush::init()
{
    auto shader = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_3D_SKYBOX);
    _glProgramState = GLProgramState::create(shader);
    _glProgramState->setVertexAttribPointer(GLProgram::ATTRIBUTE_NAME_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), nullptr);
    _glProgramState->retain();
    
    initBuffer();
    
    return true;
}

void CACameraBackgroundSkyBoxBrush::initBuffer()
{
    if (_vertexBuffer)
        glDeleteBuffers(1, &_vertexBuffer);
    if (_indexBuffer)
        glDeleteBuffers(1, &_indexBuffer);
    
#if CC_TEXTURE_ATLAS_USE_VAO
    if (_vao)
    {
        glDeleteVertexArrays(1, &_vao);
        GL::bindVAO(0);
        _vao = 0;
    }
    
    glGenVertexArrays(1, &_vao);
    GL::bindVAO(_vao);
    
#endif
    
    // init vertex buffer object
    Vec3 vexBuf[] =
    {
        DPoint3D(1, -1, 1),  DPoint3D(1, 1, 1),  DPoint3D(-1, 1, 1),  DPoint3D(-1, -1, 1),
        DPoint3D(1, -1, -1), DPoint3D(1, 1, -1), DPoint3D(-1, 1, -1), DPoint3D(-1, -1, -1)
    };
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vexBuf), vexBuf, GL_STATIC_DRAW);
    
    // init index buffer object
    const unsigned char idxBuf[] = {  2, 1, 0, 3, 2, 0, // font
        1, 5, 4, 1, 4, 0, // right
        4, 5, 6, 4, 6, 7, // back
        7, 6, 2, 7, 2, 3, // left
        2, 6, 5, 2, 5, 1, // up
        3, 0, 4, 3, 4, 7  // down
    };
    
    glGenBuffers(1, &_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxBuf), idxBuf, GL_STATIC_DRAW);
    
#if CC_TEXTURE_ATLAS_USE_VAO
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
    _glProgramState->applyAttributes(false);
    GL::bindVAO(0);
#endif
}

void CACameraBackgroundSkyBoxBrush::setImage(CrossApp::CAImageCube *image)
{
    CC_SAFE_RETAIN(image);
    CC_SAFE_RELEASE(_image);
    _image = image;
    _glProgramState->setUniformTexture("u_Env", _image->getName());
}

bool CACameraBackgroundSkyBoxBrush::isActived() const
{
    return _actived;
}
void CACameraBackgroundSkyBoxBrush::setActived(bool actived)
{
    _actived = actived;
}

void CACameraBackgroundSkyBoxBrush::setTextureValid(bool valid)
{
    _textureValid = valid;
}

bool CACameraBackgroundSkyBoxBrush::isValid()
{
    return _actived;
}

NS_CC_END
