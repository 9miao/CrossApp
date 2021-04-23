

#ifndef __CC_PRIMITIVE_H__
#define __CC_PRIMITIVE_H__

#include "renderer/CCVertexIndexData.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_CC_BEGIN

class IndexBuffer;

/**
 Primitive can support sending points, lines and triangles to glpipeline, which is an abstraction
 of primitive data.
 */
class CC_DLL Primitive : public CAObject
{
public:
    /**
     Create an instance of primitive.
     @param verts VertexData used in the primitive.
     @param indices Optional index data.
     @param type The type (Points, Lines, Triangles) used.
     */
    static Primitive* create(VertexData* verts, IndexBuffer* indices, int type);
    /**Get the vertexData.*/
    const VertexData* getVertexData() const;
    /**Get the optional index data, will return null if index data is not used.*/
    const IndexBuffer* getIndexData() const;
    /**Get the primitive type.*/
    int getType() const { return _type; }
    
    /**called by rendering framework, will send the data to GLPipeline.*/
    void draw();
    
    /**Get the start index of primitive.*/
    int getStart() const { return _start; }
    /**Get the number of vertices or indices used for drawing.*/
    int getCount() const { return _count; }
    /**Setter for the start index.*/
    void setStart(int start);
    /**Setter for the count. */
    void setCount(int count);
    
protected:
    Primitive();
    virtual ~Primitive();
    
    bool init(VertexData* verts, IndexBuffer* indices, int type);
    
protected:
    VertexData* _verts;
    IndexBuffer* _indices;
    int _start;
    int _count;
    int _type;
};

NS_CC_END

/**
 end of support group
 @}
 */
#endif //__CC_PRIMITIVE_H__
