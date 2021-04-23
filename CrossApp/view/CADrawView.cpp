

#include "CADrawView.h"
#include "basics/CAConfiguration.h"
#include "basics/CAApplication.h"
#include "basics/CANotificationCenter.h"
#include "basics/CAPointExtension.h"
#include "renderer/CCRenderer.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgramCache.h"
#include "ccTypes.h"
#include "CCGL.h"
#include "game/actions/CGActionCatmullRom.h"
#include "view/CADrawingPrimitives.h"

NS_CC_BEGIN

// DPoint == CGPoint in 32-bits, but not in 64-bits (OS X)
// that's why the "v2f" functions are needed
static DPoint v2fzero = {0.0f,0.0f};

static inline DPoint v2f(float x, float y)
{
    DPoint ret = {x, y};
	return ret;
}

static inline DPoint v2fadd(const DPoint &v0, const DPoint &v1)
{
	return v2f(v0.x+v1.x, v0.y+v1.y);
}

static inline DPoint v2fsub(const DPoint &v0, const DPoint &v1)
{
	return v2f(v0.x-v1.x, v0.y-v1.y);
}

static inline DPoint v2fmult(const DPoint &v, float s)
{
	return v2f(v.x * s, v.y * s);
}

static inline DPoint v2fperp(const DPoint &p0)
{
	return v2f(-p0.y, p0.x);
}

static inline DPoint v2fneg(const DPoint &p0)
{
	return v2f(-p0.x, - p0.y);
}

static inline float v2fdot(const DPoint &p0, const DPoint &p1)
{
	return  p0.x * p1.x + p0.y * p1.y;
}

static inline DPoint v2fforangle(float _a_)
{
	return v2f(cosf(_a_), sinf(_a_));
}

static inline DPoint v2fnormalize(const DPoint &p)
{
	DPoint r = ccpNormalize(DPoint(p.x, p.y));
	return v2f(r.x, r.y);
}

static inline DPoint __v2f(const DPoint &v)
{
//#ifdef __LP64__
	return v2f(v.x, v.y);
// #else
// 	return * ((DPoint*) &v);
// #endif
}

static inline ccTex2F __t(const DPoint &v)
{
	return *(ccTex2F*)&v;
}

CADrawView::CADrawView(int lineWidth)
: _vao(0)
, _vbo(0)
, _vaoGLPoint(0)
, _vboGLPoint(0)
, _vaoGLLine(0)
, _vboGLLine(0)
, _bufferCapacity(0)
, _bufferCount(0)
, _buffer(nullptr)
, _bufferCapacityGLPoint(0)
, _bufferCountGLPoint(0)
, _bufferGLPoint(nullptr)
, _bufferCapacityGLLine(0)
, _bufferCountGLLine(0)
, _bufferGLLine(nullptr)
, _dirty(false)
, _dirtyGLPoint(false)
, _dirtyGLLine(false)
, _lineWidth(lineWidth)
, _defaultLineWidth(lineWidth)
{
    _blendFunc = BlendFunc_alpha_premultiplied;
}

CADrawView::~CADrawView()
{
    free(_buffer);
    _buffer = nullptr;
    free(_bufferGLPoint);
    _bufferGLPoint = nullptr;
    free(_bufferGLLine);
    _bufferGLLine = nullptr;
    
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_vboGLLine);
    glDeleteBuffers(1, &_vboGLPoint);
    _vbo = 0;
    _vboGLPoint = 0;
    _vboGLLine = 0;
    
    if (CAConfiguration::getInstance()->supportsShareableVAO())
    {
#if CC_TEXTURE_ATLAS_USE_VAO
        GL::bindVAO(0);
        glDeleteVertexArrays(1, &_vao);
        glDeleteVertexArrays(1, &_vaoGLLine);
        glDeleteVertexArrays(1, &_vaoGLPoint);
        _vao = _vaoGLLine = _vaoGLPoint = 0;
#endif
    }
}

