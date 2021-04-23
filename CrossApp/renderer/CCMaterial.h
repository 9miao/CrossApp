

#ifndef __CossApp_libs__CCMaterial__
#define __CossApp_libs__CCMaterial__

#include <string>

#include "renderer/CCRenderState.h"
#include "renderer/CCTechnique.h"
#include "basics/CAObject.h"
#include "basics/CASTLContainer.h"
#include "basics/CAPoint.h"
#include "basics/CAPoint3D.h"
#include "basics/CAVec4.h"
#include "math/CAMat4.h"
#include "platform/CCPlatformMacros.h"


NS_CC_BEGIN

class Technique;
class Pass;
class GLProgramState;
class Properties;

/// Material
class CC_DLL Material : public RenderState
{
    friend class CAView;
    friend class Technique;
    friend class Pass;
    friend class MeshCommand;
    friend class Renderer;
    friend class Mesh;

public:
    /**
     * Creates a Material using the data from the Properties object defined at the specified URL,
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional).
     *
     * @param url The URL pointing to the Properties object defining the material.
     *
     * @return A new Material or NULL if there was an error.
     */
    static Material* createWithFilename(const std::string& path);

    /** Creates a Material with a GLProgramState.
     It will only contain one Technique and one Pass.
     Added in order to support legacy code.
     */
    static Material* createWithGLStateProgram(GLProgramState* programState);

    /**
     * Creates a material from the specified properties object.
     *
     * @param materialProperties The properties object defining the
     *      material (must have namespace equal to 'material').
     * @return A new Material.
     */
    static Material* createWithProperties(Properties* materialProperties);

    /// returns the material name
    std::string getName() const;
    /// sets the material name
    void setName(const std::string& name);

    /** Returns a Technique by its name.
     returns `nullptr` if the Technique can't be found.
     */
    Technique* getTechniqueByName(const std::string& name);

    /** Returns a Technique by index. 
     returns `nullptr` if the index is invalid.
     */
    Technique* getTechniqueByIndex(ssize_t index);

    /** Returns the Technique used by the Material */
    Technique* getTechnique() const;

    /** Returns the list of Techniques */
    const CAVector<Technique*>& getTechniques() const;

    /** Returns the number of Techniques in the Material. */
    ssize_t getTechniqueCount() const;

    /** Adds a Technique into the Material */
    void addTechnique(Technique* technique);

    /** Sets the current technique */
    void setTechnique(const std::string& techniqueName);

    /** returns a clone (deep-copy) of the material */
    virtual Material* clone() const;

protected:
    Material();
    ~Material();
    bool initWithGLProgramState(GLProgramState* state);
    bool initWithFile(const std::string& file);
    bool initWithProperties(Properties* materialProperties);

    void setTarget(void* target);
    
    bool parseProperties(Properties* properties);
    bool parseTechnique(Properties* properties);
    bool parsePass(Technique* technique, Properties* properties);
    bool parseShader(Pass* pass, Properties* properties);
    bool parseSampler(GLProgramState* glProgramState, Properties* properties);
    bool parseUniform(GLProgramState* programState, Properties* properties, const char* uniformName);
    bool parseRenderState(RenderState* renderState, Properties* properties);
    
    
    // material name
    std::string _name;

    // array of techniques
    CAVector<Technique*> _techniques;

    // weak pointer since it is being help by _techniques
    Technique* _currentTechnique;

    void* _target;
};

NS_CC_END


#endif /* defined(__CossApp_libs__CCMaterial__) */
