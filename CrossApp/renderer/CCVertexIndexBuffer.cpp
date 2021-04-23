
#include "renderer/CCVertexIndexBuffer.h"
#include "basics/CAApplication.h"
#include "basics/CANotificationCenter.h"
NS_CC_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
bool s_vertexBufferEnableShadowCopy = true;
bool s_indexBufferEnableShadowCopy = true;
#else
bool s_vertexBufferEnableShadowCopy = false;
bool s_indexBufferEnableShadowCopy = false;
#endif

bool VertexBuffer::isShadowCopyEnabled()
{
	return s_vertexBufferEnableShadowCopy;
}

void VertexBuffer::enableShadowCopy(bool enabled)
{
	s_vertexBufferEnableShadowCopy = enabled;
}

bool IndexBuffer::isShadowCopyEnabled()
{
	return s_indexBufferEnableShadowCopy;
}

void IndexBuffer::enableShadowCopy(bool enabled)
{
	s_indexBufferEnableShadowCopy = enabled;
}

VertexBuffer* VertexBuffer::create(int sizePerVertex, int vertexNumber, GLenum usage/* = GL_STATIC_DRAW*/)
{
    auto result = new (std::nothrow) VertexBuffer();
    if(result && result->init(sizePerVertex, vertexNumber, usage))
    {
        result->autorelease();
        return result;
    }
    CC_SAFE_DELETE(result);
    return nullptr;
    
}

VertexBuffer::VertexBuffer()
: _vbo(0)
, _sizePerVertex(0)
, _vertexNumber(0)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    CANotificationCenter::getInstance()->addObserver([this](CAObject* obj)
    {
        this->recreateVBO();
    }, this, EVENT_COME_TO_FOREGROUND);
#endif
}

VertexBuffer::~VertexBuffer()
{
    if(glIsBuffer(_vbo))
    {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }
}

bool VertexBuffer::init(int sizePerVertex, int vertexNumber, GLenum usage/* = GL_STATIC_DRAW*/)
{
    if(0 == sizePerVertex || 0 == vertexNumber)
        return false;
    _sizePerVertex = sizePerVertex;
    _vertexNumber = vertexNumber;
    _usage = usage;
    
    if(isShadowCopyEnabled())
    {
        _shadowCopy.resize(sizePerVertex * _vertexNumber);
    }
    
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, getSize(), nullptr, _usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
}

int VertexBuffer::getSizePerVertex() const
{
    return _sizePerVertex;
}

int VertexBuffer::getVertexNumber() const
{
    return _vertexNumber;
}

bool VertexBuffer::updateVertices(const void* verts, int count, int begin)
{
    if(count <= 0 || nullptr == verts) return false;
    
    if(begin < 0)
    {
        CCLOG("Update vertices with begin = %d, will set begin to 0", begin);
        begin = 0;
    }
    
    if(count + begin > _vertexNumber)
    {
        CCLOG("updated vertices exceed the max size of vertex buffer, will set count to _vertexNumber-begin");
        count = _vertexNumber - begin;
    }
    
    if(isShadowCopyEnabled())
    {
        memcpy(&_shadowCopy[begin * _sizePerVertex], verts, count * _sizePerVertex);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, begin * _sizePerVertex, count * _sizePerVertex, verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return true;
}

GLuint VertexBuffer::getVBO() const
{
    return _vbo;
}

void VertexBuffer::recreateVBO() const
{
    CCLOG("come to foreground of VertexBuffer");
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    const void* buffer = nullptr;
    if(isShadowCopyEnabled())
    {
        buffer = &_shadowCopy[0];
    }
    CCLOG("recreate IndexBuffer with size %d %d", getSizePerVertex(), _vertexNumber);
    glBufferData(GL_ARRAY_BUFFER, _sizePerVertex * _vertexNumber, buffer, _usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(!glIsBuffer(_vbo))
    {
        CCLOG("recreate VertexBuffer Error");
    }
}

int VertexBuffer::getSize() const
{
    return _sizePerVertex * _vertexNumber;
}

IndexBuffer* IndexBuffer::create(IndexType type, int number, GLenum usage/* = GL_STATIC_DRAW*/)
{
    auto result = new (std::nothrow) IndexBuffer();
    if(result && result->init(type, number, usage))
    {
        result->autorelease();
        return result;
    }
    CC_SAFE_DELETE(result);
    return nullptr;
}

IndexBuffer::IndexBuffer()
: _vbo(0)
, _type(IndexType::INDEX_TYPE_SHORT_16)
, _indexNumber(0)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    CANotificationCenter::getInstance()->addObserver([this](CAObject* obj)
    {
        this->recreateVBO();
    }, this, EVENT_COME_TO_FOREGROUND);
#endif
}

IndexBuffer::~IndexBuffer()
{
    if(glIsBuffer(_vbo))
    {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }
}

bool IndexBuffer::init(IndexBuffer::IndexType type, int number, GLenum usage/* = GL_STATIC_DRAW*/)
{
    if(number <=0 ) return false;
    
    _type = type;
    _indexNumber = number;
    _usage = usage;
    
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getSize(), nullptr, _usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    if(isShadowCopyEnabled())
    {
        _shadowCopy.resize(getSize());
    }
    
    return true;
}

IndexBuffer::IndexType IndexBuffer::getType() const
{
    return _type;
}

int IndexBuffer::getSizePerIndex() const
{
    return IndexType::INDEX_TYPE_SHORT_16 == _type ? 2 : 4;
}

int IndexBuffer::getIndexNumber() const
{
    return _indexNumber;
}

bool IndexBuffer::updateIndices(const void* indices, int count, int begin)
{
    if(count <= 0 || nullptr == indices) return false;
    
    if(begin < 0)
    {
        CCLOG("Update indices with begin = %d, will set begin to 0", begin);
        begin = 0;
    }
    
    if(count + begin > _indexNumber)
    {
        CCLOG("updated indices exceed the max size of vertex buffer, will set count to _indexNumber-begin");
        count = _indexNumber - begin;
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, begin * getSizePerIndex(), count * getSizePerIndex(), indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    if(isShadowCopyEnabled())
    {
        memcpy(&_shadowCopy[begin * getSizePerIndex()], indices, count * getSizePerIndex());
    }
    
    return true;
}

int IndexBuffer::getSize() const
{
    return getSizePerIndex() * _indexNumber;
}

GLuint IndexBuffer::getVBO() const
{
    return _vbo;
}

void IndexBuffer::recreateVBO() const
{
    CCLOG("come to foreground of IndexBuffer");
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    const void* buffer = nullptr;
    if(isShadowCopyEnabled())
    {
        buffer = &_shadowCopy[0];
    }
    CCLOG("recreate IndexBuffer with size %d %d ", getSizePerIndex(), _indexNumber);
    glBufferData(GL_ARRAY_BUFFER, getSize(), buffer, _usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(!glIsBuffer(_vbo))
    {
        CCLOG("recreate IndexBuffer Error");
    }
}

NS_CC_END