CADrawView* CADrawView::create(int defaultLineWidth)
{
    CADrawView* ret = new (std::nothrow) CADrawView(defaultLineWidth);
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

void CADrawView::ensureCapacity(int count)
{
    CCASSERT(count>=0, "capacity must be >= 0");
    
    if(_bufferCount + count > _bufferCapacity)
    {
        _bufferCapacity += MAX(_bufferCapacity, count);
        _buffer = (ccV2F_C4B_T2F*)realloc(_buffer, _bufferCapacity*sizeof(ccV2F_C4B_T2F));
    }
}

void CADrawView::ensureCapacityGLPoint(int count)
{
    CCASSERT(count>=0, "capacity must be >= 0");
    
    if(_bufferCountGLPoint + count > _bufferCapacityGLPoint)
    {
        _bufferCapacityGLPoint += MAX(_bufferCapacityGLPoint, count);
        _bufferGLPoint = (ccV2F_C4B_T2F*)realloc(_bufferGLPoint, _bufferCapacityGLPoint*sizeof(ccV2F_C4B_T2F));
    }
}

void CADrawView::ensureCapacityGLLine(int count)
{
    CCASSERT(count>=0, "capacity must be >= 0");
    
    if(_bufferCountGLLine + count > _bufferCapacityGLLine)
    {
        _bufferCapacityGLLine += MAX(_bufferCapacityGLLine, count);
        _bufferGLLine = (ccV2F_C4B_T2F*)realloc(_bufferGLLine, _bufferCapacityGLLine*sizeof(ccV2F_C4B_T2F));
    }
}

bool CADrawView::init()
{
    _blendFunc = BlendFunc_alpha_premultiplied;
    
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR));
    
    ensureCapacity(512);
    ensureCapacityGLPoint(64);
    ensureCapacityGLLine(256);
    
    if (CAConfiguration::getInstance()->supportsShareableVAO())
    {
#if CC_TEXTURE_ATLAS_USE_VAO
        glGenVertexArrays(1, &_vao);
        GL::bindVAO(_vao);
#endif
        
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
        // texcood
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));

#if CC_TEXTURE_ATLAS_USE_VAO
        glGenVertexArrays(1, &_vaoGLLine);
        GL::bindVAO(_vaoGLLine);
#endif
        
        glGenBuffers(1, &_vboGLLine);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
        // texcood
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
    
#if CC_TEXTURE_ATLAS_USE_VAO
        glGenVertexArrays(1, &_vaoGLPoint);
        GL::bindVAO(_vaoGLPoint);
#endif
        
        glGenBuffers(1, &_vboGLPoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
        // Texture coord as pointsize
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
        
        GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    }
    else
    {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
        
        glGenBuffers(1, &_vboGLLine);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        
        glGenBuffers(1, &_vboGLPoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    CHECK_GL_ERROR_DEBUG();
    
    _dirty = true;
    _dirtyGLLine = true;
    _dirtyGLPoint = true;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    // Need to listen the event only when not use batchnode, because it will use VBO
    CANotificationCenter::getInstance()->addObserver([this](CAObject* obj)
    {
        this->init();
    }, this, EVENT_COME_TO_FOREGROUND);
#endif
    
    return true;
}

void CADrawView::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if(_bufferCount)
    {
        _customCommand.init(0, transform, flags);
        _customCommand.func = std::bind(&CADrawView::onDraw, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
    
    if(_bufferCountGLPoint)
    {
        _customCommandGLPoint.init(0, transform, flags);
        _customCommandGLPoint.func = std::bind(&CADrawView::onDrawGLPoint, this, transform, flags);
        renderer->addCommand(&_customCommandGLPoint);
    }
    
    if(_bufferCountGLLine)
    {
        _customCommandGLLine.init(0, transform, flags);
        _customCommandGLLine.func = std::bind(&CADrawView::onDrawGLLine, this, transform, flags);
        renderer->addCommand(&_customCommandGLLine);
    }
}

void CADrawView::onDraw(const Mat4 &transform, uint32_t flags)
{
    getGLProgramState()->apply(transform);
    
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    if (_dirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);
        
        _dirty = false;
    }
    if (CAConfiguration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vao);
    }
    else
    {
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
        // texcood
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
    }
    
    glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (CAConfiguration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCount);
    CHECK_GL_ERROR_DEBUG();
}

void CADrawView::onDrawGLLine(const Mat4 &transform, uint32_t flags)
{
    auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR);
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    if (_dirtyGLLine)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        _dirtyGLLine = false;
    }
    if (CAConfiguration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vaoGLLine);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
        // texcood
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
    }
    
    glLineWidth(_lineWidth);
    glDrawArrays(GL_LINES, 0, _bufferCountGLLine);
    
    if (CAConfiguration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCountGLLine);
    
    CHECK_GL_ERROR_DEBUG();
}

