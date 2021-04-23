
#include "renderer/CCQuadCommand.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCMaterial.h"
#include "renderer/CCTechnique.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCPass.h"
#include "support/xxhash/xxhash.h"
#include <algorithm>

NS_CC_BEGIN

int QuadCommand::__indexCapacity = -1;
GLushort* QuadCommand::__indices = nullptr;

QuadCommand::QuadCommand():
_indexSize(-1),
_ownedIndices()
{
}

QuadCommand::~QuadCommand()
{
    for (auto& indices : _ownedIndices)
    {
        CC_SAFE_DELETE_ARRAY(indices);
    }
}

void QuadCommand::init(float globalOrder, GLuint textureID, GLProgramState* glProgramState, const BlendFunc& blendType, ccV3F_C4B_T2F_Quad* quads, ssize_t quadCount,
                       const Mat4& mv, uint32_t flags)
{
    CCASSERT(glProgramState, "Invalid GLProgramState");
    CCASSERT(glProgramState->getVertexAttribsFlags() == 0, "No custom attributes are supported in QuadCommand");

    if (quadCount * 6 > _indexSize)
        reIndex((int)quadCount*6);

    Triangles triangles;
    triangles.verts = &quads->tl;
    triangles.vertCount = (int)quadCount * 4;
    triangles.indices = __indices;
    triangles.indexCount = (int)quadCount * 6;
    TrianglesCommand::init(globalOrder, textureID, glProgramState, blendType, triangles, mv, flags);
}

void QuadCommand::reIndex(int indicesCount)
{
    // first time init: create a decent buffer size for indices to prevent too much resizing
    if (__indexCapacity == -1)
    {
        indicesCount = MAX(indicesCount, 2048);
    }

    if (indicesCount > __indexCapacity)
    {
        // if resizing is needed, get needed size plus 25%, but not bigger that max size
        indicesCount *= 1.25;
        indicesCount = MIN(indicesCount, 65536);

        CCLOG("CrossApp: QuadCommand: resizing index size from [%d] to [%d]", __indexCapacity, indicesCount);

        _ownedIndices.push_back(__indices);
        __indices = new (std::nothrow) GLushort[indicesCount];
        __indexCapacity = indicesCount;
    }

    for( int i=0; i < __indexCapacity/6; i++)
    {
        __indices[i*6+0] = (GLushort) (i*4+0);
        __indices[i*6+1] = (GLushort) (i*4+1);
        __indices[i*6+2] = (GLushort) (i*4+2);
        __indices[i*6+3] = (GLushort) (i*4+3);
        __indices[i*6+4] = (GLushort) (i*4+2);
        __indices[i*6+5] = (GLushort) (i*4+1);
    }

    _indexSize = indicesCount;
}

NS_CC_END
