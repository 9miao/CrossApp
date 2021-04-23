

#include "renderer/CCPrimitive.h"
#include "renderer/CCVertexIndexBuffer.h"

NS_CC_BEGIN

Primitive* Primitive::create(VertexData* verts, IndexBuffer* indices, int type)
{
    auto result = new (std::nothrow) Primitive();
    if( result && result->init(verts, indices, type))
    {
        result->autorelease();
        return result;
    }
    
    CC_SAFE_DELETE(result);
    return  nullptr;
}

const VertexData* Primitive::getVertexData() const
{
    return _verts;
}

const IndexBuffer* Primitive::getIndexData() const
{
    return _indices;
}

Primitive::Primitive()
: _verts(nullptr)
, _indices(nullptr)
, _type(GL_POINTS)
, _start(0)
, _count(0)
{
}

Primitive::~Primitive()
{
    CC_SAFE_RELEASE_NULL(_verts);
    CC_SAFE_RELEASE_NULL(_indices);
}

bool Primitive::init(VertexData* verts, IndexBuffer* indices, int type)
{
    if( nullptr == verts ) return false;
    if(verts != _verts)
    {
        CC_SAFE_RELEASE(_verts);
        CC_SAFE_RETAIN(verts);
        _verts = verts;
    }
    
    if(indices != _indices)
    {
        CC_SAFE_RETAIN(indices);
        CC_SAFE_RELEASE(_indices);
        _indices = indices;
    }

    _type = type;
    
    return true;
}

void Primitive::draw()
{
    if(_verts)
    {
        _verts->use();
        if(_indices!= nullptr)
        {
            GLenum type = (_indices->getType() == IndexBuffer::IndexType::INDEX_TYPE_SHORT_16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices->getVBO());
            size_t offset = _start * _indices->getSizePerIndex();
            glDrawElements((GLenum)_type, _count, type, (GLvoid*)offset);
        }
        else
        {
            glDrawArrays((GLenum)_type, _start, _count);
        }
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Primitive::setStart(int start)
{
    _start = start;
}

void Primitive::setCount(int count)
{
    _count = count;
}

NS_CC_END