void CADrawView::onDrawGLPoint(const Mat4 &transform, uint32_t flags)
{
    auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE);
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    if (_dirtyGLPoint)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
        
        _dirtyGLPoint = false;
    }
    
    if (CAConfiguration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vaoGLPoint);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
    }
    
    glDrawArrays(GL_POINTS, 0, _bufferCountGLPoint);
    
    if (CAConfiguration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCountGLPoint);
    CHECK_GL_ERROR_DEBUG();
}

void CADrawView::drawPoint(const Vec2& position, const float pointSize, const CAColor4B &color)
{
    ensureCapacityGLPoint(1);
    
    ccV2F_C4B_T2F *point = (ccV2F_C4B_T2F*)(_bufferGLPoint + _bufferCountGLPoint);
    ccV2F_C4B_T2F a = {position, color, tex2(pointSize,0)};
    *point = a;
    
    _bufferCountGLPoint += 1;

    _dirtyGLPoint = true;}

void CADrawView::drawPoints(const Vec2 *position, unsigned int numberOfPoints, const CAColor4B &color)
{
    drawPoints(position, numberOfPoints, 1.0, color);
}

void CADrawView::drawPoints(const Vec2 *position, unsigned int numberOfPoints, const float pointSize, const CAColor4B &color)
{
    ensureCapacityGLPoint(numberOfPoints);
    
    ccV2F_C4B_T2F *point = (ccV2F_C4B_T2F*)(_bufferGLPoint + _bufferCountGLPoint);
    
    for(unsigned int i=0; i < numberOfPoints; i++,point++)
    {
        ccV2F_C4B_T2F a = {position[i], color, tex2(pointSize,0)};
        *point = a;
    }
    
    _bufferCountGLPoint += numberOfPoints;
    _dirtyGLPoint = true;
}

void CADrawView::drawLine(const Vec2 &origin, const Vec2 &destination, const CAColor4B &color)
{
    ensureCapacityGLLine(2);
    
    ccV2F_C4B_T2F *point = (ccV2F_C4B_T2F*)(_bufferGLLine + _bufferCountGLLine);
    
    ccV2F_C4B_T2F a = {origin, color, tex2(0.0, 0.0)};
    ccV2F_C4B_T2F b = {destination, color, tex2(0.0, 0.0)};
    
    *point = a;
    *(point+1) = b;
    
    _bufferCountGLLine += 2;
    _dirtyGLLine = true;
}

void CADrawView::drawRect(const Vec2 &origin, const Vec2 &destination, const CAColor4B &color)
{
    drawLine(Vec2(origin.x, origin.y), Vec2(destination.x, origin.y), color);
    drawLine(Vec2(destination.x, origin.y), Vec2(destination.x, destination.y), color);
    drawLine(Vec2(destination.x, destination.y), Vec2(origin.x, destination.y), color);
    drawLine(Vec2(origin.x, destination.y), Vec2(origin.x, origin.y), color);
}

void CADrawView::drawPoly(const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const CAColor4B &color)
{
    unsigned int vertext_count;
    if(closePolygon)
    {
        vertext_count = 2 * numberOfPoints;
        ensureCapacityGLLine(vertext_count);
    }
    else
    {
        vertext_count = 2 * (numberOfPoints - 1);
        ensureCapacityGLLine(vertext_count);
    }
    
    ccV2F_C4B_T2F *point = (ccV2F_C4B_T2F*)(_bufferGLLine + _bufferCountGLLine);
    
    unsigned int i = 0;
    for(; i<numberOfPoints-1; i++)
    {
        ccV2F_C4B_T2F a = {poli[i], color, tex2(0.0, 0.0)};
        ccV2F_C4B_T2F b = {poli[i+1], color, tex2(0.0, 0.0)};
        
        *point = a;
        *(point+1) = b;
        point += 2;
    }
    if(closePolygon)
    {
        ccV2F_C4B_T2F a = {poli[i], color, tex2(0.0, 0.0)};
        ccV2F_C4B_T2F b = {poli[0], color, tex2(0.0, 0.0)};
        *point = a;
        *(point+1) = b;
    }
    
    _bufferCountGLLine += vertext_count;
}

