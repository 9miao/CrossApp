

#ifndef __CossApp_libs__CCTechnique__
#define __CossApp_libs__CCTechnique__

#include <string>
#include "renderer/CCRenderState.h"
#include "renderer/CCPass.h"
#include "basics/CAObject.h"
#include "platform/CCPlatformMacros.h"
#include "basics/CASTLContainer.h"

NS_CC_BEGIN

class Pass;
class GLProgramState;
class Material;

/// Technique
class CC_DLL Technique : public RenderState
{
    friend class Material;
    friend class Renderer;
    friend class Pass;
    friend class MeshCommand;
    friend class Mesh;

public:
    /** Creates a new Technique with a GLProgramState.
     Method added to support legacy code
     */
    static Technique* createWithGLProgramState(Material* parent, GLProgramState* state);
    static Technique* create(Material* parent);

    /** Adds a new pass to the Technique.
     Order matters. First added, first rendered
     */
    void addPass(Pass* pass);

    /** Returns the name of the Technique */
    std::string getName() const;

    /** Returns the Pass at given index */
    Pass* getPassByIndex(ssize_t index) const;

    /** Returns the number of Passes in the Technique */
    ssize_t getPassCount() const;

    /** Returns the list of passes */
    const CAVector<Pass*>& getPasses() const;

    /** Returns a new clone of the Technique */
    Technique* clone() const;

protected:
    Technique();
    ~Technique();
    bool init(Material* parent);

    void setName(const std::string& name);

    std::string _name;
    CAVector<Pass*> _passes;
};

NS_CC_END

#endif /* defined(__CossApp_libs__CCTechnique__) */
