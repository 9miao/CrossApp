
#ifndef _CC_MESHCOMMAND_H_
#define _CC_MESHCOMMAND_H_

#include <unordered_map>
#include "renderer/CCRenderCommand.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCRenderState.h"
#include "math/CAMath.h"

NS_CC_BEGIN

class GLProgramState;
class Material;

//it is a common mesh
class CC_DLL MeshCommand : public RenderCommand
{
public:

    MeshCommand();
    virtual ~MeshCommand();

    void init(float zOrder, Material* material, GLuint vertexBuffer, GLuint indexBuffer, GLenum primitive, GLenum indexFormat, ssize_t indexCount, const Mat4 &mv, uint32_t flags);

    void init(float zOrder, GLuint textureID, GLProgramState* glProgramState, RenderState::StateBlock* stateBlock, GLuint vertexBuffer, GLuint indexBuffer, GLenum primitive, GLenum indexFormat, ssize_t indexCount, const Mat4 &mv, uint32_t flags);

    void setDisplayColor(const Vec4& color);
    void setMatrixPalette(const Vec4* matrixPalette);
    void setMatrixPaletteSize(int size);
    void setLightMask(unsigned int lightmask);

    void execute();
    
    //used for batch
    void preBatchDraw();
    void batchDraw();
    void postBatchDraw();
    
    void genMaterialID(GLuint texID, void* glProgramState, GLuint vertexBuffer, GLuint indexBuffer, BlendFunc blend);
    
    uint32_t getMaterialID() const;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    void listenRendererRecreated();
#endif

protected:
    //build & release vao
    void buildVAO();
    void releaseVAO();
    
    // apply renderstate, not used when using material
    void applyRenderState();


    Vec4 _displayColor; // in order to support tint and fade in fade out
    
    // used for skin
    const Vec4* _matrixPalette;
    int   _matrixPaletteSize;
    
    uint32_t _materialID; //material ID
    
    GLuint   _vao; //use vao if possible
    
    GLuint _vertexBuffer;
    GLuint _indexBuffer;
    GLenum _primitive;
    GLenum _indexFormat;
    ssize_t _indexCount;
    
    // States, default value all false


    // ModelView transform
    Mat4 _mv;

    // Mode A: Material
    // weak ref
    Material* _material;

    // Mode B: StateBlock
    // weak ref
    GLProgramState* _glProgramState;
    RenderState::StateBlock* _stateBlock;
    GLuint _textureID;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    CAObject* _notificationTarget;
#endif

};

NS_CC_END

#endif //_CC_MESHCOMMAND_H_