void CADrawView::drawCircle(const Vec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const CAColor4B &color)
{
    const float coef = 2.0f * (float)M_PI/segments;
    
    Vec2 *vertices = new (std::nothrow) Vec2[segments+2];
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + angle) * scaleX + center.x;
        GLfloat k = radius * sinf(rads + angle) * scaleY + center.y;
        
        vertices[i].x = j;
        vertices[i].y = k;
    }
    if(drawLineToCenter)
    {
        vertices[segments+1].x = center.x;
        vertices[segments+1].y = center.y;
        drawPoly(vertices, segments+2, true, color);
    }
    else
        drawPoly(vertices, segments+1, true, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void CADrawView::drawCircle(const Vec2 &center, float radius, float angle, unsigned int segments, bool drawLineToCenter, const CAColor4B &color)
{
    drawCircle(center, radius, angle, segments, drawLineToCenter, 1.0f, 1.0f, color);
}

void CADrawView::drawQuadBezier(const Vec2 &origin, const Vec2 &control, const Vec2 &destination, unsigned int segments, const CAColor4B &color)
{
    Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
    if( ! vertices )
        return;
    
    float t = 0.0f;
    for(unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 2) * origin.x + 2.0f * (1 - t) * t * control.x + t * t * destination.x;
        vertices[i].y = powf(1 - t, 2) * origin.y + 2.0f * (1 - t) * t * control.y + t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;
    
    drawPoly(vertices, segments+1, false, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void CADrawView::drawCubicBezier(const Vec2 &origin, const Vec2 &control1, const Vec2 &control2, const Vec2 &destination, unsigned int segments, const CAColor4B &color)
{
    Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
    if( ! vertices )
        return;
    
    float t = 0;
    for (unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 3) * origin.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * destination.x;
        vertices[i].y = powf(1 - t, 3) * origin.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;
    
    drawPoly(vertices, segments+1, false, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void CADrawView::drawCardinalSpline(PointArray *config, float tension,  unsigned int segments, const CAColor4B &color)
{
    Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
    if( ! vertices )
        return;
    
    ssize_t p;
    float lt;
    float deltaT = 1.0f / config->count();
    
    for( unsigned int i=0; i < segments+1;i++) {
        
        float dt = (float)i / segments;
        
        // border
        if( dt == 1 ) {
            p = config->count() - 1;
            lt = 1;
        } else {
            p = dt / deltaT;
            lt = (dt - deltaT * (float)p) / deltaT;
        }
        
        // Interpolate
        Vec2 pp0 = config->getControlPointAtIndex(p-1);
        Vec2 pp1 = config->getControlPointAtIndex(p+0);
        Vec2 pp2 = config->getControlPointAtIndex(p+1);
        Vec2 pp3 = config->getControlPointAtIndex(p+2);
        
        Vec2 newPos = ccCardinalSplineAt( pp0, pp1, pp2, pp3, tension, lt);
        vertices[i].x = newPos.x;
        vertices[i].y = newPos.y;
    }
    
    drawPoly(vertices, segments+1, false, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void CADrawView::drawCatmullRom(PointArray *points, unsigned int segments, const CAColor4B &color)
{
    drawCardinalSpline( points, 0.5f, segments, color);
}

void CADrawView::drawDot(const Vec2 &pos, float radius, const CAColor4B &color)
{
    unsigned int vertex_count = 2*3;
    ensureCapacity(vertex_count);
    
    ccV2F_C4B_T2F a = {Vec2(pos.x - radius, pos.y - radius), color, tex2(-1.0, -1.0) };
    ccV2F_C4B_T2F b = {Vec2(pos.x - radius, pos.y + radius), color, tex2(-1.0,  1.0) };
    ccV2F_C4B_T2F c = {Vec2(pos.x + radius, pos.y + radius), color, tex2( 1.0,  1.0) };
    ccV2F_C4B_T2F d = {Vec2(pos.x + radius, pos.y - radius), color, tex2( 1.0, -1.0) };
    
    ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    ccV2F_C4B_T2F_Triangle triangle0 = {a, b, c};
    ccV2F_C4B_T2F_Triangle triangle1 = {a, c, d};
    triangles[0] = triangle0;
    triangles[1] = triangle1;
    
    _bufferCount += vertex_count;
    
    _dirty = true;
}

void CADrawView::drawRect(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2& p4, const CAColor4B &color)
{
    drawLine(Vec2(p1.x, p1.y), Vec2(p2.x, p2.y), color);
    drawLine(Vec2(p2.x, p2.y), Vec2(p3.x, p3.y), color);
    drawLine(Vec2(p3.x, p3.y), Vec2(p4.x, p4.y), color);
    drawLine(Vec2(p4.x, p4.y), Vec2(p1.x, p1.y), color);
}

void CADrawView::drawSegment(const Vec2 &from, const Vec2 &to, float radius, const CAColor4B &color)
{
    unsigned int vertex_count = 6*3;
    ensureCapacity(vertex_count);
    
    Vec2 a = __v2f(from);
    Vec2 b = __v2f(to);
    
    
    Vec2 n = v2fnormalize(v2fperp(v2fsub(b, a)));
    Vec2 t = v2fperp(n);
    
    Vec2 nw = v2fmult(n, radius);
    Vec2 tw = v2fmult(t, radius);
    Vec2 v0 = v2fsub(b, v2fadd(nw, tw));
    Vec2 v1 = v2fadd(b, v2fsub(nw, tw));
    Vec2 v2 = v2fsub(b, nw);
    Vec2 v3 = v2fadd(b, nw);
    Vec2 v4 = v2fsub(a, nw);
    Vec2 v5 = v2fadd(a, nw);
    Vec2 v6 = v2fsub(a, v2fsub(nw, tw));
    Vec2 v7 = v2fadd(a, v2fadd(nw, tw));
    
    
    ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    
    ccV2F_C4B_T2F_Triangle triangles0 = {
        {v0, color, __t(v2fneg(v2fadd(n, t)))},
        {v1, color, __t(v2fsub(n, t))},
        {v2, color, __t(v2fneg(n))},
    };
    triangles[0] = triangles0;
    
    ccV2F_C4B_T2F_Triangle triangles1 = {
        {v3, color, __t(n)},
        {v1, color, __t(v2fsub(n, t))},
        {v2, color, __t(v2fneg(n))},
    };
    triangles[1] = triangles1;
    
    ccV2F_C4B_T2F_Triangle triangles2 = {
        {v3, color, __t(n)},
        {v4, color, __t(v2fneg(n))},
        {v2, color, __t(v2fneg(n))},
    };
    triangles[2] = triangles2;
    
    ccV2F_C4B_T2F_Triangle triangles3 = {
        {v3, color, __t(n)},
        {v4, color, __t(v2fneg(n))},
        {v5, color, __t(n) },
    };
    triangles[3] = triangles3;
    
    ccV2F_C4B_T2F_Triangle triangles4 = {
        {v6, color, __t(v2fsub(t, n))},
        {v4, color, __t(v2fneg(n)) },
        {v5, color, __t(n)},
    };
    triangles[4] = triangles4;
    
    ccV2F_C4B_T2F_Triangle triangles5 = {
        {v6, color, __t(v2fsub(t, n))},
        {v7, color, __t(v2fadd(n, t))},
        {v5, color, __t(n)},
    };
    triangles[5] = triangles5;
    
    _bufferCount += vertex_count;
    
    _dirty = true;
}

void CADrawView::drawPolygon(const Vec2 *verts, int count, const CAColor4B &fillColor, float borderWidth, const CAColor4B &borderColor)
{
    CCASSERT(count >= 0, "invalid count value");
    
    bool outline = (borderColor.a > 0.0f && borderWidth > 0.0f);
    
    auto  triangle_count = outline ? (3*count - 2) : (count - 2);
    auto vertex_count = 3*triangle_count;
    ensureCapacity(vertex_count);
    
    ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    ccV2F_C4B_T2F_Triangle *cursor = triangles;
    
    for (int i = 0; i < count-2; i++)
    {
        ccV2F_C4B_T2F_Triangle tmp = {
            {verts[0], fillColor, __t(v2fzero)},
            {verts[i+1], fillColor, __t(v2fzero)},
            {verts[i+2], fillColor, __t(v2fzero)},
        };
        
        *cursor++ = tmp;
    }
    
    if(outline)
    {
        struct ExtrudeVerts {Vec2 offset, n;};
        struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts)*count);
        memset(extrude, 0, sizeof(struct ExtrudeVerts)*count);
        
        for (int i = 0; i < count; i++)
        {
            Vec2 v0 = __v2f(verts[(i-1+count)%count]);
            Vec2 v1 = __v2f(verts[i]);
            Vec2 v2 = __v2f(verts[(i+1)%count]);
            
            Vec2 n1 = v2fnormalize(v2fperp(v2fsub(v1, v0)));
            Vec2 n2 = v2fnormalize(v2fperp(v2fsub(v2, v1)));
            
            Vec2 offset = v2fmult(v2fadd(n1, n2), 1.0f / (v2fdot(n1, n2) + 1.0f));
            struct ExtrudeVerts tmp = {offset, n2};
            extrude[i] = tmp;
        }
        
        for(int i = 0; i < count; i++)
        {
            int j = (i+1)%count;
            Vec2 v0 = __v2f(verts[i]);
            Vec2 v1 = __v2f(verts[j]);
            
            Vec2 n0 = extrude[i].n;
            
            Vec2 offset0 = extrude[i].offset;
            Vec2 offset1 = extrude[j].offset;
            
            Vec2 inner0 = v2fsub(v0, v2fmult(offset0, borderWidth));
            Vec2 inner1 = v2fsub(v1, v2fmult(offset1, borderWidth));
            Vec2 outer0 = v2fadd(v0, v2fmult(offset0, borderWidth));
            Vec2 outer1 = v2fadd(v1, v2fmult(offset1, borderWidth));
            
            ccV2F_C4B_T2F_Triangle tmp1 = {
                {inner0, borderColor, __t(v2fneg(n0))},
                {inner1, borderColor, __t(v2fneg(n0))},
                {outer1, borderColor, __t(n0)}
            };
            *cursor++ = tmp1;
            
            ccV2F_C4B_T2F_Triangle tmp2 = {
                {inner0, borderColor, __t(v2fneg(n0))},
                {outer0, borderColor, __t(n0)},
                {outer1, borderColor, __t(n0)}
            };
            *cursor++ = tmp2;
        }
        
        free(extrude);
    }
    
    _bufferCount += vertex_count;
    
    _dirty = true;
}

void CADrawView::drawSolidRect(const Vec2 &origin, const Vec2 &destination, const CAColor4B &color)
{
    Vec2 vertices[] = {
        origin,
        Vec2(destination.x, origin.y),
        destination,
        Vec2(origin.x, destination.y)
    };
    
    drawSolidPoly(vertices, 4, color );
}

void CADrawView::drawSolidPoly(const Vec2 *poli, unsigned int numberOfPoints, const CAColor4B &color)
{
    drawPolygon(poli, numberOfPoints, color, 0.0, CAColor4B(0, 0, 0, 0));
}

void CADrawView::drawSolidCircle(const Vec2& center, float radius, float angle, unsigned int segments, float scaleX, float scaleY, const CAColor4B &color)
{
    const float coef = 2.0f * (float)M_PI/segments;
    
    Vec2 *vertices = new (std::nothrow) Vec2[segments];
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i < segments; i++)
    {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + angle) * scaleX + center.x;
        GLfloat k = radius * sinf(rads + angle) * scaleY + center.y;
        
        vertices[i].x = j;
        vertices[i].y = k;
    }
    
    drawSolidPoly(vertices, segments, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void CADrawView::drawSolidCircle( const Vec2& center, float radius, float angle, unsigned int segments, const CAColor4B& color)
{
    drawSolidCircle(center, radius, angle, segments, 1.0f, 1.0f, color);
}

void CADrawView::drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const CAColor4B &color)
{
    unsigned int vertex_count = 3;
    ensureCapacity(vertex_count);
    
    CAColor4B col = color;
    ccV2F_C4B_T2F a = {Vec2(p1.x, p1.y), col, tex2(0.0, 0.0) };
    ccV2F_C4B_T2F b = {Vec2(p2.x, p2.y), col, tex2(0.0,  0.0) };
    ccV2F_C4B_T2F c = {Vec2(p3.x, p3.y), col, tex2(0.0,  0.0) };
    
    ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    ccV2F_C4B_T2F_Triangle triangle = {a, b, c};
    triangles[0] = triangle;
    
    _bufferCount += vertex_count;
    _dirty = true;
}

void CADrawView::clear()
{
    _bufferCount = 0;
    _dirty = true;
    _bufferCountGLLine = 0;
    _dirtyGLLine = true;
    _bufferCountGLPoint = 0;
    _dirtyGLPoint = true;
    _lineWidth = _defaultLineWidth;
}

const BlendFunc& CADrawView::getBlendFunc() const
{
    return _blendFunc;
}

void CADrawView::setBlendFunc(const BlendFunc &blendFunc)
{
    _blendFunc = blendFunc;
}

void CADrawView::setLineWidth(int lineWidth)
{
    _lineWidth = lineWidth;
}

float CADrawView::getLineWidth()
{
    return this->_lineWidth;
}

NS_CC_END
