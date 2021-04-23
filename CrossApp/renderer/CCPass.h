

#ifndef __CossApp_libs__CCPass__
#define __CossApp_libs__CCPass__

#include <stdio.h>

#include "platform/CCPlatformMacros.h"
#include "renderer/CCRenderState.h"

NS_CC_BEGIN

class GLProgramState;
class Technique;

class CC_DLL Pass : public RenderState
{
    friend class Material;

public:
    /** Creates a Pass with a GLProgramState.
     */
    static Pass* createWithGLProgramState(Technique* parent, GLProgramState* programState);

    static Pass* create(Technique* parent);

    /** Returns the GLProgramState */
    GLProgramState* getGLProgramState() const;

    /** Binds the GLProgramState and the RenderState.
     This method must be called before call the actual draw call.
     */
    void bind(const Mat4& modelView);
    void bind(const Mat4& modelView, bool bindAttributes);

    /** Unbinds the Pass.
     This method must be called AFTER calling the actual draw call
     */
    void unbind();

    uint32_t getHash() const;

    /**
     * Returns a clone (deep-copy) of this instance */
    Pass* clone() const;

protected:
    Pass();
    ~Pass();
    bool init(Technique* parent);
    bool initWithGLProgramState(Technique* parent, GLProgramState *glProgramState);

    void setGLProgramState(GLProgramState* glProgramState);
    void* getTarget() const;

    GLProgramState* _glProgramState;
};

NS_CC_END



#endif /* defined(__CossApp_libs__CCPass__) */
